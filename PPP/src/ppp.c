/*------------------------------------------------------------------------------
* ppp.c : precise point positioning
*
*          Copyright (C) 2010-2016 by T.TAKASU, All rights reserved.
*
* options : -DIERS_MODEL  use IERS tide model
*           -DOUTSTAT_AMB output ambiguity parameters to solution status
*
* references :
*    [1] D.D.McCarthy, IERS Technical Note 21, IERS Conventions 1996, July 1996
*    [2] D.D.McCarthy and G.Petit, IERS Technical Note 32, IERS Conventions
*        2003, November 2003
*    [3] D.A.Vallado, Fundamentals of Astrodynamics and Applications 2nd ed,
*        Space Technology Library, 2004
*    [4] J.Kouba, A Guide to using International GNSS Service (IGS) products,
*        May 2009
*    [5] RTCM Paper, April 12, 2010, Proposed SSR Messages for SV Orbit Clock,
*        Code Biases, URA
*    [6] MacMillan et al., Atmospheric gradients and the VLBI terrestrial and
*        celestial reference frames, Geophys. Res. Let., 1997
*    [7] G.Petit and B.Luzum (eds), IERS Technical Note No. 36, IERS
*         Conventions (2010), 2010
*    [8] J.Kouba, A simplified yaw-attitude model for eclipsing GPS satellites,
*        GPS Solutions, 13:1-12, 2009
*    [9] F.Dilssner, GPS IIF-1 satellite antenna phase center and attitude
*        modeling, InsideGNSS, September, 2010
*    [10] F.Dilssner, The GLONASS-M satellite yaw-attitude model, Advances in
*        Space Research, 2010
*    [11] IGS MGEX (http://igs.org/mgex)
*
* version : $Revision:$ $Date:$
* history : 2010/07/20 1.0  new
*                           added api:
*                               tidedisp()
*           2010/12/11 1.1  enable exclusion of eclipsing satellite
*           2012/02/01 1.2  add gps-glonass h/w bias correction
*                           move windupcorr() to rtkcmn.c
*           2013/03/11 1.3  add otl and pole tides corrections
*                           involve iers model with -DIERS_MODEL
*                           change initial variances
*                           suppress acos domain error
*           2013/09/01 1.4  pole tide model by iers 2010
*                           add mode of ionosphere model off
*           2014/05/23 1.5  add output of trop gradient in solution status
*           2014/10/13 1.6  fix bug on P0(a[3]) computation in tide_oload()
*                           fix bug on m2 computation in tide_pole()
*           2015/03/19 1.7  fix bug on ionosphere correction for GLO and BDS
*           2015/05/10 1.8  add function to detect slip by MW-LC jump
*                           fix ppp solutin problem with large clock variance
*           2015/06/08 1.9  add precise satellite yaw-models
*                           cope with day-boundary problem of satellite clock
*           2015/07/31 1.10 fix bug on nan-solution without glonass nav-data
*                           pppoutsolsat() -> pppoutstat()
*           2015/11/13 1.11 add L5-receiver-dcb estimation
*                           merge post-residual validation by rnx2rtkp_test
*                           support support option opt->pppopt=-GAP_RESION=nnnn
*           2016/01/22 1.12 delete support for yaw-model bug
*                           add support for ura of ephemeris
*-----------------------------------------------------------------------------*/
#include "rtklib.h"
extern gtime_t ssr_time;
static const char rcsid[]="$Id:$";
#define SWAP_I(x,y) do {int    _tmp=x; x=y; y=_tmp;} while (0)
#define SWAP_D(x,y) do {double _tmp=x; x=y; y=_tmp;} while (0)
#define SQR(x)      ((x)*(x))
#define SQRT(x)     ((x)<=0.0?0.0:sqrt(x))
#define MAX(x,y)    ((x)>(y)?(x):(y))
#define MIN(x,y)    ((x)<(y)?(x):(y))
#define ROUND(x)    (int)floor((x)+0.5)

#define MAX_ITER    8               /* max number of iterations */
#define MAX_STD_FIX 0.15            /* max std-dev (3d) to fix solution */
#define MIN_NSAT_SOL 4              /* min satellite number for solution */
#define THRES_REJECT 4              /* reject threshold of posfit-res (sigma) */
#define THRES_REJECT_K0 2.5            /* reject threshold of posfit-res (sigma) */
#define THRES_REJECT_K1 4           /* reject threshold of posfit-res (sigma) */

//#define THRES_MW_JUMP 10.0
#define THRES_MW_JUMP 2.5
#define THRES_GF_JUMP 0.05
#define VAR_POS     SQR(5.0)       /* init variance receiver position (m^2) */
#define VAR_CLK     SQR(60.0)       /* init variance receiver clock (m^2) */
#define VAR_ZTD     SQR( 0.6)       /* init variance ztd (m^2) */
#define VAR_GRA     SQR(0.01)       /* init variance gradient (m^2) */
#define VAR_DCB     SQR(30.0)       /* init variance dcb (m^2) */
#define VAR_BIAS    SQR(60.0)       /* init variance phase-bias (m^2) */
#define VAR_IONO    SQR(60.0)       /* init variance iono-delay */
#define VAR_GLO_IFB SQR( 0.6)       /* variance of glonass ifb */

#define ERR_SAAS    0.3             /* saastamoinen model error std (m) */
#define ERR_BRDCI   0.5             /* broadcast iono model error factor */
#define ERR_CBIAS   0.3             /* code bias error std (m) */
#define REL_HUMI    0.7             /* relative humidity for saastamoinen model */
#define GAP_RESION  120             /* default gap to reset ionos parameters (ep) */

#define EFACT_GPS_L5 10.0           /* error factor of GPS/QZS L5 */

#define MUDOT_GPS   (0.00836*D2R)   /* average angular velocity GPS (rad/s) */
#define MUDOT_GLO   (0.00888*D2R)   /* average angular velocity GLO (rad/s) */
#define EPS0_GPS    (13.5*D2R)      /* max shadow crossing angle GPS (rad) */
#define EPS0_GLO    (14.2*D2R)      /* max shadow crossing angle GLO (rad) */
#define T_POSTSHADOW 1800.0         /* post-shadow recovery time (s) */
#define QZS_EC_BETA 20.0            /* max beta angle for qzss Ec (deg) */
#define MU_GPS   3.9860050E14     /* gravitational constant         ref [1] */
#define MU_GLO   3.9860044E14     /* gravitational constant         ref [2] */
#define MU_GAL   3.986004418E14   /* earth gravitational constant   ref [7] */
#define MU_CMP   3.986004418E14   /* earth gravitational constant   ref [9] */
/* number and index of states */
#define NF(opt)     ((opt)->ionoopt==IONOOPT_IFLC?1:(opt)->nf)
#define NP(opt)     ((opt)->dynamics?9:3)
#define NC(opt)     (NSYS)
#define NT(opt)     ((opt)->tropopt<TROPOPT_EST?0:((opt)->tropopt==TROPOPT_EST?1:3))
#define NI(opt)     ((opt)->ionoopt==IONOOPT_EST?MAXSAT:0)
#define ND(opt)     ((opt)->nf>=3?1:0)
#define NR(opt)     (NP(opt)+NC(opt)+NT(opt)+NI(opt)+ND(opt))
#define NB(opt)     (NF(opt)*MAXSAT)
#define NX(opt)     (NR(opt)+NB(opt))
#define IC(s,opt)   (NP(opt)+(s))
#define IT(opt)     (NP(opt)+NC(opt))
#define II(s,opt)   (NP(opt)+NC(opt)+NT(opt)+(s)-1)
#define ID(opt)     (NP(opt)+NC(opt)+NT(opt)+NI(opt))
#define IB(s,f,opt) (NR(opt)+MAXSAT*(f)+(s)-1)

extern fileout_t file_out;
extern FILE* outFp[50];   //输出文件指针
/* standard deviation of state -----------------------------------------------*/
static double STD(rtk_t *rtk, int i)
{
    if (rtk->sol.stat==SOLQ_FIX) return SQRT(rtk->Pa[i+i*rtk->nx]);
    return SQRT(rtk->P[i+i*rtk->nx]);
}
/* write solution status for PPP ---------------------------------------------*/
extern int pppoutstat(rtk_t *rtk, char *buff)
{
    ssat_t *ssat;
    double tow,pos[3],vel[3],acc[3],*x;
    int i,j,week;
    char id[32],*p=buff;
    
    if (!rtk->sol.stat) return 0;
    
    trace(3,"pppoutstat:\n");
    
    tow=time2gpst(rtk->sol.time,&week);
    
    x=rtk->sol.stat==SOLQ_FIX?rtk->xa:rtk->x;
    
    /* receiver position */
    p+=sprintf(p,"$POS,%d,%.3f,%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n",week,tow,
               rtk->sol.stat,x[0],x[1],x[2],STD(rtk,0),STD(rtk,1),STD(rtk,2));

    
    /* receiver velocity and acceleration */
    if (rtk->opt.dynamics) {
        ecef2pos(rtk->sol.rr,pos);
        ecef2enu(pos,rtk->x+3,vel);
        ecef2enu(pos,rtk->x+6,acc);
        p+=sprintf(p,"$VELACC,%d,%.3f,%d,%.4f,%.4f,%.4f,%.5f,%.5f,%.5f,%.4f,%.4f,"
                   "%.4f,%.5f,%.5f,%.5f\n",week,tow,rtk->sol.stat,vel[0],vel[1],
                   vel[2],acc[0],acc[1],acc[2],0.0,0.0,0.0,0.0,0.0,0.0);
    }
    /* receiver clocks */
    i=IC(0,&rtk->opt);
    p+=sprintf(p,"$CLK,%d,%.3f,%d,%d,%.3f,%.3f,%.3f,%.3f\n",
               week,tow,rtk->sol.stat,1,x[i]*1E9/CLIGHT,x[i+1]*1E9/CLIGHT,
               STD(rtk,i)*1E9/CLIGHT,STD(rtk,i+1)*1E9/CLIGHT);
    
    /* tropospheric parameters */
    if (rtk->opt.tropopt==TROPOPT_EST||rtk->opt.tropopt==TROPOPT_ESTG) {
        i=IT(&rtk->opt);
        p+=sprintf(p,"$TROP,%d,%.3f,%d,%d,%.4f,%.4f\n",week,tow,rtk->sol.stat,
                   1,x[i],STD(rtk,i));
    }
    if (rtk->opt.tropopt==TROPOPT_ESTG) {
        i=IT(&rtk->opt);
        p+=sprintf(p,"$TRPG,%d,%.3f,%d,%d,%.5f,%.5f,%.5f,%.5f\n",week,tow,
                   rtk->sol.stat,1,x[i+1],x[i+2],STD(rtk,i+1),STD(rtk,i+2));
    }
    /* ionosphere parameters */
    if (rtk->opt.ionoopt==IONOOPT_EST) {
        for (i=0;i<MAXSAT;i++) {
            ssat=rtk->ssat+i;
            if (!ssat->vs) continue;
            j=II(i+1,&rtk->opt);
            if (rtk->x[j]==0.0) continue;
            satno2id(i+1,id);
            p+=sprintf(p,"$ION,%d,%.3f,%d,%s,%.1f,%.1f,%.4f,%.4f\n",week,tow,
                       rtk->sol.stat,id,rtk->ssat[i].azel[0]*R2D,
                       rtk->ssat[i].azel[1]*R2D,x[j],STD(rtk,j));
        }
    }


	

#ifdef OUTSTAT_AMB
    /* ambiguity parameters */
    for (i=0;i<MAXSAT;i++) for (j=0;j<NF(&rtk->opt);j++) {
        k=IB(i+1,j,&rtk->opt);
        if (rtk->x[k]==0.0) continue;
        satno2id(i+1,id);
        p+=sprintf(p,"$AMB,%d,%.3f,%d,%s,%d,%.4f,%.4f\n",week,tow,
                   rtk->sol.stat,id,j+1,x[k],STD(rtk,k));
    }
#endif
    return p-buff;
}
static double sagnac(const double *rs, const double *rr)
{
	return OMGE * (rs[0] * rr[1] - rs[1] * rr[0]) / CLIGHT;
}

/* exclude meas of eclipsing satellite (block IIA) ---------------------------*/
static void testeclipse(const obsd_t *obs, int n, const nav_t *nav, double *rs)
{
    double rsun[3],esun[3],r,ang,erpv[5]={0},cosa;
    int i,j;
    const char *type;
    
    trace(3,"testeclipse:\n");
    
    /* unit vector of sun direction (ecef) */
    sunmoonpos(gpst2utc(obs[0].time),erpv,rsun,NULL,NULL);
    normv3(rsun,esun);
    
    for (i=0;i<n;i++) {
        type=nav->pcvs[obs[i].sat-1].type;
        
        if ((r=norm(rs+i*6,3))<=0.0) continue;
        
        /* only block IIA */
        if (*type&&!strstr(type,"BLOCK IIA")) continue;
        
        /* sun-earth-satellite angle */
        cosa=dot(rs+i*6,esun,3)/r;
        cosa=cosa<-1.0?-1.0:(cosa>1.0?1.0:cosa);
        ang=acos(cosa);
        
        /* test eclipse */
        if (ang<PI/2.0||r*sin(ang)>RE_WGS84) continue;
        
        trace(3,"eclipsing sat excluded %s sat=%2d\n",time_str(obs[0].time,0),
              obs[i].sat);
        
        for (j=0;j<3;j++) rs[j+i*6]=0.0;
    }
}
/* nominal yaw-angle ---------------------------------------------------------*/
static double yaw_nominal(double beta, double mu)
{
    if (fabs(beta)<1E-12&&fabs(mu)<1E-12) return PI;
    return atan2(-tan(beta),sin(mu))+PI;
}
/* shadow-crossing GPS IIA ---------------------------------------------------*/
static int yaw_shadow_IIA(double beta, double mu, double eps0, double R,
                          double mudot, double *yaw)
{
    double mu_s,mu_e;
    
    mu_s=-sqrt(SQR(eps0)-SQR(beta));
    mu_e=-mu_s;
    
    if (mu_s<=mu&&mu<mu_e) {
        *yaw=atan2(-tan(beta),sin(mu_s))+R*(mu-mu_s)/mudot;
    }
    else if (mu_e<=mu&&mu<mu_e+T_POSTSHADOW*mudot) {
        return 0;
    }
    return 1;
}
/* shadow-crossing GLONASS-M -------------------------------------------------*/
static int yaw_shadow_GLO(double beta, double mu, double eps0, double R,
                          double mudot, double *yaw)
{
    double mu_s,mu_e,mu_f,tan_beta,sin_mu_s;
    
    if (beta<0) R=-R;
    tan_beta=tan(beta);
    
    mu_s=-acos(cos(eps0)/cos(beta));
    mu_e=-mu_s;
    sin_mu_s=sin(mu_s);
    mu_f=mudot*(atan2(-tan_beta,-sin_mu_s)-atan2(-tan_beta,sin_mu_s))/R+mu_s;
    
    if (mu_s<=mu&&mu<mu_f) {
        *yaw=atan2(-tan_beta,sin_mu_s)+R*(mu-mu_s)/mudot;
    }
    else if (mu_f<=mu&&mu<mu_e) {
        *yaw=atan2(-tan_beta,-sin_mu_s);
    }
    return 1;
}
/* noon-turn maneuver --------------------------------------------------------*/
static int yaw_noon(double beta, double mu, double beta0, double R,
                    double mudot, double *yaw)
{
    double mu_s,y;
    
    if (beta>=0) R=-R;
    mu_s=PI-sqrt(beta0*fabs(beta)-SQR(beta));
    
    if (mu_s<=mu) {
        y=atan2(-tan(beta),sin(mu_s))+R*(mu-mu_s)/mudot;
        if ((beta>=0&&y>*yaw)||(beta<0&&y<*yaw)) *yaw=y;
    }
    return 1;
}
/* midnight-turn maneuver ----------------------------------------------------*/
static int yaw_midnight(double beta, double mu, double beta0, double R,
                        double mudot, double *yaw)
{
    double mu_s,y;
    
    if (beta<0) R=-R;
    
    mu_s=-sqrt(beta0*fabs(beta)-SQR(beta));
    
    if (mu_s<=mu) {
        y=atan2(-tan(beta),sin(mu_s))+R*(mu-mu_s)/mudot;
        if ((beta>=0&&y<*yaw)||(beta<0&&y>*yaw)) *yaw=y;
    }
    return 1;
}
/* noon-turn maneuver for GLONASS-M ------------------------------------------*/
static int yaw_noon_GLO(double beta, double mu, double R, double mudot,
                        double *yaw)
{
    double mu_s,mu_e,sin_mu_s,B;
    int i;
    
    if (beta>=0) R=-R;
    
    for (mu_s=178.6*D2R,i=0;i<4;i++) {
        sin_mu_s=sin(mu_s);
        B=-beta*cos(mu_s)/(SQR(beta)+SQR(sin_mu_s));
        mu_s=(atan(-beta/sin_mu_s)+mu_s*B+PI*R/mudot-PI/2.0)/(R/mudot+B);
    }
    if (beta>=0) mu_s=2.0*PI-mu_s;
    mu_e=2.0*PI-mu_s;
    
    if (mu_s<=mu&&mu<mu_e) {
        *yaw=atan2(-tan(beta),sin(mu_s))+R*(mu-mu_s)/mudot;
    }
    return 1;
}
/* yaw-angle of GPS IIA (ref [8]) --------------------------------------------*/
static int yaw_IIA(int sat, int opt, double beta, double mu, double *yaw)
{
    const double R_GPSIIA[]={
        0.1046,0.1230,0.1255,0.1249,0.1003,0.1230,0.1136,0.1169,0.1253,0.0999,
        0.1230,0.1230,0.1230,0.1230,0.1092,0.1230,0.1230,0.1230,0.1230,0.1230,
        0.1230,0.1230,0.1230,0.0960,0.0838,0.1284,0.1183,0.1230,0.1024,0.1042,
        0.1230,0.1100,0.1230
    };
    double R=R_GPSIIA[sat-1]*D2R,beta0=atan(MUDOT_GPS/R);
    
    *yaw=atan2(-tan(beta),sin(mu));
    
    if (opt==2) { /* precise yaw */
        if (mu<PI/2.0&&fabs(beta)<EPS0_GPS) {
            if (!yaw_shadow_IIA(beta,mu,EPS0_GPS,R,MUDOT_GPS,yaw)) return 0;
        }
        else if (mu>PI/2.0&&fabs(beta)<beta0) {
            if (!yaw_noon(beta,mu,beta0,R,MUDOT_GPS,yaw)) return 0;
        }
    }
    return 1;
}
/* yaw-angle of GPS IIR (ref [8]) --------------------------------------------*/
static int yaw_IIR(int sat, int opt, double beta, double mu, double *yaw)
{
    const double R=0.2*D2R;
    double beta0=atan(MUDOT_GPS/R);
    
    *yaw=atan2(-tan(beta),sin(mu));
    
    if (opt==2) { /* precise yaw */
        if (mu<PI/2.0&&fabs(beta)<beta0) {
            if (!yaw_midnight(beta,mu,beta0,R,MUDOT_GPS,yaw)) return 0;
        }
        else if (mu>PI/2.0&&fabs(beta)<beta0) {
            if (!yaw_noon(beta,mu,beta0,R,MUDOT_GPS,yaw)) return 0;
        }
    }
    *yaw+=PI;
    return 1;
}
/* yaw-angle of GPS IIF (ref [9]) --------------------------------------------*/
static int yaw_IIF(int sat, int opt, double beta, double mu, double *yaw)
{
    const double R0=0.06*D2R,R1=0.11*D2R;
    double beta0=atan(MUDOT_GPS/R1);
    
    *yaw=atan2(-tan(beta),sin(mu));
    
    if (opt==2) { /* precise yaw */
        if (fabs(mu)<EPS0_GPS&&fabs(beta)<EPS0_GPS) {
            if (!yaw_shadow_GLO(beta,mu,EPS0_GPS,R0,MUDOT_GPS,yaw)) return 0;
        }
        else if (mu>PI/2.0&&fabs(beta)<beta0) {
            if (!yaw_noon(beta,mu,beta0,R1,MUDOT_GPS,yaw)) return 0;
        }
    }
    return 1;
}
/* yaw-angle of GLONASS-M (ref [10]) -----------------------------------------*/
static int yaw_GLO(int sat, int opt, double beta, double mu, double *yaw)
{
    const double eps1= 2.0*D2R,mu0=176.8*D2R;
    double R=0.25*D2R;
    
    *yaw=atan2(-tan(beta),sin(mu));
    
    if (opt==2) { /* precise yaw */
        if (fabs(mu)<EPS0_GLO&&fabs(beta)<EPS0_GLO) {
            if (!yaw_shadow_GLO(beta,mu,EPS0_GLO,R,MUDOT_GLO,yaw)) return 0;
        }
        else if (mu0<mu&&mu<2.0*PI-mu0&&fabs(beta)<eps1) {
            if (!yaw_noon_GLO(beta,mu,R,MUDOT_GLO,yaw)) return 0;
        }
    }
    return 1;
}
/* yaw-angle of Galileo (ref [11]) -------------------------------------------*/
static int yaw_GAL(int sat, int opt, double beta, double mu, double *yaw)
{
    *yaw=yaw_nominal(beta,mu);
    return 1;
}
/* yaw-angle of QZSS (ref [11]) ----------------------------------------------*/
static int yaw_QZS(int sat, int opt, double beta, double mu, double *yaw)
{
    if (abs(beta*R2D)<QZS_EC_BETA) *yaw=0.0;
    else *yaw=yaw_nominal(beta,mu);
    return 1;
}
/* yaw-angle of satellite ----------------------------------------------------*/
//extern int yaw_angle(int sat, const char *type, int opt, double beta, double mu,
//                     double *yaw)
//{
//    if      (strstr(type,"BLOCK IIA")) return yaw_IIA(sat,opt,beta,mu,yaw);
//    else if (strstr(type,"BLOCK IIR")) return yaw_IIR(sat,opt,beta,mu,yaw);
//    else if (strstr(type,"BLOCK IIF")) return yaw_IIF(sat,opt,beta,mu,yaw);
//    else if (strstr(type,"GLONASS"  )) return yaw_GLO(sat,opt,beta,mu,yaw);
//    else if (strstr(type,"Galileo"  )) return yaw_GAL(sat,opt,beta,mu,yaw);
//    else if (strstr(type,"QZSS"     )) return yaw_QZS(sat,opt,beta,mu,yaw);
//    return 0;
//}
extern int yaw_angle(int sat, const char *type, int opt, double beta, double mu,
	double *yaw)
{
	*yaw = yaw_nominal(beta, mu);
	return 1;
}
/* satellite attitude model --------------------------------------------------*/
static int sat_yaw(gtime_t time, int sat, const char *type, int opt,
                   const double *rs, double *exs, double *eys)
{
    double rsun[3],ri[6],es[3],esun[3],n[3],p[3],en[3],ep[3],ex[3],E,beta,mu;
    double yaw,cosy,siny,erpv[5]={0};
    int i;
    
    sunmoonpos(gpst2utc(time),erpv,rsun,NULL,NULL);
    
    /* beta and orbit angle */
    matcpy(ri,rs,6,1);
    ri[3]-=OMGE*ri[1];
    ri[4]+=OMGE*ri[0];
    cross3(ri,ri+3,n);
    cross3(rsun,n,p);
    if (!normv3(rs,es)||!normv3(rsun,esun)||!normv3(n,en)||
        !normv3(p,ep)) return 0;
    beta=PI/2.0-acos(dot(esun,en,3));
    E=acos(dot(es,ep,3));
    mu=PI/2.0+(dot(es,esun,3)<=0?-E:E);
    if      (mu<-PI/2.0) mu+=2.0*PI;
    else if (mu>=PI/2.0) mu-=2.0*PI;
    
    /* yaw-angle of satellite */
    if (!yaw_angle(sat,type,opt,beta,mu,&yaw)) return 0;
    
    /* satellite fixed x,y-vector */
    cross3(en,es,ex);
    cosy=cos(yaw);
    siny=sin(yaw);
    for (i=0;i<3;i++) {
        exs[i]=-siny*en[i]+cosy*ex[i];
        eys[i]=-cosy*en[i]-siny*ex[i];
    }
    return 1;
}
/* phase windup model --------------------------------------------------------*/
static int model_phw(gtime_t time, int sat, const char *type, int opt,
                     const double *rs, const double *rr, double *phw)
{
    double exs[3],eys[3],ek[3],exr[3],eyr[3],eks[3],ekr[3],E[9];
    double dr[3],ds[3],drs[3],r[3],pos[3],cosp,ph;
    int i;
    
    if (opt<=0) return 1; /* no phase windup */
    
    /* satellite yaw attitude model */
    if (!sat_yaw(time,sat,type,opt,rs,exs,eys)) return 0;
    
    /* unit vector satellite to receiver */
    for (i=0;i<3;i++) r[i]=rr[i]-rs[i];
    if (!normv3(r,ek)) return 0;
    
    /* unit vectors of receiver antenna */
    ecef2pos(rr,pos);
    xyz2enu(pos,E);
    exr[0]= E[1]; exr[1]= E[4]; exr[2]= E[7]; /* x = north */
    eyr[0]=-E[0]; eyr[1]=-E[3]; eyr[2]=-E[6]; /* y = west  */
    
    /* phase windup effect */
    cross3(ek,eys,eks);
    cross3(ek,eyr,ekr);
    for (i=0;i<3;i++) {
        ds[i]=exs[i]-ek[i]*dot(ek,exs,3)-eks[i];
        dr[i]=exr[i]-ek[i]*dot(ek,exr,3)+ekr[i];
    }
    cosp=dot(ds,dr,3)/norm(ds,3)/norm(dr,3);
    if      (cosp<-1.0) cosp=-1.0;
    else if (cosp> 1.0) cosp= 1.0;
    ph=acos(cosp)/2.0/PI;
    cross3(ds,dr,drs);
    if (dot(ek,drs,3)<0.0) ph=-ph;
    
    *phw=ph+floor(*phw-ph+0.5); /* in cycle */
    return 1;
}
/* measurement error variance ------------------------------------------------*/
static double varerr(int sat, int sys, double el, int freq, int type,
                     const prcopt_t *opt)
{
	double a = opt->err[1], b = opt->err[2];
	double c = 1.0, fact = 1.0;
	double sinel = sin(el);

	double elev = el;
	elev *= R2D;
	fact = EFACT_GPS;
	c = type ? opt->err[0] : 1.0;   /* type=0:phase,1:code */

	if (sys == SYS_GLO) {
		fact = EFACT_GLO;
		if (type) c = 100.0;
	}
	else if (sys == SYS_CMP) {
		if (type) c = 500.0;
	}
	else if (sys == SYS_GAL) {
		if (type) c = 100.0;
	}
	else if (sys == SYS_QZS) {
		if (type) c = 100.0;
	}

	if (opt->ionoopt == IONOOPT_IFLC) fact *= 3.0;
	double test = (SQR(a) + SQR(b / sinel));

	return SQR(fact*c)*(SQR(a) + SQR(b / sinel));
}
/* initialize state and covariance -------------------------------------------*/
static void initx(rtk_t *rtk, double xi, double var, int i)
{
    int j;
    rtk->x[i]=xi;
    for (j=0;j<rtk->nx;j++) {		
        rtk->P[i+j*rtk->nx]=rtk->P[j+i*rtk->nx]=i==j?var:0.0;
		//if(i==j)
		//printf("rtk->p[%d]=rtk->p[%d]=%lf\n", (i + j * rtk->nx), (j + i * rtk->nx), var);
    }
}
/* geometry-free phase measurement -------------------------------------------*/
static double gfmeas(const obsd_t *obs, const nav_t *nav)
{
    const double *lam=nav->lam[obs->sat-1];
    int i=(satsys(obs->sat,NULL)&(SYS_GAL|SYS_SBS))?2:1;
    
    if (lam[0]==0.0||lam[i]==0.0||obs->L[0]==0.0||obs->L[i]==0.0) return 0.0;
    return lam[0]*obs->L[0]-lam[i]*obs->L[i];
}


//From GLAB
/*****************************************************************************
* Name        : gravitationalDelayCorrection
* Description : Obtains the gravitational delay correction for the effect of
*               general relativity (red shift) to the GPS signal
* Parameters  :
* Name                           |Da|Unit|Description
* double  *receiverPosition       I  m    Position of the receiver
* double  *satellitePosition      I  m    Position of the satellite
* Returned value (double)         O  m    Gravitational delay correction
*****************************************************************************/
extern double gravitationalDelayCorrection(const int sys, const double *receiverPosition,
	const double *satellitePosition)
{
	double	receiverModule;
	double	satelliteModule;
	double	distance;
	double  MU = MU_GPS;

	receiverModule = sqrt(receiverPosition[0] * receiverPosition[0] + receiverPosition[1] * receiverPosition[1] +
		receiverPosition[2] * receiverPosition[2]);
	satelliteModule = sqrt(satellitePosition[0] * satellitePosition[0] + satellitePosition[1] * satellitePosition[1] +
		satellitePosition[2] * satellitePosition[2]);
	distance = sqrt((satellitePosition[0] - receiverPosition[0])*(satellitePosition[0] - receiverPosition[0]) +
		(satellitePosition[1] - receiverPosition[1])*(satellitePosition[1] - receiverPosition[1]) +
		(satellitePosition[2] - receiverPosition[2])*(satellitePosition[2] - receiverPosition[2]));

	switch (sys) {
	case SYS_GPS:
		MU = MU_GPS;
		break;
	case SYS_GLO:
		MU = MU_GLO;
		break;
	case SYS_GAL:
		MU = MU_GAL;
		break;
	case SYS_CMP:
		MU = MU_CMP;
		break;
	default:
		MU = MU_GPS;
		break;
	}

	return 2.0*MU / (CLIGHT*CLIGHT)*log((satelliteModule + receiverModule + distance) / (satelliteModule + receiverModule - distance));
}


/* Melbourne-Wubbena linear combination --------------------------------------*/
static double mwmeas(const obsd_t *obs, const nav_t *nav)
{
 //   const double *lam=nav->lam[obs->sat-1];
 //  // int i=(satsys(obs->sat,NULL)&(SYS_GAL|SYS_SBS))?2:1;
	//int i = 1;
 //   
 //   if (lam[0]==0.0||lam[i]==0.0||obs->L[0]==0.0||obs->L[i]==0.0||
 //       obs->P[0]==0.0||obs->P[i]==0.0) return 0.0;
 //   return lam[0]*lam[i]*(obs->L[0]-obs->L[i])/(lam[i]-lam[0])-
 //          (lam[i]*obs->P[0]+lam[0]*obs->P[i])/(lam[i]+lam[0]);
	int i = 0, j = 1;
	const double *lam = nav->lam[obs->sat - 1];
	double P1, P2, P1_C1, P2_C2, lam1, lam2, res;

	if (obs->L[i] == 0.0) return 0.0;
	if (obs->L[j] == 0.0) return 0.0;
	if (obs->P[i] == 0.0) return 0.0;
	if (obs->P[j] == 0.0) return 0.0;
	if (lam[i] * lam[j] == 0.0) return 0.0;

	P1 = obs->P[i];
	P2 = obs->P[j];
	P1_C1 = nav->cbias[obs->sat - 1][1];
	P2_C2 = nav->cbias[obs->sat - 1][2];

	if (obs->code[0] == CODE_L1C) 
		P1 += P1_C1; /* C1->P1 */
	if (obs->code[1] == CODE_L2C) 
		P2 += P2_C2; /* C2->P2 */

	lam1 = lam[i];
	lam2 = lam[j];
	res = (obs->L[i] - obs->L[j]) - (lam2 - lam1) / (lam1 + lam2)*(P1 / lam1 + P2 / lam2);
	return res;
}
/* antenna corrected measurements --------------------------------------------*/
static void corr_meas(const obsd_t *obs, const nav_t *nav, const double *azel,
                      const prcopt_t *opt, const double *dantr,
                      const double *dants, double phw, double *L, double *P,
                      double *Lc, double *Pc)
{
    const double *lam=nav->lam[obs->sat-1];
    double C1,C2;
    int i,sys;
    
    for (i=0;i<NFREQ;i++) {
        L[i]=P[i]=0.0;
        if (lam[i]==0.0||obs->L[i]==0.0||obs->P[i]==0.0) continue;
        if (testsnr(0,0,azel[1],obs->SNR[i]*0.25,&opt->snrmask)) continue;
        
        /* antenna phase center and phase windup correction */
        L[i]=obs->L[i]*lam[i]-dants[i]-dantr[i]-phw*lam[i];
        P[i]=obs->P[i]       -dants[i]-dantr[i];
        
        /* P1-C1,P2-C2 dcb correction (C1->P1,C2->P2) */
        if (obs->code[i]==CODE_L1C) {
            P[i]+=nav->cbias[obs->sat-1][1];
        }
        else if (obs->code[i]==CODE_L2C||obs->code[i]==CODE_L2X||
                 obs->code[i]==CODE_L2L||obs->code[i]==CODE_L2S) {
            P[i]+=nav->cbias[obs->sat-1][2];
#if 0
            L[i]-=0.25*lam[i]; /* 1/4 cycle-shift */
#endif
        }
    }
    /* iono-free LC */
    *Lc=*Pc=0.0;
    sys=satsys(obs->sat,NULL);
	i = (sys&(SYS_GAL | SYS_SBS | SYS_CMP)) ? 2 : 1; /* L1/L2 or L1/L5 */
   // i=(sys&(SYS_GAL|SYS_SBS))?2:1; /* L1/L2 or L1/L5 */
    if (lam[0]==0.0||lam[i]==0.0) return;
    
    C1= SQR(lam[i])/(SQR(lam[i])-SQR(lam[0]));
    C2=-SQR(lam[0])/(SQR(lam[i])-SQR(lam[0]));
    
#if 0
    /* P1-P2 dcb correction (P1->Pc,P2->Pc) */
    if (sys&(SYS_GPS|SYS_GLO|SYS_QZS)) {
        if (P[0]!=0.0) P[0]-=C2*nav->cbias[obs->sat-1][0];
        if (P[1]!=0.0) P[1]+=C1*nav->cbias[obs->sat-1][0];
    }
#endif
    if (L[0]!=0.0&&L[i]!=0.0) *Lc=C1*L[0]+C2*L[i];
    if (P[0]!=0.0&&P[i]!=0.0) *Pc=C1*P[0]+C2*P[i];
}
/* detect cycle slip by LLI --------------------------------------------------*/
static void detslp_ll(rtk_t *rtk, const obsd_t *obs, int n)
{
    int i,j;
    
    trace(3,"detslp_ll: n=%d\n",n);
    
    for (i=0;i<n&&i<MAXOBS;i++) for (j=0;j<rtk->opt.nf;j++) {
        if (obs[i].L[j]==0.0||!(obs[i].LLI[j]&3)) continue;
        
        //fprintf(outFp[OFILE_DEBUG], "detslp_ll: slip detected sat=%2d f=%d\n",obs[i].sat,j+1);
		printf("detslp_ll: slip detected sat=%2d f=%d\n", obs[i].sat, j + 1);
        rtk->ssat[obs[i].sat-1].slip[j]=1;
    }
}

static void detslp_mw(rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav)
{
	double w0, w1, elev, el, dtmp, thres, thres0;
	int i, j, sat;
	const double rad_20 = 20.0*D2R;
	trace(3, "detslp_mw: n=%d\n", n);
	int week;
	double sow, ep[6];
	char *sep = " ";
	int slips[MAXSAT][2] = {0};
	time2epoch(obs[0].time, ep);
	sow = time2gpst(obs[0].time, &week);

    fprintf(outFp[OFILE_MWSLIP], "%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s%04d%s%9.2f%s",
        (int)ep[0], sep, (int)ep[1], sep, (int)ep[2], sep, (int)ep[3], sep, (int)ep[4], sep, (int)ep[5], sep, week, sep, sow, sep);

	for (i = 0; i<n&&i<MAXOBS; i++) {
		if ((w1 = mwmeas(obs + i, nav)) == 0.0) continue;
		sat = obs[i].sat;
		elev = rtk->ssat[sat - 1].azel[1];
		el = elev;
		//
		if (elev<rtk->opt.elmin) continue;
		//
		dtmp = el * R2D;
		if (el >= rad_20) thres = THRES_MW_JUMP;
		else	 thres = -THRES_MW_JUMP * 0.1*dtmp + 3 * THRES_MW_JUMP;
		thres0 = MIN(thres*1.0, 6.0);
		w0 = rtk->ssat[obs[i].sat - 1].mw;
		rtk->ssat[obs[i].sat - 1].mw = w1;

		slips[sat - 1][0] = sat;
	
		//-----------------------------------------------------------------------------------------------
		if (w0 != 0.0&&fabs(w1 - w0)>thres0) {
			slips[sat - 1][1] = 1;
            fprintf(outFp[OFILE_DEBUG],"tow=%9.2f detslip_mw: slip detected sat=%2d mw=%8.3f->%8.3f\n",
                sow,obs[i].sat, w0, w1);
		    
			for (j = 0; j<rtk->opt.nf; j++) rtk->ssat[obs[i].sat - 1].slip[j] |= 1;
		}
	}

	for ( i = 0; i < MAXSAT; i++)
	{
		if ((i+1)== slips[i][0])
		{
            fprintf(outFp[OFILE_MWSLIP], "%d%s", slips[i][1],sep);
		}
		else
        {
            fprintf(outFp[OFILE_MWSLIP], "%d%s", 0,sep);
        }
	}
    fprintf(outFp[OFILE_MWSLIP], "\n");

}
/* detect cycle slip by geometry free phase jump -----------------------------*/
static void detslp_gf(rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav)
{
    double g0,g1,thres = 0.0, elev, el;
	double elev0, thres0;
    int i,j,sat;
    
    trace(3,"detslp_gf: n=%d\n",n);
	int week;
	double sow, ep[6];
	char *sep = " ";
	int slips[MAXSAT][2] = { 0 };
	time2epoch(obs[0].time, ep);
	sow = time2gpst(obs[0].time, &week);

    fprintf(outFp[OFILE_GFSLIP], "%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s%04d%s%9.2f%s",
        (int)ep[0], sep, (int)ep[1], sep, (int)ep[2], sep, (int)ep[3], sep, (int)ep[4], sep, (int)ep[5], sep, week, sep, sow, sep);

		for (i = 0; i<n&&i<MAXOBS; i++)
		{

			sat = obs[i].sat;
			elev = rtk->ssat[sat - 1].azel[1] * R2D;
			elev0 = elev;

			g1 = gfmeas(obs + i, nav);

			if (g1 == 0.0) continue;

			if (elev < rtk->opt.elmin*R2D) continue;//elev = rtk->opt.elmin*R2D;


			if (elev >= 15.0) thres = THRES_GF_JUMP;
			else thres = -THRES_GF_JUMP / 15.0*elev + 2 * THRES_GF_JUMP;

			g0 = rtk->ssat[sat - 1].gf;
			thres0 = MIN(thres*1.0, 1.5);

			slips[sat - 1][0] = sat;
			if (g0 != 0.0&&fabs(g1 - g0)>MIN(thres*1.0, 1.5))
			{
				slips[sat - 1][1] = 1;
                fprintf(outFp[OFILE_DEBUG],"tow=%9.2f detslip_gf: slip detected sat=%2d gf=%8.3f->%8.3f\n", sow,obs[i].sat, g0, g1);
				for (j = 0; j<rtk->opt.nf; j++)
					rtk->ssat[sat - 1].slip[j] |= 1;

			}
		}
		for (i = 0; i < MAXSAT; i++)
		{
			if ((i + 1) == slips[i][0])
			{
                fprintf(outFp[OFILE_GFSLIP], "%d%s", slips[i][1], sep);
			}
			else
            {
                fprintf(outFp[OFILE_GFSLIP], "%d%s", 0, sep);
            }
		}
        fprintf(outFp[OFILE_GFSLIP], "\n");
}
	


/* temporal update of position -----------------------------------------------*/
static void udpos_ppp(rtk_t *rtk)
{
    int i;
    
    trace(3,"udpos_ppp:\n");
    
    /* fixed mode */
    if (rtk->opt.mode==PMODE_PPP_FIXED) {
        for (i=0;i<3;i++) initx(rtk,rtk->opt.ru[i],1E-8,i);
        return;
    }
    /* initialize position for first epoch */
    if (norm(rtk->x,3)<=0.0) {
        for (i=0;i<3;i++) 
			initx(rtk,rtk->sol.rr[i],VAR_POS,i);
    }
    /* static ppp mode */
    if (rtk->opt.mode==PMODE_PPP_STATIC) {
        for (i=0;i<3;i++) {
            rtk->P[i*(1+rtk->nx)]+=SQR(rtk->opt.prn[5])*fabs(rtk->tt);
        }
        return;
    }
    /* kinmatic mode without dynamics */
    for (i=0;i<3;i++) {
        initx(rtk,rtk->sol.rr[i],VAR_POS,i);
    }
}
/* temporal update of clock --------------------------------------------------*/
//static void udclk_ppp(rtk_t *rtk)
//{
//    double dtr;
//    int i;

//    trace(3,"udclk_ppp:\n");

//    /* initialize every epoch for clock (white noise) */
//    for (i=0;i<NSYS;i++) {
//        if (rtk->opt.sateph==EPHOPT_PREC) {
//            /* time of prec ephemeris is based gpst */
//            /* negelect receiver inter-system bias  */
//            dtr=rtk->sol.dtr[0];
//        }
//        else {
//            dtr=i==0?rtk->sol.dtr[0]:rtk->sol.dtr[0]+rtk->sol.dtr[i];
//        }
//        initx(rtk,CLIGHT*dtr,VAR_CLK,IC(i,&rtk->opt));
//    }
//}

static void udclk_ppp(rtk_t *rtk)
{
    prcopt_t *opt = &rtk->opt;
    double dtr, tt = fabs(rtk->tt);
    int i,jj, ic, sys = rtk->opt.navsys;
    /* initialize every epoch for clock (white noise) */
    dtr = rtk->sol.dtr[0];
    if (fabs(dtr)<1.0e-16) dtr = 1.0e-16;
    ic = IC(0, opt);
    initx(rtk, CLIGHT*dtr, VAR_CLK, ic);
    int ttt = NSYS;
    for (i = 1; i < NSYS; i++)
    {
        if (sys&SYS_CMP)
            jj = 1;
        if (sys&SYS_GLO)
            jj = 2;
        if (sys&SYS_GAL)
            jj = 3;
        if (sys&SYS_QZS)
            jj == 4;
        dtr = rtk->sol.dtr[jj];
        ic = IC(jj, opt);

        if (opt->gnsisb == GNSISB_CT) {
            // constant
            if (rtk->x[ic] == 0.0) {
                if (fabs(dtr) < 1.0e-16)
                    dtr = 1.0e-16;
                initx(rtk, CLIGHT*dtr, VAR_CLK, ic);
            }
        }

        else if (opt->gnsisb == GNSISB_RW) {
            // random walk process
            if (rtk->x[ic] == 0.0) {
                if (fabs(dtr) < 1.0e-16) dtr = 1.0e-16;
                initx(rtk, CLIGHT*dtr, VAR_CLK, ic);
            }
            else {
                rtk->P[ic + ic * rtk->nx] += SQR(0.001)*tt;
            }
        }
        else if (opt->gnsisb == GNSISB_WN) {
            //white noise process
            if (fabs(dtr) < 1.0e-16) dtr = 1.0e-16;
            initx(rtk, CLIGHT*dtr, VAR_CLK, ic);
        }
    }
}




/* temporal update of tropospheric parameters --------------------------------*/
//static void udtrop_ppp(rtk_t *rtk)
//{
//    double pos[3],azel[]={0.0,PI/2.0},ztd,var;
//    int i=IT(&rtk->opt),j;
//
//    trace(3,"udtrop_ppp:\n");
//    
//    if (rtk->x[i]==0.0) {
//        ecef2pos(rtk->sol.rr,pos);
//        ztd=sbstropcorr(rtk->sol.time,pos,azel,&var);
//        initx(rtk,ztd,var,i);
//        
//        if (rtk->opt.tropopt>=TROPOPT_ESTG) {
//            for (j=i+1;j<i+3;j++) initx(rtk,1E-6,VAR_GRA,j);
//        }
//    }
//    else {
//        rtk->P[i+i*rtk->nx]+=SQR(rtk->opt.prn[2])*fabs(rtk->tt);
//        
//        if (rtk->opt.tropopt>=TROPOPT_ESTG) {
//            for (j=i+1;j<i+3;j++) {
//                rtk->P[j+j*rtk->nx]+=SQR(rtk->opt.prn[2]*0.1)*fabs(rtk->tt);
//            }
//        }
//    }
//}
/* temporal update of tropospheric parameters --------------------------------*/
static void udtrop_ppp(rtk_t *rtk)
{
	double var, tt = fabs(rtk->tt);
	int i = IT(&rtk->opt), j;

	if (rtk->x[i] == 0.0) {
		var = SQR(0.3);
		initx(rtk, 0.15, var, i);

		if (rtk->opt.tropopt >= TROPOPT_ESTG) {
			for (j = i + 1; j<i + 3; j++) initx(rtk, 1E-6, VAR_GRA, j);
		}
	}
	else {
		rtk->P[i + i * rtk->nx] += SQR(rtk->opt.prn[2])*tt;

		if (rtk->opt.tropopt >= TROPOPT_ESTG) {
			for (j = i + 1; j<i + 3; j++) {
				rtk->P[j + j * rtk->nx] += SQR(rtk->opt.prn[2] * 0.1)*tt;
			}
		}
	}
}

/* temporal update of ionospheric parameters ---------------------------------*/
static void udiono_ppp(rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav)
{
    const double *lam;
    double ion,sinel,pos[3],*azel;
    char *p;
    int i,j,k,gap_resion=GAP_RESION;
    
    trace(3,"udiono_ppp:\n");
    
    if ((p=strstr(rtk->opt.pppopt,"-GAP_RESION="))) {
        sscanf(p,"-GAP_RESION=%d",&gap_resion);
    }
    for (i=0;i<MAXSAT;i++) {
        j=II(i+1,&rtk->opt);
        if (rtk->x[j]!=0.0&&(int)rtk->ssat[i].outc[0]>gap_resion) {
            rtk->x[j]=0.0;
        }
    }
    for (i=0;i<n;i++) {
        j=II(obs[i].sat,&rtk->opt);
        if (rtk->x[j]==0.0) {
            k=satsys(obs[i].sat,NULL)==SYS_GAL?2:1;
            lam=nav->lam[obs[i].sat-1];
            if (obs[i].P[0]==0.0||obs[i].P[k]==0.0||lam[0]==0.0||lam[k]==0.0) {
                continue;
            }
            ion=(obs[i].P[0]-obs[i].P[k])/(1.0-SQR(lam[k]/lam[0]));
            ecef2pos(rtk->sol.rr,pos);
            azel=rtk->ssat[obs[i].sat-1].azel;
            ion/=ionmapf(pos,azel);
            initx(rtk,ion,VAR_IONO,j);
        }
        else {
            sinel=sin(MAX(rtk->ssat[obs[i].sat-1].azel[1],5.0*D2R));
            rtk->P[j+j*rtk->nx]+=SQR(rtk->opt.prn[1]/sinel)*fabs(rtk->tt);
        }
    }
}
/* temporal update of L5-receiver-dcb parameters -----------------------------*/
static void uddcb_ppp(rtk_t *rtk)
{
    int i=ID(&rtk->opt);
    
    trace(3,"uddcb_ppp:\n");
    
    if (rtk->x[i]==0.0) {
        initx(rtk,1E-6,VAR_DCB,i);
    }
}
/* temporal update of phase biases -------------------------------------------*/
static void udbias_ppp(rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav)
{
    const double *lam;
    double L[NFREQ],P[NFREQ],Lc,Pc,bias[MAXOBS],offset=0.0,pos[3]={0};
    double ion,dantr[NFREQ]={0},dants[NFREQ]={0};
    int i,j,k,l,f,sat,slip[MAXOBS]={0},clk_jump=0;
    
    trace(3,"udbias  : n=%d\n",n);
    
    /* handle day-boundary clock jump */
    if (rtk->opt.posopt[5]) {
        clk_jump=ROUND(time2gpst(obs[0].time,NULL)*10)%864000==0;
    }
    for (i=0;i<MAXSAT;i++) for (j=0;j<rtk->opt.nf;j++) {
        rtk->ssat[i].slip[j]=0;
    }
    /* detect cycle slip by LLI */
    //detslp_ll(rtk,obs,n);
    
	/* detect slip by Melbourne-Wubbena linear combination jump */
	detslp_mw(rtk, obs, n, nav);

    /* detect cycle slip by geometry-free phase jump */
    detslp_gf(rtk,obs,n,nav);
     
    ecef2pos(rtk->sol.rr,pos);
    
    for (f=0;f<NF(&rtk->opt);f++) {
        
        /* reset phase-bias if expire obs outage counter */
        for (i=0;i<MAXSAT;i++) {
            if (++rtk->ssat[i].outc[f]>(unsigned int)rtk->opt.maxout||
                rtk->opt.modear==ARMODE_INST||clk_jump) {
                initx(rtk,0.0,0.0,IB(i+1,f,&rtk->opt));
            }
        }
        for (i=k=0;i<n&&i<MAXOBS;i++) {
            sat=obs[i].sat;
            j=IB(sat,f,&rtk->opt);
            corr_meas(obs+i,nav,rtk->ssat[sat-1].azel,&rtk->opt,dantr,dants,
                      0.0,L,P,&Lc,&Pc);
            
            bias[i]=0.0;
            
            if (rtk->opt.ionoopt==IONOOPT_IFLC) {
                bias[i]=Lc-Pc;
                slip[i]=rtk->ssat[sat-1].slip[0]||rtk->ssat[sat-1].slip[1];
            }
            else if (L[f]!=0.0&&P[f]!=0.0) {
                slip[i]=rtk->ssat[sat-1].slip[f];
                l=satsys(sat,NULL)==SYS_GAL?2:1;
                lam=nav->lam[sat-1];
                if (obs[i].P[0]==0.0||obs[i].P[l]==0.0||
                    lam[0]==0.0||lam[l]==0.0||lam[f]==0.0) continue;
                ion=(obs[i].P[0]-obs[i].P[l])/(1.0-SQR(lam[l]/lam[0]));
                bias[i]=L[f]-P[f]+2.0*ion*SQR(lam[f]/lam[0]);
            }
            if (rtk->x[j]==0.0||slip[i]||bias[i]==0.0) continue;
            
            offset+=bias[i]-rtk->x[j];
            k++;
        }
        /* correct phase-code jump to ensure phase-code coherency */
        if (k>=2&&fabs(offset/k)>0.0005*CLIGHT) {
            for (i=0;i<MAXSAT;i++) {
                j=IB(i+1,f,&rtk->opt);
                if (rtk->x[j]!=0.0) rtk->x[j]+=offset/k;
            }
            trace(2,"phase-code jump corrected: %s n=%2d dt=%12.9fs\n",
                  time_str(rtk->sol.time,0),k,offset/k/CLIGHT);
        }
        for (i=0;i<n&&i<MAXOBS;i++) {
            sat=obs[i].sat;
            j=IB(sat,f,&rtk->opt);
            
            rtk->P[j+j*rtk->nx]+=SQR(rtk->opt.prn[0])*fabs(rtk->tt);
            
            if (bias[i]==0.0||(rtk->x[j]!=0.0&&!slip[i])) continue;
            
            /* reinitialize phase-bias if detecting cycle slip */
            initx(rtk,bias[i],VAR_BIAS,IB(sat,f,&rtk->opt));
            
            /* reset fix flags */
            for (k=0;k<MAXSAT;k++) rtk->ambc[sat-1].flags[k]=0;
            
            trace(5,"udbias_ppp: sat=%2d bias=%.3f\n",sat,bias[i]);
        }
    }
}
/* temporal update of states --------------------------------------------------*/
static void udstate_ppp(rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav)
{
    trace(3,"udstate_ppp: n=%d\n",n);
    
    /* temporal update of position */
    udpos_ppp(rtk);
    
    /* temporal update of clock */
    udclk_ppp(rtk);
    
    /* temporal update of tropospheric parameters */
    if (rtk->opt.tropopt==TROPOPT_EST||rtk->opt.tropopt==TROPOPT_ESTG) {
        udtrop_ppp(rtk);
    }
    /* temporal update of ionospheric parameters */
    if (rtk->opt.ionoopt==IONOOPT_EST) {
        udiono_ppp(rtk,obs,n,nav);
    }
    /* temporal update of L5-receiver-dcb parameters */
    if (rtk->opt.nf>=3) {
        uddcb_ppp(rtk);
    }
    /* temporal update of phase-bias */
    udbias_ppp(rtk,obs,n,nav);

}
/* satellite antenna phase center variation ----------------------------------*/
static void satantpcv(int sat, const double *rs, const double *rr, const pcv_t *pcv,
	double *dant)
{
	double ru[3], rz[3], eu[3], ez[3], nadir, cosa;
	int i;

	for (i = 0; i<3; i++) {
		ru[i] = rr[i] - rs[i];
		rz[i] = -rs[i];
	}
	if (!normv3(ru, eu) || !normv3(rz, ez)) return;

	cosa = dot(eu, ez, 3);
	cosa = cosa<-1.0 ? -1.0 : (cosa>1.0 ? 1.0 : cosa);
	nadir = acos(cosa);

	antmodel_s(sat, pcv, nadir, dant);
}
/* precise tropospheric model ------------------------------------------------*/
static double trop_model_prec(gtime_t time, const double *pos, const double *azel,
	const prcopt_t *opt, const double *x, double *dtdx, double *shd, double *var)
{
	const double zazel[] = { 0.0,PI / 2.0 };
	double zhd, zwd, m_h, m_w, cotz, grad_n, grad_e;
    
    /* zenith hydrostatic delay */
	zhd = tropmodel(time, pos, zazel, 0.0, &zwd, 1);//天顶干分量延迟 湿分量为0
    
    /* mapping function */
    m_h=tropmapf(time,pos,azel,&m_w);//m_h干分量系数  m_w湿分量系数 
	*shd = m_h * zhd;
	if (opt->tropopt >= TROPOPT_ESTG && azel[1]>0.0) {
		/* m_w=m_0+m_0*cot(el)*(Gn*cos(az)+Ge*sin(az)): ref [6] */
		cotz = 1.0 / tan(azel[1]);
		grad_n = m_w * cotz*cos(azel[0]);
		grad_e = m_w * cotz*sin(azel[0]);
		m_w += grad_n * x[1] + grad_e * x[2];
		dtdx[1] = grad_n * (x[0]);
		dtdx[2] = grad_e * (x[0]);
	}
    dtdx[0]=m_w;
    *var=SQR(0.01);
    //return m_h*zhd+m_w*(x[0]-zhd);
	return m_h * zhd + m_w * (x[0]);
}
/* tropospheric model ---------------------------------------------------------*/
static int model_trop(gtime_t time, const double *pos, const double *azel,
	const prcopt_t *opt, const double *x, double *dtdx,
	const nav_t *nav, double *dtrp, double *shd, double *var)
{
	double trp[3] = { 0 };
	double zwd;
    
    if (opt->tropopt==TROPOPT_SAAS) {
		*dtrp = tropmodel(time, pos, azel, REL_HUMI, &zwd, 0);
        *var=SQR(ERR_SAAS);
        return 1;
    }
   /* if (opt->tropopt==TROPOPT_SBAS) {
        *dtrp=sbstropcorr(time,pos,azel,var);
        return 1;
    }*/
	
    if (opt->tropopt==TROPOPT_EST||opt->tropopt==TROPOPT_ESTG) {
        matcpy(trp,x+IT(opt),opt->tropopt==TROPOPT_EST?1:3,1);//IT(opt) 如果是运动模式 返回9+4（分别为位置 速度 加速度+4（四个导航系统种差）） 否则返回3+4//x+7后面 拷贝 1*1内存到trp
		*dtrp = trop_model_prec(time, pos, azel, opt, trp, dtdx, shd, var);
        return 1;
    }
   /* if (opt->tropopt==TROPOPT_ZTD) {
        if (pppcorr_trop(&nav->pppcorr,time,pos,trp,std)) {
            *dtrp=trop_model_prec(time,pos,azel,trp,dtdx,var);
            *var=SQR(dtdx[0]*std[0]);
            return 1;
        }
    }*/
    return 0;
}
/* ionospheric model ---------------------------------------------------------*/
static int model_iono(gtime_t time, const double *pos, const double *azel,
                      const prcopt_t *opt, int sat, const double *x,
                      const nav_t *nav, double *dion, double *var)
{
    static double iono_p[MAXSAT]={0},std_p[MAXSAT]={0};
    static gtime_t time_p;
    
 /*   if (opt->ionoopt==IONOOPT_SBAS) {
        return sbsioncorr(time,nav,pos,azel,dion,var);
    }*/
    /*if (opt->ionoopt==IONOOPT_TEC) {
        return iontec(time,nav,pos,azel,1,dion,var);
    }*/
    if (opt->ionoopt==IONOOPT_BRDC) {
        *dion=ionmodel(time,nav->ion_gps,pos,azel);
        *var=SQR(*dion*ERR_BRDCI);
        return 1;
    }
    if (opt->ionoopt==IONOOPT_EST) {
        *dion=x[II(sat,opt)];
        *var=0.0;
        return 1;
    }
    if (opt->ionoopt==IONOOPT_IFLC) {
        *dion=*var=0.0;
        return 1;
    }
   /* if (opt->ionoopt==IONOOPT_STEC) {
        if (timediff(time,time_p)!=0.0&&
            !pppcorr_stec(&nav->pppcorr,time,pos,iono_p,std_p)) return 0;
        if (iono_p[sat-1]==0.0||std_p[sat-1]>0.1) return 0;
        time_p=time;
        *dion=iono_p[sat-1];
        *var=SQR(std_p[sat-1]);
        return 1;
    }*/
    return 0;
}
/* constraint to local correction --------------------------------------------*/
//static int const_corr(const obsd_t *obs, int n, const int *exc,
//                      const nav_t *nav, const double *x, const double *pos,
//                      const double *azel, rtk_t *rtk, double *v, double *H,
//                      double *var)
//{
//    gtime_t time=obs[0].time;
//    double trop[3],std_trop[3],iono[MAXSAT],std_iono[MAXSAT];
//    int i,j,k,sat,nv=0;
//    
//    /* constraint to external troposphere correction */
//    if ((rtk->opt.tropopt==TROPOPT_EST||rtk->opt.tropopt==TROPOPT_ESTG)&&
//        pppcorr_trop(&nav->pppcorr,time,pos,trop,std_trop)) {
//        
//        for (i=0;i<(rtk->opt.tropopt==TROPOPT_EST?1:3);i++) {
//            if (std_trop[i]==0.0) continue;
//            j=IT(&rtk->opt)+i;
//            v[nv]=trop[i]-x[j];
//            for (k=0;k<rtk->nx;k++) H[k+nv*rtk->nx]=k==j?1.0:0.0;
//            var[nv++]=SQR(std_trop[i]);
//        }
//    }
 //   /* constraint to external ionosphere correction */
 //   if (rtk->opt.ionoopt==IONOOPT_EST&&
 //       pppcorr_stec(&nav->pppcorr,time,pos,iono,std_iono)) 
	//{
 //       
 //       for (i=0;i<n;i++) {
 //           sat=obs[i].sat;
 //           if (exc[i]||iono[sat-1]==0.0||std_iono[sat-1]>0.5) continue;
 //           j=II(sat,&rtk->opt);
 //           v[nv]=iono[sat-1]-x[j];
 //           for (k=0;k<rtk->nx;k++) H[k+nv*rtk->nx]=k==j?1.0:0.0;
 //           var[nv++]=SQR(std_iono[sat-1]);
 //       }
 //   }
    //return nv;
//}
/* phase and code residuals --------------------------------------------------*/
#if 0
static int ppp_res(int post, const obsd_t *obs, int n, const double *rs,
	const double *dts, const double *var_rs, const int *svh,
	const double *dr, int *exc, const nav_t *nav,
	double *x,double *Pp, rtk_t *rtk, double *v, double *H, double *R,
	double *azel, double *P_, double *H_, int *np, int *robust,double *var)
{
	const double *lam;
	prcopt_t *opt = &rtk->opt;
	double y, r, cdtr, bias, C, rr[3], pos[3], e[3], dtdx[3], L[NFREQ], P[NFREQ], Lc, Pc;
	double dtrp = 0.0, dion = 0.0, vart = 0.0, vari = 0.0, dcb;
	double dantr[NFREQ] = { 0 }, dants[NFREQ] = { 0 };
	double ve[MAXOBS * 2 * NFREQ] = { 0 }, vmax = 0, gravitationalDelayModel, shd = 0.0;
	char str[32];
	int ne = 0, obsi[MAXOBS * 2 * NFREQ] = { 0 }, frqi[MAXOBS * 2 * NFREQ], maxobs, maxfrq, rej;
	int i, j, k, sat, sys, nv = 0, nx = rtk->nx, stat = 1;
	int prnn; double pp_; double pp;
	int sat_use[MAXOBS] = { 0 };
	int obs_use[MAXOBS] = { 0 };
	time2str(obs[0].time, str, 2);

	for (i = 0; i<MAXSAT; i++) for (j = 0; j<opt->nf; j++) rtk->ssat[i].vsat[j] = 0;

	for (i = 0; i<3; i++) rr[i] = x[i] + dr[i];
	ecef2pos(rr, pos);
	int sow, week;
	int tss;
	int jj = 0;
	int kk=0;
	sow = time2gpst(obs[0].time, &week);
	if (sow == 352098 && post == 2)
		tss = 1;
		//exit(1);
	for (i = 0; i<n&&i<MAXOBS; i++) {
		sat = obs[i].sat;
		lam = nav->lam[sat - 1];
		if (lam[j / 2] == 0.0 || lam[0] == 0.0) {
			jj = IB(sat, 0, &rtk->opt);
			Pp[jj + jj * rtk->nx] = 0.0;
			x[jj] = 0.0; 
			continue;
		}

		if ((r = geodist(rs + i * 6, rr, e)) <= 0.0 ||
			satazel(pos, e, azel + i * 2)<opt->elmin) {
			exc[i] = 1;
			jj = IB(sat, 0, &rtk->opt);
			Pp[jj + jj * rtk->nx] = 0.0;
			x[jj] = 0.0;
			continue;
		}

		//****************************************************
		//---------------record sat position and azel date----
		for (kk = 0; kk < 3; kk++) {
			rtk->ssat[sat - 1].satpos[kk] = (rs + i * 6)[kk];
			rtk->ssat[sat - 1].azel[kk] = (azel + i * 2)[kk];
			rtk->ssat[sat - 1].satclk = dts[i * 2] * CLIGHT;
		}

		double cosaz, sinaz, cosel, sinel, enu[3] = { 0 };
		//line-of-sight (LOS) unit vector
		cosel = cos(rtk->ssat[sat - 1].azel[1]); sinel = sin(rtk->ssat[sat - 1].azel[1]);
		cosaz = cos(rtk->ssat[sat - 1].azel[0]); sinaz = sin(rtk->ssat[sat - 1].azel[0]);

		//convert 3D tidal displacements to LOS range
		ecef2enu(pos, dr, enu);
		rtk->ssat[sat - 1].dtid = enu[1] * cosel*cosaz + enu[0] * cosel*sinaz + enu[2] * sinel;
		//****************************************************
		//------------------------------------------------------

		if (!(sys = satsys(sat, NULL)) || !rtk->ssat[sat - 1].vs ||satexclude(obs[i].sat, svh[i], opt) || exc[i]) {
			exc[i] = 1;
			jj = IB(sat, 0, &rtk->opt);
			Pp[jj + jj * rtk->nx] = 0.0;
			x[jj] = 0.0;
			continue;
		}
		/* tropospheric and ionospheric model */
		if (!model_trop(obs[i].time, pos, azel + i * 2, opt, x, dtdx, nav, &dtrp, &shd, &vart) ||
			!model_iono(obs[i].time, pos, azel + i * 2, opt, sat, x, nav, &dion, &vari)) {
			jj = IB(sat, 0, &rtk->opt);
			Pp[jj + jj * rtk->nx] = 0.0;
			x[jj] = 0.0;
			continue;
		}
		//****************************************************
		//---------------record tropospheric date--------------
		rtk->ssat[sat - 1].dsag = sagnac(rs + i * 6, rr);
		rtk->ssat[sat - 1].dtrp = dtrp;     //对流层总延迟
		rtk->ssat[sat - 1].shd = shd;      //对流层延迟干分量=干分量投影函数*ztd
		rtk->ssat[sat - 1].wmap = dtdx[0]; //湿分量投影函数
		//****************************************************
		//------------------------------------------------------

		/* satellite and receiver antenna model */
		if (opt->posopt[0])
			satantpcv(sat, rs + i * 6, rr, nav->pcvs + sat - 1, dants);

		//printf("sat=%3d dants=%10.7lf %10.7lf\t %10.7lf\n", sat, dants[0], dants[1], dants[2]);

		antmodel(sat, opt->pcvr, opt->antdel[0], azel + i * 2, opt->posopt[1], dantr);

		/* phase windup model */
		if (!model_phw(rtk->sol.time, sat, nav->pcvs[sat - 1].type,
			opt->posopt[2] ? 2 : 0, rs + i * 6, rr, &rtk->ssat[sat - 1].phw)) {
			jj = IB(sat, 0, &rtk->opt);
			Pp[jj + jj * rtk->nx] = 0.0;
			x[jj] = 0.0;
			continue;
		}
		//****************************************************
		//---------------record dant date--------------
		for (j = 0; j < NFREQ; j++) rtk->ssat[sat - 1].dant[j] = dants[j];

		//****************************************************
		//------------------------------------------------------

		/* corrected phase and code measurements */
		corr_meas(obs + i, nav, azel + i * 2, &rtk->opt, dantr, dants,
			rtk->ssat[sat - 1].phw, L, P, &Lc, &Pc);
		//****************************************************
		//---------------record obs date----------------------
		for (kk = 0; kk < NFREQ; kk++)
		{
			rtk->ssat[sat - 1].P[kk] = obs[i].P[kk];
			rtk->ssat[sat - 1].L[kk] = obs[i].L[kk];
		}
		//-----------------------------------------------------
		//****************************************************

		gravitationalDelayModel = gravitationalDelayCorrection(sys, rr, rs + i * 6);
		/* stack phase and code residuals {L1,P1,L2,P2,...} */
		for (j = 0; j<2 * NF(opt); j++) {

			dcb = bias = 0.0;

			if (opt->ionoopt == IONOOPT_IFLC) {
				if ((y = j % 2 == 0 ? Lc : Pc) == 0.0) { 
					jj = IB(sat, 0, &rtk->opt);
					Pp[jj + jj * rtk->nx] = 0.0;
					x[jj] = 0.0;
					continue; 
				}
			}
			else {
				if ((y = j % 2 == 0 ? L[j / 2] : P[j / 2]) == 0.0)
				{
					jj = IB(sat, 0, &rtk->opt);
					Pp[jj + jj * rtk->nx] = 0.0;
					x[jj] = 0.0;
					continue;
				}
				/* receiver DCB correction for P2 */
				if (j / 2 == 1) dcb = -nav->rbias[0][sys == SYS_GLO ? 1 : 0][0];
			}
			C = SQR(lam[j / 2] / lam[0])*ionmapf(pos, azel + i * 2)*(j % 2 == 0 ? -1.0 : 1.0);

			for (k = 0; k < nx; k++) 	H[k + nx * nv] = k < 3 ? -e[k] : 0.0;


			/* receiver clock */
			//k = sys == SYS_GLO ? 1 : (sys == SYS_GAL ? 2 : (sys == SYS_CMP ? 3 : 0));
			k = sys == SYS_CMP ? 1 : (sys == SYS_GLO ? 2 : (sys == SYS_GAL ? 3 : 0));
			cdtr = x[IC(k, opt)];
			H[IC(k, opt) + nx * nv] = 1.0;
			if (opt->tropopt == TROPOPT_EST || opt->tropopt == TROPOPT_ESTG) {
				for (k = 0; k<(opt->tropopt >= TROPOPT_ESTG ? 3 : 1); k++) {
					H[IT(opt) + k + nx * nv] = dtdx[k];
				}
			}
			if (opt->ionoopt == IONOOPT_EST) {
				if (rtk->x[II(sat, opt)] == 0.0)
				{
					jj = IB(sat, 0, &rtk->opt);
					Pp[jj + jj * rtk->nx] = 0.0;
					x[jj] = 0.0;
					continue;
				}
				H[II(sat, opt) + nx * nv] = C;
			}
			if (j / 2 == 2 && j % 2 == 1) { /* L5-receiver-dcb */

				dcb += rtk->x[ID(opt)];
				H[ID(opt) + nx * nv] = 1.0;
			}
			if (j % 2 == 0) { /* phase bias */
				if ((bias = x[IB(sat, j / 2, opt)]) == 0.0)
				{
					jj = IB(sat, 0, &rtk->opt);
					Pp[jj + jj * rtk->nx] = 0.0;
					x[jj] = 0.0;
					continue;
				}
				H[IB(sat, j / 2, opt) + nx * nv] = 1.0;
			}
			/* residual */
			v[nv] = y - (r + cdtr - CLIGHT * dts[i * 2] + dtrp + C * dion + dcb + bias - gravitationalDelayModel);
			sat_use[nv] = sat;
			obs_use[nv] = i;

			if (j % 2 == 0) rtk->ssat[sat - 1].resc[j / 2] = v[nv];
			else        rtk->ssat[sat - 1].resp[j / 2] = v[nv];

			if (post == 0 && (robust[i] != sat)) {
				/* variance */
				var[nv] = varerr(obs[i].sat, sys, azel[1 + i * 2], j / 2, j % 2, opt) + vart + SQR(C)*vari + var_rs[i];
				if (sys == SYS_GLO && j % 2 == 1) var[nv] += VAR_GLO_IFB;
			}
			
			trace(4, "%s sat=%2d %s%d res=%9.4f sig=%9.4f el=%4.1f\n", str, sat,
				j % 2 ? "P" : "L", j / 2 + 1, v[nv], sqrt(var[nv]), azel[1 + i * 2] * R2D);

			/* reject satellite by pre-fit residuals */
			if (!post&&opt->maxinno>0.0&&fabs(v[nv])>opt->maxinno) {
                //fprintf(outFp[OFILE_DEBUG], "outlier (%d) rejected %s sat=%2d %s%d res=%9.4f el=%4.1f\n",
                    //post, str, sat, j % 2 ? "P" : "L", j / 2 + 1, v[nv], azel[1 + i * 2] * R2D);
				//printf("outlier (%d) rejected %s sat=%2d %s%d res=%9.4f el=%4.1f\n",
					//post, str, sat, j % 2 ? "P" : "L", j / 2 + 1, v[nv], azel[1 + i * 2] * R2D);
                    //exc[i] = 1; rtk->ssat[sat - 1].rejc[j % 2]++;
				//rtk->ssat[sat - 1].rejc[j % 2]++;

				jj = IB(sat, 0, &rtk->opt);
				Pp[jj + jj * rtk->nx] = 0.0;
				x[jj] = 0.0;
				continue;
			}

			if (j % 2 == 0) rtk->ssat[sat - 1].vsat[j / 2] = 1;
			nv++;
		}
	}

	//------------------------------20180627------------------------------
	//------------------------------抗差因子------------------------------
	//--------------------------------------------------------------------
	//R-HPH'

	if (post) {
		double  *RR = mat(nv, 1);
		double *F = mat(*np, nv), *Q = mat(nv, nv);
		double  *sig = mat(nv, 1);

		double *VR = mat(1, nv);
		double *VRV = (double*)malloc(sizeof(double) * 1);
		double *R_P = mat(nv, nv);

		for (i = 0; i < nv; i++)
			for (j = 0; j < nv; j++)
			{
				R[i + j * nv] = i == j ? var[i] : 0.0;
			}
		//for(int i=0;i<nv*nv;i++)
		//	printf("R[%d]=%f\n", i, R[i]);
		for (i = 0; i < nv; i++)
		{
			for (j = 0; j < nv; j++) {
				if (i == j)
				{
					R_P[i + j * nv] = 1 / sqrt(R[i + j * nv]);  //R_权矩阵
				}
				else
					R_P[i + j * nv] = 0.0;
			}
		}
		//for (int i = 0; i<nv*nv; i++)
		//	printf("R_P[%d]=%f\n", i, R_P[i]);
		matmul("TN", 1, nv, nv, 1.0, v, R_P, 0.0, VR);       //F=V'*R_
		matmul("NN", 1, 1, nv, 1.0, VR, v, 0.0, VRV);       //F=V'R_V


		matmul("NN", *np, nv, *np, 1.0, P_, H_, 0.0, F);       /* Q=H'*P*H+R */
		matmul("TN", nv, nv, *np, 1.0, H_, F, 1.0, Q);           //Q残差方差

		matcpy(Q, R, nv, nv);
		for (i = 0; i < nv; i++)
			for (j = 0; j < nv; j++) {
				if (i == j) {
					//sig[i] = sqrt(R[i + j * nv]);//sig 量测标准差
					sig[i] = sqrt(*VRV / nv);//sig 量测标准差
					RR[i] = sqrt(fabs(Q[i + j * nv])) * sig[i]; //Q信息协方差阵  -> 多余分量
					RR[i] = fabs(v[i]) / RR[i]; //标准化残差？
					//printf("RR[%d]=%f\n", i, RR[i]);
				}
			}
		for (i = 0; i < nv; i++) {
			//printf("RR[%d]=%lf\n", i, RR[i]);
			if (fabs(RR[i]) > THRES_REJECT) {
				obsi[ne] = obs_use[i];
				frqi[ne] = (i % 2 == 0) ? 0 : 1;
				ve[ne] = RR[i];
				ne++;
            }
		}
		if (ne>0) {
			vmax = ve[0]; maxobs = obsi[0]; maxfrq = frqi[0]; rej = 0;
			for (j = 1; j<ne; j++) {
				if (fabs(vmax) >= fabs(ve[j])) continue;
				vmax = ve[j]; maxobs = obsi[j]; maxfrq = frqi[j]; rej = j;
			}
			sat = obs[maxobs].sat;
			robust[maxobs] = sat;
            //fprintf(outFp[OFILE_DEBUG], "outlier (%d) robust %s sat=%2d %s%d res=%9.4f el=%4.1f\n",
                //post, str, sat, maxfrq % 2 ? "P" : "L", maxfrq / 2 + 1, vmax, azel[1 + maxobs * 2] * R2D);
            //printf( "outlier (%d) robust %s sat=%2d %s%d res=%9.4f el=%4.1f\n",
                //post, str, sat, maxfrq % 2 ? "P" : "L", maxfrq / 2 + 1, vmax, azel[1 + maxobs * 2] * R2D);
		//	exc[maxobs] = 1; rtk->ssat[sat - 1].rejc[maxfrq % 2]++; stat = 0;
			rtk->ssat[sat - 1].rejc[maxfrq % 2]++; stat = 0;
			ve[rej] = 0;
		}
		//-----------------------------------------------------------------------------

		//---------------------------等加权计算----------------------------------------

		for (i = 0; i < n; i++)
			if (robust[i] != 0) {             //抗差卫星号
				prnn = robust[i];

				for (k = 0; k < nv; k++) {
					if (sat_use[k] == prnn)//找到抗差卫星号对应的var序号
					{
						if (RR[k] <= THRES_REJECT_K0)
							var[k] = var[k];
						else if ((RR[k] > THRES_REJECT_K0) && (RR[k] <= THRES_REJECT_K1)) {
							pp_ = 1 / sqrt(var[k]);
							pp = pp_ * (THRES_REJECT_K0 / RR[k])*SQR(((THRES_REJECT_K1 - RR[k]) / (THRES_REJECT_K1 - THRES_REJECT_K0)));
							var[k] = SQR((1 / pp));

						}
						else
							var[k] = 10E8;
					}
					else
						continue;
				}

			}
			else
				continue;
		free(VR); free(VRV); free(R_P);
		free(F); free(Q); free(sig); free(RR);;
	}
	//-------------------------------------------------------------------

	for (i = 0; i < nv; i++) 
		for (j = 0; j < nv; j++) {
			R[i + j * nv] = i == j ? var[i] : 0.0;
		}
	
	return post ? stat : nv;
}
#else
static int ppp_res(int post, const obsd_t *obs, int n, const double *rs,
    const double *dts, const double *var_rs, const int *svh,
    const double *dr, int *exc, const nav_t *nav,
    double *x,double *Pp, rtk_t *rtk, double *v, double *H, double *R,
    double *azel)
{
    const double *lam;
    prcopt_t *opt = &rtk->opt;
    double y, r, cdtr, bias, C, rr[3], pos[3], e[3], dtdx[3], L[NFREQ], P[NFREQ], Lc, Pc;
    double var[MAXOBS * 2], dtrp = 0.0, dion = 0.0, vart = 0.0, vari = 0.0, dcb;
    double dantr[NFREQ] = { 0 }, dants[NFREQ] = { 0 };
    double ve[MAXOBS * 2 * NFREQ] = { 0 }, vmax = 0, gravitationalDelayModel, shd = 0.0;
    char str[32];
    int ne = 0, obsi[MAXOBS * 2 * NFREQ] = { 0 }, frqi[MAXOBS * 2 * NFREQ], maxobs, maxfrq, rej;
    int i, j, k, sat, sys, nv = 0, nx = rtk->nx, stat = 1;

    time2str(obs[0].time, str, 2);

    for (i = 0; i<MAXSAT; i++) for (j = 0; j<opt->nf; j++) rtk->ssat[i].vsat[j] = 0;

    for (i = 0; i<3; i++) rr[i] = x[i] + dr[i];
    ecef2pos(rr, pos);
    int sow, week;
    int tss;
    int jj = 0;
    sow = time2gpst(obs[0].time, &week);
    if (sow == 351912)
        tss = 1;
    for (i = 0; i<n&&i<MAXOBS; i++) {
        sat = obs[i].sat;
        lam = nav->lam[sat - 1];
        if (lam[j / 2] == 0.0 || lam[0] == 0.0) {
            jj = IB(sat, 0, &rtk->opt);
            Pp[jj + jj * rtk->nx] = 0.0;
            x[jj] = 0.0;
            continue;
        }

        if ((r = geodist(rs + i * 6, rr, e)) <= 0.0 ||
            satazel(pos, e, azel + i * 2)<opt->elmin) {
            exc[i] = 1;
            jj = IB(sat, 0, &rtk->opt);
            Pp[jj + jj * rtk->nx] = 0.0;
            x[jj] = 0.0;
            continue;
        }

        //****************************************************
        //---------------record sat position and azel date----
        for (int kk = 0; kk < 3; kk++) {
            rtk->ssat[sat - 1].satpos[kk] = (rs + i * 6)[kk];
            rtk->ssat[sat - 1].azel[kk] = (azel + i * 2)[kk];
            rtk->ssat[sat - 1].satclk = dts[i * 2] * CLIGHT;
        }

        double cosaz, sinaz, cosel, sinel, enu[3] = { 0 };
        //line-of-sight (LOS) unit vector
        cosel = cos(rtk->ssat[sat - 1].azel[1]); sinel = sin(rtk->ssat[sat - 1].azel[1]);
        cosaz = cos(rtk->ssat[sat - 1].azel[0]); sinaz = sin(rtk->ssat[sat - 1].azel[0]);

        //convert 3D tidal displacements to LOS range
        ecef2enu(pos, dr, enu);
        rtk->ssat[sat - 1].dtid = enu[1] * cosel*cosaz + enu[0] * cosel*sinaz + enu[2] * sinel;
        //****************************************************
        //------------------------------------------------------

        if (!(sys = satsys(sat, NULL)) || !rtk->ssat[sat - 1].vs ||
            satexclude(obs[i].sat, svh[i], opt) || exc[i]) {
            exc[i] = 1;
            jj = IB(sat, 0, &rtk->opt);
            Pp[jj + jj * rtk->nx] = 0.0;
            x[jj] = 0.0;
            continue;
        }
        /* tropospheric and ionospheric model */
        if (!model_trop(obs[i].time, pos, azel + i * 2, opt, x, dtdx, nav, &dtrp, &shd, &vart) ||
            !model_iono(obs[i].time, pos, azel + i * 2, opt, sat, x, nav, &dion, &vari)) {
            jj = IB(sat, 0, &rtk->opt);
            Pp[jj + jj * rtk->nx] = 0.0;
            x[jj] = 0.0;
            continue;
        }
        //****************************************************
        //---------------record tropospheric date--------------
        rtk->ssat[sat - 1].dsag = sagnac(rs + i * 6, rr);
        rtk->ssat[sat - 1].dtrp = dtrp;     //对流层总延迟
        rtk->ssat[sat - 1].shd = shd;      //对流层延迟干分量=干分量投影函数*ztd
        rtk->ssat[sat - 1].wmap = dtdx[0]; //湿分量投影函数
        //****************************************************
        //------------------------------------------------------

        /* satellite and receiver antenna model */
        if (opt->posopt[0])
            satantpcv(sat, rs + i * 6, rr, nav->pcvs + sat - 1, dants);

        //printf("sat=%3d dants=%10.7lf %10.7lf\t %10.7lf\n", sat, dants[0], dants[1], dants[2]);

        antmodel(sat, opt->pcvr, opt->antdel[0], azel + i * 2, opt->posopt[1], dantr);

        /* phase windup model */
        if (!model_phw(rtk->sol.time, sat, nav->pcvs[sat - 1].type,
            opt->posopt[2] ? 2 : 0, rs + i * 6, rr, &rtk->ssat[sat - 1].phw)) {
            jj = IB(sat, 0, &rtk->opt);
            Pp[jj + jj * rtk->nx] = 0.0;
            x[jj] = 0.0;
            continue;
        }
        //****************************************************
        //---------------record dant date--------------
        for (j = 0; j < NFREQ; j++) rtk->ssat[sat - 1].dant[j] = dants[j];

        //****************************************************
        //------------------------------------------------------

        /* corrected phase and code measurements */
        corr_meas(obs + i, nav, azel + i * 2, &rtk->opt, dantr, dants,
            rtk->ssat[sat - 1].phw, L, P, &Lc, &Pc);
        //****************************************************
        //---------------record obs date----------------------
        for (int kk = 0; kk < NFREQ; kk++)
        {
            rtk->ssat[sat - 1].P[kk] = obs[i].P[kk];
            rtk->ssat[sat - 1].L[kk] = obs[i].L[kk];
        }
        //-----------------------------------------------------
        //****************************************************

        gravitationalDelayModel = gravitationalDelayCorrection(sys, rr, rs + i * 6);
        /* stack phase and code residuals {L1,P1,L2,P2,...} */
        for (j = 0; j<2 * NF(opt); j++) {

            dcb = bias = 0.0;

            if (opt->ionoopt == IONOOPT_IFLC) {
                if ((y = j % 2 == 0 ? Lc : Pc) == 0.0) {
                    jj = IB(sat, 0, &rtk->opt);
                    Pp[jj + jj * rtk->nx] = 0.0;
                    x[jj] = 0.0;
                    continue;
                }
            }
            else {
                if ((y = j % 2 == 0 ? L[j / 2] : P[j / 2]) == 0.0)
                {
                    jj = IB(sat, 0, &rtk->opt);
                    Pp[jj + jj * rtk->nx] = 0.0;
                    x[jj] = 0.0;
                    continue;
                }
                /* receiver DCB correction for P2 */
                if (j / 2 == 1) dcb = -nav->rbias[0][sys == SYS_GLO ? 1 : 0][0];
            }
            C = SQR(lam[j / 2] / lam[0])*ionmapf(pos, azel + i * 2)*(j % 2 == 0 ? -1.0 : 1.0);

            for (k = 0; k < nx; k++) 	H[k + nx * nv] = k < 3 ? -e[k] : 0.0;


            /* receiver clock */
            k = sys == SYS_GLO ? 1 : (sys == SYS_GAL ? 2 : (sys == SYS_CMP ? 3 : 0));
            cdtr = x[IC(k, opt)];
            H[IC(k, opt) + nx * nv] = 1.0;
            if (opt->tropopt == TROPOPT_EST || opt->tropopt == TROPOPT_ESTG) {
                for (k = 0; k<(opt->tropopt >= TROPOPT_ESTG ? 3 : 1); k++) {
                    H[IT(opt) + k + nx * nv] = dtdx[k];
                }
            }
            if (opt->ionoopt == IONOOPT_EST) {
                if (rtk->x[II(sat, opt)] == 0.0)
                {
                    jj = IB(sat, 0, &rtk->opt);
                    Pp[jj + jj * rtk->nx] = 0.0;
                    x[jj] = 0.0;
                    continue;
                }
                H[II(sat, opt) + nx * nv] = C;
            }
            if (j / 2 == 2 && j % 2 == 1) { /* L5-receiver-dcb */
                dcb += rtk->x[ID(opt)];
                H[ID(opt) + nx * nv] = 1.0;
            }
            if (j % 2 == 0) { /* phase bias */
                if ((bias = x[IB(sat, j / 2, opt)]) == 0.0)
                {
                    jj = IB(sat, 0, &rtk->opt);
                    Pp[jj + jj * rtk->nx] = 0.0;
                   x[jj] = 0.0;
                    continue;
                }
                H[IB(sat, j / 2, opt) + nx * nv] = 1.0;
            }
            /* residual */
            v[nv] = y - (r + cdtr - CLIGHT * dts[i * 2] + dtrp + C * dion + dcb + bias - gravitationalDelayModel);
            if (j % 2 == 0) rtk->ssat[sat - 1].resc[j / 2] = v[nv];
            else        rtk->ssat[sat - 1].resp[j / 2] = v[nv];

            /* variance */
            var[nv] = varerr(obs[i].sat, sys, azel[1 + i * 2], j / 2, j % 2, opt) +vart + SQR(C)*vari + var_rs[i];
            if (sys == SYS_GLO && j % 2 == 1) var[nv] += VAR_GLO_IFB;

        //	double test = v[nv] / sqrt(var[nv]);
            trace(4, "%s sat=%2d %s%d res=%9.4f sig=%9.4f el=%4.1f\n", str, sat,
                j % 2 ? "P" : "L", j / 2 + 1, v[nv], sqrt(var[nv]), azel[1 + i * 2] * R2D);

            /* reject satellite by pre-fit residuals */
            if (!post&&opt->maxinno>0.0&&fabs(v[nv])>opt->maxinno) {
                fprintf(outFp[OFILE_DEBUG], "outlier (%d) rejected %s sat=%2d %s%d res=%9.4f el=%4.1f\n",
                    post, str, sat, j % 2 ? "P" : "L", j / 2 + 1, v[nv], azel[1 + i * 2] * R2D);
                //printf("outlier (%d) rejected %s sat=%2d %s%d res=%9.4f el=%4.1f\n",
                    //post, str, sat, j % 2 ? "P" : "L", j / 2 + 1, v[nv], azel[1 + i * 2] * R2D);
                exc[i] = 1; rtk->ssat[sat - 1].rejc[j % 2]++;

                jj = IB(sat, 0, &rtk->opt);
                Pp[jj + jj * rtk->nx] = 0.0;
                x[jj] = 0.0;
                continue;
            }
            /* record large post-fit residuals */
            //if (post&&fabs(v[nv])>sqrt(var[nv])*THRES_REJECT) {
            if (post && (fabs((v[nv])>sqrt(var[nv])*THRES_REJECT) || ((fabs(v[nv])>0.1) && j == 0))) {
                obsi[ne] = i; frqi[ne] = j; ve[ne] = v[nv]; ne++;
            }
            if (j % 2 == 0) rtk->ssat[sat - 1].vsat[j / 2] = 1;
            nv++;
        }
    }
    /* reject satellite with large and max post-fit residual */
    if (post&&ne>0) {
        vmax = ve[0]; maxobs = obsi[0]; maxfrq = frqi[0]; rej = 0;
        for (j = 1; j<ne; j++) {
            if (fabs(vmax) >= fabs(ve[j])) continue;
            vmax = ve[j]; maxobs = obsi[j]; maxfrq = frqi[j]; rej = j;
        }
        sat = obs[maxobs].sat;
        fprintf(outFp[OFILE_DEBUG], "outlier (%d) rejected %s sat=%2d %s%d res=%9.4f el=%4.1f\n",
            post, str, sat, maxfrq % 2 ? "P" : "L", maxfrq / 2 + 1, vmax, azel[1 + maxobs * 2] * R2D);
        printf( "outlier (%d) rejected %s sat=%2d %s%d res=%9.4f el=%4.1f\n",
            post, str, sat, maxfrq % 2 ? "P" : "L", maxfrq / 2 + 1, vmax, azel[1 + maxobs * 2] * R2D);
        exc[maxobs] = 1; rtk->ssat[sat - 1].rejc[maxfrq % 2]++; stat = 0;
        ve[rej] = 0;
    }

    //for (i = 0; i < nv; i++)
    //{
    //	printf("v[%d]=%lf,var[%d]=%lf\n",i, v[i],i, var[i]);
    //}

    for (i = 0; i < nv; i++) for (j = 0; j < nv; j++) {

        R[i + j * nv] = i == j ? var[i] : 0.0;
    }

    return post ? stat : nv;
}
#endif
/* number of estimated states ------------------------------------------------*/
extern int pppnx(const prcopt_t *opt)
{
    return NX(opt);
}

static void Record_Ambiguity(rtk_t *rtk, double *ep, int *week, double *sow)
{
    char *sep = " ";
    int i = 0;
    int j = IT(&rtk->opt) + 1;
    fprintf(outFp[OFILE_AMBIGUITY], "%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s%04d%s%9.2f%s",
        (int)ep[0], sep, (int)ep[1], sep, (int)ep[2], sep, (int)ep[3], sep, (int)ep[4], sep, (int)ep[5], sep, *week, sep, *sow, sep);

    for (i = 0; i < MAXSAT; i++)
    {
        if (rtk->x[i+j]==0)
        {
            rtk->x[i + j] = 99999.0;
        }

        fprintf(outFp[OFILE_AMBIGUITY], "%10.4f%s", rtk->x[i + j],sep);
    }

    fprintf(outFp[OFILE_AMBIGUITY], "\n");
}

static void Record_res(rtk_t *rtk, double *ep, char *sep, int *week, double *sow)
{

	unsigned char buff1[MAXSOLMSG + 1];
	unsigned char buff2[MAXSOLMSG + 1];
	char *p1 = (char *)buff1;
	char *p2 = (char *)buff2;
	int n1, n2;
	int i=0;
	double resc, resp;
    fprintf(outFp[OFILE_RESC], "%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s%04d%s%9.2f%s",
        (int)ep[0], sep, (int)ep[1], sep, (int)ep[2], sep, (int)ep[3], sep, (int)ep[4], sep, (int)ep[5], sep, *week, sep, *sow, sep);
    fprintf(outFp[OFILE_RESP], "%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s%04d%s%9.2f%s",
        (int)ep[0], sep, (int)ep[1], sep, (int)ep[2], sep, (int)ep[3], sep, (int)ep[4], sep, (int)ep[5], sep, *week, sep, *sow, sep);

    for (i = 0; i < MAXSAT; i++) {
		if (rtk->ssat[i].vsat[0] == 0) {
			resc = 99999.0;
			resp = 99999.0;
		}
		if (rtk->ssat[i].vsat[0] == 1 && (rtk->ssat[i].P[0] != 0 || rtk->ssat[i].P[1]!= 0 || rtk->ssat[i].L[0] != 0 || rtk->ssat[i].L[1] != 0)) {
			resc = rtk->ssat[i].resc[0];
			resp = rtk->ssat[i].resp[0];

		}
        fprintf(outFp[OFILE_RESC], "%10.4f%s", resc,sep);
        fprintf(outFp[OFILE_RESP], "%10.4f%s", resp,sep);
	}
    fprintf(outFp[OFILE_RESC],"\n");
    fprintf(outFp[OFILE_RESP], "\n");

}

static void outPdop(rtk_t *rtk, const obsd_t *obs)
{
	unsigned char buff[MAXSOLMSG + 1];
	int n;
	char *p = (char *)buff;
	char *sep = " ";
	double sow, ep[6];
	int week, nsat;

	time2epoch(obs[0].time, ep);

	sow = time2gpst(obs[0].time, &week);

	nsat = rtk->sol.nsats[0] + rtk->sol.nsats[1] + rtk->sol.nsats[2] + rtk->sol.nsats[3] + rtk->sol.nsats[4];
    fprintf(outFp[OFILE_PDOP], "%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s%4d%s%9.2f%s%3d%s%2d%s%2d%s%2d%s%2d%s%2d%s%10.3f\n",
        (int)ep[0], sep, (int)ep[1], sep, (int)ep[2], sep, (int)ep[3], sep, (int)ep[4], sep, (int)ep[5], sep, week,
        sep, sow, sep, nsat, sep, rtk->sol.nsats[0], sep, rtk->sol.nsats[1], sep, rtk->sol.nsats[2], sep, rtk->sol.nsats[3], sep, rtk->sol.nsats[4],
        sep, rtk->sol.dop[1]);

}

/* update solution status ----------------------------------------------------*/
static void update_stat(rtk_t *rtk, const obsd_t *obs, int n, int nv,int stat)
{
	const prcopt_t *opt = &rtk->opt;
	int i, j;
	char test_output[500] = "\0";
	char *sep = " ";
	int week;
	double sow, ep[6], pos[3], denu[3], dxyz[3] = {0.0};
	time2epoch(obs[0].time, ep);
	sow = time2gpst(obs[0].time, &week);

	if (sow == 355868)
		printf("ok\n");
	/* test # of valid satellites */
	rtk->sol.ns = 0;
	for (i = 0; i < n&&i < MAXOBS; i++) {
		for (j = 0; j < opt->nf; j++) {
			if (!rtk->ssat[obs[i].sat - 1].vsat[j]) continue;
			rtk->ssat[obs[i].sat - 1].lock[j]++;
			rtk->ssat[obs[i].sat - 1].outc[j] = 0;
			if (j == 0) rtk->sol.ns++;
		}
	}
	rtk->sol.stat = rtk->sol.ns < MIN_NSAT_SOL ? SOLQ_NONE : stat;

	if (rtk->sol.stat == SOLQ_FIX) {
		for (i = 0; i < 3; i++) {
			rtk->sol.rr[i] = rtk->xa[i];
			rtk->sol.qr[i] = (float)rtk->Pa[i + i * rtk->na];
		}
		rtk->sol.qr[3] = (float)rtk->Pa[1];
		rtk->sol.qr[4] = (float)rtk->Pa[1 + 2 * rtk->na];
		rtk->sol.qr[5] = (float)rtk->Pa[2];
	}
	else {
		for (i = 0; i < 3; i++) {
			rtk->sol.rr[i] = rtk->x[i];
			rtk->sol.qr[i] = (float)rtk->P[i + i * rtk->nx];
		}
		rtk->sol.qr[3] = (float)rtk->P[1];
		rtk->sol.qr[4] = (float)rtk->P[2 + rtk->nx];
		rtk->sol.qr[5] = (float)rtk->P[2];
	}
	rtk->sol.dtr[0] = rtk->x[IC(0, opt)];
	rtk->sol.dtr[1] = rtk->x[IC(1, opt)] - rtk->x[IC(0, opt)];

	for (i = 0; i < n&&i < MAXOBS; i++) for (j = 0; j < opt->nf; j++) {
		rtk->ssat[obs[i].sat - 1].snr[j] = obs[i].SNR[j];
	}
	for (i = 0; i < MAXSAT; i++) for (j = 0; j < opt->nf; j++) {
		if (rtk->ssat[i].slip[j] & 3) rtk->ssat[i].slipc[j]++;
		if (rtk->ssat[i].fix[j] == 2 && stat != SOLQ_FIX) rtk->ssat[i].fix[j] = 1;
	}


    double ture_xyz[3] = { -2005044.4023382 ,5411159.3009611 ,2707856.7667605 };
        ecef2pos(ture_xyz, pos);
        for (i = 0; i<3; i++)
            denu[i] = rtk->sol.dxyz[i] = rtk->sol.rr[i] - ture_xyz[i];
        ecef2enu(pos, rtk->sol.dxyz, denu);

    double time_dif = timediff(obs[0].time, ssr_time);
    ecef2pos(rtk->sol.rr, pos);
    //printf("%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s%04d%s%9.2f%s%14.7f%s%14.7f%s%14.7f%s%7.4f%s%7.4f%s%7.4f%s%7.4f%s%02d%s%02d%s%02d%s%4.2f%s%3.2f\n",
       // (int)ep[0], sep, (int)ep[1], sep, (int)ep[2], sep, (int)ep[3], sep, (int)ep[4], sep, (int)ep[5], sep, week, sep, sow, sep, rtk->sol.rr[0], sep, rtk->sol.rr[1], sep, rtk->sol.rr[2], sep, denu[0], sep, denu[1], sep, denu[2], sep, norm(denu, 3), sep,stat,sep ,n, sep,nv,sep, rtk->sol.dop[1],sep, time_dif);
    fprintf(outFp[OFILE_PPPPOS],"%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s%04d%s%9.2f%s%14.7f%s%14.7f%s%14.7f%s%7.4f%s%7.4f%s%7.4f%s%7.4f%s%02d%s%02d%s%4.2f%s%3.2f\n",
            (int)ep[0], sep, (int)ep[1], sep, (int)ep[2], sep, (int)ep[3], sep, (int)ep[4], sep, (int)ep[5], sep, week, sep, sow, sep, rtk->sol.rr[0], sep, rtk->sol.rr[1], sep, rtk->sol.rr[2], sep, denu[0], sep, denu[1], sep, denu[2], sep, norm(denu, 3), sep, n, sep,nv,sep, rtk->sol.dop[1],sep,time_dif);

//    char id[MAXSAT];
//    fprintf(outFp[OFILE_PPP], "%4d %2d %2d %2d %2d %11.7f   %3d", (int)ep[0],
//        (int)ep[1], (int)ep[2], (int)ep[3], (int)ep[4], ep[5], n);
//    fprintf(outFp[OFILE_PPP], "\n");
//    for (i = 0; i < MAXSAT; i++) {
//        if (rtk->ssat[i].vsat[0] == 0) continue;
//        int sat = i + 1;
//        if (rtk->ssat[sat - 1].P[0] == 0 || rtk->ssat[sat - 1].P[1] == 0  || rtk->ssat[sat - 1].L[0] == 0 || rtk->ssat[sat - 1].L[1] == 0 )continue;

//        satno2id(sat, id);
//        fprintf(outFp[OFILE_PPP], "%3d %15.4f %15.4f %15.4f %15.4f %15.4f %15.4f %15.4f %15.4f %15.4f"
//            "%15.4f %15.4f %15.4f %15.4f %15.4f %15.4f %15.4f %15.4f %15.4f %15.4f", sat,
//            rtk->ssat[sat - 1].satpos[0], rtk->ssat[sat - 1].satpos[1], rtk->ssat[sat - 1].satpos[2],
//            rtk->ssat[sat - 1].satclk, rtk->ssat[sat - 1].azel[1] * R2D, rtk->ssat[sat - 1].azel[0] * R2D,
//            rtk->ssat[sat - 1].P[0], rtk->ssat[sat - 1].P[1], rtk->ssat[sat - 1].P[2],
//            rtk->ssat[sat - 1].L[0], rtk->ssat[sat - 1].L[1], rtk->ssat[sat - 1].L[2],
//            rtk->ssat[sat - 1].shd, rtk->ssat[sat - 1].wmap, rtk->ssat[sat - 1].dsag,
//            rtk->ssat[sat - 1].dtid, rtk->ssat[sat - 1].dant[0], rtk->ssat[sat - 1].dant[1],
//            rtk->ssat[sat - 1].phw);

//        fprintf(outFp[OFILE_PPP], "%15.4lf %15.4lf %4d", rtk->ssat[sat - 1].resp[0], rtk->ssat[sat - 1].resc[0], rtk->ssat[sat - 1].slip[0]);

//        fprintf(outFp[OFILE_PPP], "\n");
//    }
    Record_res(rtk, ep, sep,&week,&sow);
    outPdop(rtk, obs);

}


/* precise point positioning -------------------------------------------------*/
#if 0
extern void pppos(rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav)
{
    const prcopt_t *opt=&rtk->opt;
    double *rs,*dts,*var,*v,*H,*R,*azel,*xp,*Pp,dr[3]={0},std[3];
    char str[32];
    int i,j,nv,info,svh[MAXOBS],exc[MAXOBS]={0},stat=SOLQ_SINGLE, robust[MAXOBS] = { 0 };
	double var_R[MAXOBS * 2];
	double stdd = 0.0;
	double  sow,ep[6];
	int week;
	time2epoch(obs[0].time, ep);
	sow = time2gpst(obs[0].time, &week);

    time2str(obs[0].time,str,2);
    trace(3,"pppos   : time=%s nx=%d n=%d\n",str,rtk->nx,n);
   
    rs=mat(6,n); dts=mat(2,n); var=mat(1,n); azel=zeros(2,n);
    
    for (i=0;i<MAXSAT;i++) for (j=0;j<opt->nf;j++) rtk->ssat[i].fix[j]=0;
    
    /* temporal update of ekf states */
    udstate_ppp(rtk,obs,n,nav);
    
    /* satellite positions and clocks */
    satposs(obs[0].time,obs,n,nav,rtk->opt.sateph,rs,dts,var,svh);
    
    /* exclude measurements of eclipsing satellite (block IIA) */
    if (rtk->opt.posopt[3]) {
        testeclipse(obs,n,nav,rs);
    }
    /* earth tides correction */
    if (opt->tidecorr) {
        tidedisp(gpst2utc(obs[0].time),rtk->x,opt->tidecorr==1?1:7,&nav->erp,
                 opt->odisp[0],dr);
    }
    nv=n*rtk->opt.nf;
    xp=mat(rtk->nx,1); Pp=zeros(rtk->nx,rtk->nx);
    v=mat(nv,1); H=mat(rtk->nx,nv); R=mat(nv,nv);

	double *P_, *H_;
	int ii, jj, kk, *ix, nn;
	ix = imat(rtk->nx, 1);
	for (ii = kk = 0; ii < rtk->nx; ii++)
		if (xp[ii] != 0.0&&rtk->P[ii + ii * rtk->nx] > 0.0)
			ix[kk++] = ii;//找出状态方程未知数所在列数,K未知参数个数=位置+钟差+tro+ion+卫星个数
	P_ = mat(kk, kk); H_ = mat(kk, nv);

    for (i=0;i<MAX_ITER;i++) {
        
        matcpy(xp,rtk->x,rtk->nx,1);
        matcpy(Pp,rtk->P,rtk->nx,rtk->nx);

		//for (int ii = 0; ii < rtk->nx*nv; ii++)
			//if (Pp[ii] != 0)
			//	printf("Pp[%d] = %lf\n  ", ii, H[ii]);
        /* prefit residuals */
        if (!(nv=ppp_res(0,obs,n,rs,dts,var,svh,dr,exc,nav,xp, Pp,rtk,v,H,R,azel, P_, H_, &nn, robust, var_R))) {
            printf("%s ppp (%d) no valid obs data\n",str,i+1);
            break;
        }


		if ((info = filter(xp, Pp, H, v, R, rtk->nx, nv, &nn, &stdd))) {
			printf("%s ppp (%d) filter error info=%d\n", str, i + 1, info);
			break;
		}
		free(P_); free(H_);
		for (ii = kk = 0; ii < rtk->nx; ii++)
			if (xp[ii] != 0.0&&Pp[ii + ii * rtk->nx] > 0.0)
				ix[kk++] = ii;//找出状态方程未知数所在列数,K未知参数个数=位置+钟差+tro+ion+卫星个数
		P_ = mat(kk, kk); H_ = mat(kk, nv);
		//-------------------------------------验后p H---------------------------------------------------------------------
		for (ii = 0; ii < nn; ii++) {
			for (jj = 0; jj < nn; jj++) P_[ii + jj * nn] = Pp[ix[ii] + ix[jj] * rtk->nx];//155*155->n*n 为
			for (jj = 0; jj < nv; jj++) H_[ii + jj * nn] = H[ix[ii] + jj * rtk->nx];
		}

		if (ppp_res(i + 1, obs, n, rs, dts, var, svh, dr, exc, nav, xp, Pp, rtk, v, H, R, azel, P_, H_, &nn, robust, var_R)) {
			matcpy(rtk->x, xp, rtk->nx, 1);
			matcpy(rtk->P, Pp, rtk->nx, rtk->nx);
            stat = SOLQ_PPP;
			if (stdd < 0.02)
				break;
			else
				continue;
		}
		if (i == MAX_ITER - 1) {
			matcpy(rtk->x, xp, rtk->nx, 1);
			matcpy(rtk->P, Pp, rtk->nx, rtk->nx);
			stat = SOLQ_PPP;
		}

	}
#else
extern void pppos(rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav)
{
    const prcopt_t *opt=&rtk->opt;
    double *rs,*dts,*var,*v,*H,*R,*azel,*xp,*Pp,dr[3]={0},std[3];
    char str[32];
    int i,j,nv,info,svh[MAXOBS],exc[MAXOBS]={0},stat=SOLQ_SINGLE;
    double stdd = 0.0;
    double  sow,ep[6];
    int week;
    time2epoch(obs[0].time, ep);
    sow = time2gpst(obs[0].time, &week);

    time2str(obs[0].time,str,2);
    trace(3,"pppos   : time=%s nx=%d n=%d\n",str,rtk->nx,n);

    rs=mat(6,n); dts=mat(2,n); var=mat(1,n); azel=zeros(2,n);

    for (i=0;i<MAXSAT;i++) for (j=0;j<opt->nf;j++) rtk->ssat[i].fix[j]=0;

    /* temporal update of ekf states */
    udstate_ppp(rtk,obs,n,nav);

    /* satellite positions and clocks */
    satposs(obs[0].time,obs,n,nav,rtk->opt.sateph,rs,dts,var,svh);

    /* exclude measurements of eclipsing satellite (block IIA) */
    if (rtk->opt.posopt[3]) {
        testeclipse(obs,n,nav,rs);
    }
    /* earth tides correction */
    if (opt->tidecorr) {
        tidedisp(gpst2utc(obs[0].time),rtk->x,opt->tidecorr==1?1:7,&nav->erp,
                 opt->odisp[0],dr);
    }
    nv=n*rtk->opt.nf;
    xp=mat(rtk->nx,1); Pp=zeros(rtk->nx,rtk->nx);
    v=mat(nv,1); H=mat(rtk->nx,nv); R=mat(nv,nv);
    for (i=0;i<MAX_ITER;i++) {

        matcpy(xp,rtk->x,rtk->nx,1);
        matcpy(Pp,rtk->P,rtk->nx,rtk->nx);
        //for (int kkk = 0; kkk < rtk->nx; kkk++)
        //	printf("x[%d]=%lf\n", kkk, rtk->x[kkk]);
        /* prefit residuals */
        if (!(nv=ppp_res(0,obs,n,rs,dts,var,svh,dr,exc,nav,xp, Pp,rtk,v,H,R,azel))) {
            printf("%s ppp (%d) no valid obs data\n",str,i+1);
            break;
        }

        if ((info = filter(xp, Pp, H, v, R, rtk->nx, nv))) {
            printf("%s ppp (%d) filter error info=%d\n", str, i + 1, info);
            break;
        }

        if (ppp_res(i + 1, obs, n, rs, dts, var, svh, dr, exc, nav, xp, Pp, rtk, v, H, R, azel)) {
            matcpy(rtk->x, xp, rtk->nx, 1);
            matcpy(rtk->P, Pp, rtk->nx, rtk->nx);
            stat = SOLQ_PPP;
            break;
        }

    }
#endif
    Record_Ambiguity(rtk,ep,&week,&sow);

	if (stat == SOLQ_PPP) {
	//	/* ambiguity resolution in ppp */
	//	if (ppp_ar(rtk,obs,n,exc,nav,azel,xp,Pp)&&
	//	ppp_res(9,obs,n,rs,dts,var,svh,dr,exc,nav,xp,rtk,v,H,R,azel)) {

	//	matcpy(rtk->xa,xp,rtk->nx,1);
	//	matcpy(rtk->Pa,Pp,rtk->nx,rtk->nx);

	//	for (i=0;i<3;i++) std[i]=sqrt(Pp[i+i*rtk->nx]);
	//	if (norm(std,3)<MAX_STD_FIX) 
	//		stat=SOLQ_FIX;
	//	}
	//	else {
		rtk->nfix = 0;
	//	}
		/* update solution status */
		update_stat(rtk, obs, n, nv/2, stat);
	}
	free(rs); free(dts); free(var); free(azel);
	free(xp); free(Pp); free(v); free(H); free(R);
    //free(P_); free(H_); free(ix);
}
