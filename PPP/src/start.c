/*------------------------------------------------------------------------------
 * rtksvr.c : rtk server functions
 *
 *          Copyright (C) 2007-2013 by T.TAKASU, All rights reserved.
 *
 * options : -DWIN32    use WIN32 API
 *
 * version : $Revision:$ $Date:$
 * history : 2009/01/07  1.0  new
 *           2009/06/02  1.1  support glonass
 *           2010/07/25  1.2  support correction input/log stream
 *                            supoort online change of output/log streams
 *                            supoort monitor stream
 *                            added api:
 *                                rtksvropenstr(),rtksvrclosestr()
 *                            changed api:
 *                                rtksvrstart()
 *           2010/08/25  1.3  fix problem of ephemeris time inversion (2.4.0_p6)
 *           2010/09/08  1.4  fix problem of ephemeris and ssr squence upset
 *                            (2.4.0_p8)
 *           2011/01/10  1.5  change api: rtksvrstart(),rtksvrostat()
 *           2011/06/21  1.6  fix ephemeris handover problem
 *           2012/05/14  1.7  fix bugs
 *           2013/03/28  1.8  fix problem on lack of glonass freq number in raw
 *                            fix problem on ephemeris with inverted toe
 *                            add api rtksvrfree()
 *           2014/06/28  1.9  fix probram on ephemeris update of beidou
 *           2015/04/29  1.10 fix probram on ssr orbit/clock inconsistency
 *           2015/07/31  1.11 add phase bias (fcb) correction
 *           2015/12/05  1.12 support opt->pppopt=-DIS_FCB
 *-----------------------------------------------------------------------------*/
#include "rtklib.h"
static const char rcsid[] = "$Id:$";
const char outfile_type[MAXOUTFILE][100] = {
	{ "pos\0     " },{ "spppos\0   " },{ "bropos\0     " },{ "ssrpos\0    " },{ "prepos\0    " },
	{ "debug\0     " },{ "ambiguity\0   " },{ "resc\0     " },{ "resp\0     " },{ "pdop\0     " },
	{ "gfslip\0     " },{ "mwslip\0     " },{ "ppp\0     " }
};

//---------------------------ck gai------------------------------------------
static stream_t moni1;
rtksvr_t svr1;
extern prcopt_t prcopt1;
static solopt_t solopt1[2] = { { 0 } };
static filopt_t filopt1 = { "" };
sta_t stas = { 0 };      /* station infomation */

char inputfilePath[2][1024];

static int svrcycle = 10;            /* server cycle (ms) */
static int timeout = 10000;         /* timeout time (ms) */
static int reconnect = 10000;         /* reconnect interval (ms) */
static int nmeacycle1 = 5000;          /* nmea request cycle (ms) */
static int buffsize1 = 32768;         /* input buffer size (bytes) */
static int navmsgsel = 0;             /* navigation mesaage select */
static char proxyaddr[256] = "";          /* http/ntrip proxy */
static int nmeareq1 = 0;             /* nmea request type (0:off,1:lat/lon,2:single) */
static double nmeapos[] = { 0,0 };         /* nmea position (lat/lon) (deg) */
static char rcvcmds[3][1024] = { "" };    /* receiver commands files */
static char startcmd[1024] = "";        /* start command */
static char stopcmd[1024] = "";        /* stop command */
static int modflgr[256] = { 0 };           /* modified flags of receiver options */
static int modflgs[256] = { 0 };           /* modified flags of system options */
static int moniport = 0;             /* monitor port */
static int keepalive = 0;             /* keep alive flag */
static int fswapmargin = 30;            /* file swap margin (s) */


static char strpath[8][1024] = { rover_data,"",ENTRIP_DATA2,"",RECEIVE_RAW_LOG,RAW_DATA_CLIENT,"","" };
//static char strpath[8][1024] = { rover_data,ENTRIP_DATA,RECEIVE_RAW_LOG,RAW_DATA_CLIENT,"","","","" };
/* stream paths */
static int strfmt[] = {                   /* stream formats STRFMT_RNXCLK */
	STRFMT_RTCM3,STRFMT_RTCM3,STRFMT_RTCM3,STRFMT_RTCM3,SOLF_NMEA
};
gtime_t obs_time; gtime_t ssr_time;
//STRFMT_RTCM3
static int strtype[] = {     /* stream types */
	//STR_SERIAL,STR_NTRIPCLI,STR_FILE,STR_FILE,STR_NONE,STR_NONE,STR_NONE,STR_NONE  
    STR_FILE,STR_NONE,STR_NTRIPCLI,STR_NONE,STR_FILE,STR_TCPSVR,STR_NONE,STR_NONE
};
#define NAVIFILE "rtkrcv.nav"
/* open output file for append -----------------------------------------------*/
static FILE *openfile(const char *outfile)
{
	return !*outfile ? stdout : fopen(outfile, "w");
}

/* update navigation data ----------------------------------------------------*/
static void updatenav(nav_t *nav)
{
	int i, j;
	for (i = 0; i<MAXSAT; i++) for (j = 0; j<NFREQ; j++) {
		nav->lam[i][j] = satwavelen(i + 1, j, nav);
	}
}
/* update glonass frequency channel number in raw data struct ----------------*/
static void updatefcn(rtksvr_t *svr)
{
	int i, j, sat, frq;

	for (i = 0; i<MAXPRNGLO; i++) {
		sat = satno(SYS_GLO, i + 1);

		for (j = 0, frq = -999; j<3; j++) {
			if (svr->raw[j].nav.geph[i].sat != sat) continue;
			frq = svr->raw[j].nav.geph[i].frq;
		}
		if (frq<-7 || frq>6) continue;

		for (j = 0; j<3; j++) {
			if (svr->raw[j].nav.geph[i].sat == sat) continue;
			svr->raw[j].nav.geph[i].sat = sat;
			svr->raw[j].nav.geph[i].frq = frq;
		}
	}
}


/* rtk server thread ---------------------------------------------------------*/
#ifdef WIN32
static DWORD WINAPI rtksvrthread(void *arg)
#else
static void *rtksvrthread(void *arg)
#endif
{

	rtksvr_t *svr = (rtksvr_t *)arg;
	obs_t obs;
	obsd_t data[MAXOBS * 2];
	double tt;
	unsigned int tick, ticknmea;
	unsigned char *p, *q;
	int i, j, n, fobs[3] = { 0 }, cycle, cputime;

	tracet(3, "rtksvrthread:\n");

	svr->state = 1; obs.data = data;
	svr->tick = tickget();
	ticknmea = svr->tick - 1000;

	/* read DCB parameters */
	//	readdcb("", &svr->nav, &stas);

	/* read satellite antenna parameters */
	static pcvs_t pcvss = { 0 };        /* receiver antenna parameters */
	static pcvs_t pcvsr = { 0 };        /* satellite antenna parameters */
	readpcv("igs14.atx", &pcvss);

	//-----------------------精密轨道及精密种差文本--------------------
	if (svr->rtk.opt.sateph == 1)
	{
//        readsp3(SP3_FILE, &svr->nav, 0);
//        readrnxc(CLK_FILE, &svr->nav);
        readsp3(inputfilePath[0], &svr->nav, 0);
        readrnxc(inputfilePath[1], &svr->nav);
	}

	for (cycle = 0; svr->state; cycle++)
	{
		tick = tickget();
		for (i = 0; i < 4; i++)
		{

			p = svr->buff[i] + svr->nb[i];
			q = svr->buff[i] + svr->buffsize;

			/* read receiver raw/rtcm data from input stream */
			if ((n = strread(svr->stream + i, p, q - p)) <= 0) {
				continue;
			}
			if (i == 0) {
                //strwrite(svr->stream + i + 4, p, n);
				strwrite(svr->stream + i + 5, p, n);
			}
			svr->nb[i] += n;

			/* save peek buffer */
			rtksvrlock(svr);
			n = n < svr->buffsize - svr->npb[i] ? n : svr->buffsize - svr->npb[i];
			memcpy(svr->pbuf[i] + svr->npb[i], p, n);
			svr->npb[i] += n;
			rtksvrunlock(svr);
		}
		//-----------解RTCM3原始数据，返回GNSS观测值组数(一组为一秒)---------------
		for (i = 0; i < 4; i++)
		{
			fobs[i] = decoderaw(svr, i);//解码rtcm data and ssr  返回每次从流里面读取以采样间隔为单位的组数
		}
		//------------------------------------------------------------------


		/* read pole tide loading parameters */
		/*	if (!readerp(ERP_FILE, &svr->nav.erp))
			{
			printf("ERROR: no erp data %s\n");
			}*/

		/* read ocean tide loading parameters */
		//readotl(&prcopt1, BLQ_FILE, &stas);

		//read igs antex only once, and save the elements in 'pcvss'
		/* set antenna paramters */
		setpcv(svr,svr->rtcm[0].obs.data[0].time, &prcopt1, &svr->nav, &pcvss, &pcvss, &stas);
		for (i = 0; i<fobs[0]; i++)
		{ /* for each rover observation data */
			obs.n = 0;
			for (j = 0; j<svr->obs[0][i].n&&obs.n<MAXOBS * 2; j++) {
				obs.data[obs.n++] = svr->obs[0][i].data[j];
			}
			/* carrier phase bias correction */
			if (!strstr(svr->rtk.opt.pppopt, "-DIS_FCB")) {
				corr_phase_bias(obs.data, obs.n, &svr->nav);
			}
			/* rtk positioning */
			rtksvrlock(svr);
			rtkpos(&svr->rtk, obs.data, obs.n, &svr->nav);
			rtksvrunlock(svr);

			if (svr->rtk.sol.stat != SOLQ_NONE) {

				/* adjust current time */
				tt = (int)(tickget() - tick) / 1000.0 + DTTOL;
				timeset(gpst2utc(timeadd(svr->rtk.sol.time, tt)));

				/* write solution */
				//writesol(svr, i);
			}
			/* if cpu overload, inclement obs outage counter and break */
			if ((int)(tickget() - tick) >= svr->cycle)
			{
				svr->prcout += fobs[0] - i - 1;
#if 0 /* omitted v.2.4.1 */
				break;
#endif
			}
		}
		//---------------end for position---------------------------------

		/* send null solution if no solution (1hz) */
		if (svr->rtk.sol.stat == SOLQ_NONE && cycle % (1000 / svr->cycle) == 0) {
			writesol(svr, 0);
		}
		/* send nmea request to base/nrtk input stream */
		if (svr->nmeacycle>0 && (int)(tick - ticknmea) >= svr->nmeacycle) {
			if (svr->stream[1].state == 1) {
				if (svr->nmeareq == 1) {
					strsendnmea(svr->stream + 1, svr->nmeapos);
				}
				else if (svr->nmeareq == 2 && norm(svr->rtk.sol.rr, 3)>0.0) {
					strsendnmea(svr->stream + 1, svr->rtk.sol.rr);
				}
			}
			ticknmea = tick;
		}
		if ((cputime = (int)(tickget() - tick))>0) svr->cputime = cputime;

		/* sleep until next cycle */
		sleepms(svr->cycle - cputime);
	}

	for (i = 0; i<MAXSTRRTK; i++) strclose(svr->stream + i);
	for (i = 0; i<4; i++) {
		svr->nb[i] = svr->npb[i] = 0;
		free(svr->buff[i]); svr->buff[i] = NULL;
		free(svr->pbuf[i]); svr->pbuf[i] = NULL;
		//	free_raw(svr->raw + i);
		free_rtcm(svr->rtcm + i);
	}
	for (i = 0; i<2; i++) {
		svr->nsb[i] = 0;
		free(svr->sbuf[i]); svr->sbuf[i] = NULL;
	}
	return 0;
}
/* initialize rtk server -------------------------------------------------------
 * initialize rtk server
 * args   : rtksvr_t *svr    IO rtk server
 * return : status (0:error,1:ok)
 *-----------------------------------------------------------------------------*/
extern int rtksvrinit(rtksvr_t *svr)
{
	gtime_t time0 = { 0 };
	sol_t  sol0 = { { 0 } };
	eph_t  eph0 = { 0,-1,-1 };
	geph_t geph0 = { 0,-1 };
	seph_t seph0 = { 0 };
	int i, j;

	tracet(3, "rtksvrinit:\n");

	svr->state = svr->cycle = svr->nmeacycle = svr->nmeareq = 0;
	for (i = 0; i<3; i++) svr->nmeapos[i] = 0.0;
	svr->buffsize = 0;
	for (i = 0; i<4; i++) svr->format[i] = 0;
	for (i = 0; i<2; i++) svr->solopt[i] = solopt_default;
	svr->navsel = svr->nsbs = svr->nsol = 0;
   // rtkinit(&svr->rtk, &prcopt_default);
    rtkinit(&svr->rtk, &prcopt1);
	for (i = 0; i<4; i++) svr->nb[i] = 0;
	for (i = 0; i<2; i++) svr->nsb[i] = 0;
	for (i = 0; i<4; i++) svr->npb[i] = 0;
	for (i = 0; i<4; i++) svr->buff[i] = NULL;
	for (i = 0; i<2; i++) svr->sbuf[i] = NULL;
	for (i = 0; i<4; i++) svr->pbuf[i] = NULL;
	for (i = 0; i<MAXSOLBUF; i++) svr->solbuf[i] = sol0;
	for (i = 0; i<3; i++) for (j = 0; j<10; j++) svr->nmsg[i][j] = 0;
	for (i = 0; i<3; i++) svr->ftime[i] = time0;
	for (i = 0; i<3; i++) svr->files[i][0] = '\0';
	svr->moni = NULL;
	svr->tick = 0;
	svr->thread = 0;
	svr->cputime = svr->prcout = 0;

	if (!(svr->nav.eph = (eph_t  *)malloc(sizeof(eph_t)*MAXSAT * 2)) ||
			!(svr->nav.geph = (geph_t *)malloc(sizeof(geph_t)*NSATGLO * 2)) ||
			!(svr->nav.seph = (seph_t *)malloc(sizeof(seph_t)*NSATSBS * 2))) {
		tracet(1, "rtksvrinit: malloc error\n");
		return 0;
	}
	for (i = 0; i<MAXSAT * 2; i++) svr->nav.eph[i] = eph0;
	for (i = 0; i<NSATGLO * 2; i++) svr->nav.geph[i] = geph0;
	for (i = 0; i<NSATSBS * 2; i++) svr->nav.seph[i] = seph0;
	svr->nav.n = MAXSAT * 2;
	svr->nav.ng = NSATGLO * 2;
	svr->nav.ns = NSATSBS * 2;

	for (i = 0; i<3; i++) for (j = 0; j<MAXOBSBUF; j++) {
		if (!(svr->obs[i][j].data = (obsd_t *)malloc(sizeof(obsd_t)*MAXOBS))) {
			tracet(1, "rtksvrinit: malloc error\n");
			return 0;
		}
	}
	for (i = 0; i<3; i++) {
		memset(svr->raw + i, 0, sizeof(raw_t));
		memset(svr->rtcm + i, 0, sizeof(rtcm_t));
	}
	for (i = 0; i<MAXSTRRTK; i++) strinit(svr->stream + i);

	for (i = 0; i<MAXSAT; i++) {
		svr->sFlag[i].sys = satsys(i + 1, &svr->sFlag[i].prn);
		satno2id(i + 1, svr->sFlag[i].id);
	}

	initlock(&svr->lock);

	return 1;
}
/* free rtk server -------------------------------------------------------------
 * free rtk server
 * args   : rtksvr_t *svr    IO rtk server
 * return : none
 *-----------------------------------------------------------------------------*/
extern void rtksvrfree(rtksvr_t *svr)
{
	int i, j;

	free(svr->nav.eph);
	free(svr->nav.geph);
	free(svr->nav.seph);
	for (i = 0; i<3; i++) for (j = 0; j<MAXOBSBUF; j++) {
		free(svr->obs[i][j].data);
	}
}
/* lock/unlock rtk server ------------------------------------------------------
 * lock/unlock rtk server
 * args   : rtksvr_t *svr    IO rtk server
 * return : status (1:ok 0:error)
 *-----------------------------------------------------------------------------*/
extern void rtksvrlock(rtksvr_t *svr) { lock(&svr->lock); }
extern void rtksvrunlock(rtksvr_t *svr) { unlock(&svr->lock); }

/* start rtk server ------------------------------------------------------------
 * start rtk server thread
 * args   : rtksvr_t *svr    IO rtk server
 *          int     cycle    I  server cycle (ms)
 *          int     buffsize I  input buffer size (bytes)
 *          int     *strs    I  stream types (STR_???)
 *                              types[0]=input stream rover
 *                              types[1]=input stream base station
 *                              types[2]=input stream correction
 *                              types[3]=output stream solution 1
 *                              types[4]=output stream solution 2
 *                              types[5]=log stream rover
 *                              types[6]=log stream base station
 *                              types[7]=log stream correction
 *          char    *paths   I  input stream paths
 *          int     *format  I  input stream formats (STRFMT_???)
 *                              format[0]=input stream rover
 *                              format[1]=input stream base station
 *                              format[2]=input stream correction
 *          int     navsel   I  navigation message select
 *                              (0:rover,1:base,2:ephem,3:all)
 *          char    **cmds   I  input stream start commands
 *                              cmds[0]=input stream rover (NULL: no command)
 *                              cmds[1]=input stream base (NULL: no command)
 *                              cmds[2]=input stream corr (NULL: no command)
 *          char    **rcvopts I receiver options
 *                              rcvopt[0]=receiver option rover
 *                              rcvopt[1]=receiver option base
 *                              rcvopt[2]=receiver option corr
 *          int     nmeacycle I nmea request cycle (ms) (0:no request)
 *          int     nmeareq  I  nmea request type (0:no,1:base pos,2:single sol)
 *          double *nmeapos  I  transmitted nmea position (ecef) (m)
 *          prcopt_t *prcopt I  rtk processing options
 *          solopt_t *solopt I  solution options
 *                              solopt[0]=solution 1 options
 *                              solopt[1]=solution 2 options
 *          stream_t *moni   I  monitor stream (NULL: not used)
 * return : status (1:ok 0:error)
 *-----------------------------------------------------------------------------*/
//extern int rtksvrstart(rtksvr_t *svr, int cycle, int buffsize, int *strs,
//                       char **paths, int *formats, int navsel, char **cmds,
//                       char **rcvopts, int nmeacycle, int nmeareq,
//                       const double *nmeapos, prcopt_t *prcopt,
//                       solopt_t *solopt, stream_t *moni)
//int main(int argc,char **argv)
fileout_t file_out;
FILE* outFp[50];   //输出文件指针
extern int startppp(int *strs,char streamPaths[8][1024],char inputFilePaths[2][1024],char outResultPath[1024])
{

    for(int i=0;i<2;i++)
        strcpy(inputfilePath[i],inputFilePaths[i]);
	//rtksvr函数定义或初始化
	char file[1024] = "";
	int i;
	rtksvrinit(&svr1);
	strinit(&moni1);
	resetsysopts();
    //getsysopts(&prcopt1, solopt1, &filopt1);
    getsysopts(solopt1, &filopt1);
	//暂时还不知道需不需要
	if (!readnav(NAVIFILE, &svr1.nav))
	{
        ////fprintf(stderr, "no nav data:%s\n", NAVIFILE);
	}

	//读输入命令行

	/*for (i = 1; i<argc; i++) {
	  if (!strcmp(argv[i], "-o") && i + 1<argc) strcpy(file, argv[++i]);
	  }*/

	//startsvr函数定义或变量初始化
//	char *paths1[] = {
//		strpath[0],strpath[1],strpath[2],strpath[3],strpath[4],strpath[5],
//		strpath[6],strpath[7]
//	};
	/*for (i = 3; i < 8; i++)
	  {
	  if (strtype[i] == STR_FILE && !confwrite(vt, strpath[i])) return 0;
	  }*/
	double pos[3], npos[3];//ck
	pos[0] = nmeapos[0] * D2R;
	pos[1] = nmeapos[1] * D2R;
	pos[2] = 0.0;
	pos2ecef(pos, npos);
	gtime_t time, time0 = { 0 };
	char *ropts[] = { "","","","" };
	char *cmd[] = { NULL,NULL,NULL };
	int stropt[8] = { 0 };
	stropt[0] = timeout;
	stropt[1] = reconnect;
	stropt[2] = 1000;
	stropt[3] = buffsize1;
	stropt[4] = fswapmargin;
	strsetopt(stropt);
	if (strfmt[2] == 8) strfmt[2] = STRFMT_SP3;
	solopt1[0].posf = strfmt[3];
	solopt1[1].posf = strfmt[4];

	//本函数变量定义
	rtksvr_t *svr = &svr1;
	stream_t *moni = &moni1;
	prcopt_t *prcopt = &prcopt1;
	solopt_t *solopt = &solopt1[0];
	int cycle = svrcycle, navsel = navmsgsel, buffsize = buffsize1;
//	int *strs = strtype;
//	char **paths = paths1;
	int *formats = strfmt;
	char **rcvopts = ropts;
	int nmeacycle = nmeacycle1;
	int nmeareq = nmeareq1;
	const double *nmeapos = &npos[0];
	int j, rw;
	char **cmds = &cmd[0];

	for (i = 0; i<MAXOUTFILE; i++) file_out.outf[i] = NULL;
	for (i = 0; i<MAXOUTFILE; i++) {
		if (!(file_out.outf[i] = (char *)malloc(MAXSTRPATH))) {
			for (i--; i >= 0; i--) free(file_out.outf[i]);
			return 0;
		}
		else
			file_out.outf[i][0] = '\0';
    }
    char outdir[1024]="./result";
    if(strlen(outResultPath))
        strcpy(outdir,outResultPath);
	for (i = 0; i < 50; i++) {
		if (strlen(outfile_type[i]) > 0)
            sprintf(file_out.outf[i], "%s/%s.%s", outdir, OutputFileName, outfile_type[i]);
	}
	for (i = 0; i < MAXOUTFILE; i++) {
		if (file_out.outf[i] && strlen(file_out.outf[i]) > 2)
			outFp[i] = openfile(file_out.outf[i]);
		else
			outFp[i] = NULL;
	}
	tracet(3, "rtksvrstart: cycle=%d buffsize=%d navsel=%d nmeacycle=%d nmeareq=%d\n",
			cycle, buffsize, navsel, nmeacycle, nmeareq);

	if (svr->state) return 0;

	strinitcom();
	svr->cycle = cycle>1 ? cycle : 1;
	svr->nmeacycle = nmeacycle>1000 ? nmeacycle : 1000;
	svr->nmeareq = nmeareq;
	for (i = 0; i<3; i++) svr->nmeapos[i] = nmeapos[i];
	svr->buffsize = buffsize>4096 ? buffsize : 4096;
	for (i = 0; i<3; i++) svr->format[i] = formats[i];
	svr->navsel = navsel;
	svr->nsbs = 0;
	svr->nsol = 0;
	svr->prcout = 0;
	rtkfree(&svr->rtk);
	rtkinit(&svr->rtk, prcopt);

	for (i = 0; i<4; i++) { /* input/log streams */
		svr->nb[i] = svr->npb[i] = 0;
		if (!(svr->buff[i] = (unsigned char *)malloc(buffsize)) ||
				!(svr->pbuf[i] = (unsigned char *)malloc(buffsize))) {
			tracet(1, "rtksvrstart: malloc error\n");
			return 0;
		}
		for (j = 0; j<10; j++) svr->nmsg[i][j] = 0;
		for (j = 0; j<MAXOBSBUF; j++) svr->obs[i][j].n = 0;

		/* initialize receiver raw and rtcm control */
		//init_raw(svr->raw + i);
		init_rtcm(svr->rtcm + i);

		/* set receiver and rtcm option */
		strcpy(svr->raw[i].opt, rcvopts[i]);
		strcpy(svr->rtcm[i].opt, rcvopts[i]);

	}
	for (i = 0; i<2; i++) { /* output peek buffer */
		if (!(svr->sbuf[i] = (unsigned char *)malloc(buffsize))) {
			tracet(1, "rtksvrstart: malloc error\n");
			return 0;
		}
	}
	svr->nav.n = MAXSAT * 2;
	/* set solution options */
	for (i = 0; i<2; i++) {
		svr->solopt[i] = solopt[i];
	}
	/* set base station position */
	for (i = 0; i<6; i++) {
		svr->rtk.rb[i] = i<3 ? prcopt->rb[i] : 0.0;
	}
	/* update navigation data */
	for (i = 0; i<MAXSAT * 2; i++) svr->nav.eph[i].ttr = time0;
	for (i = 0; i<NSATGLO * 2; i++) svr->nav.geph[i].tof = time0;
	for (i = 0; i<NSATSBS * 2; i++) svr->nav.seph[i].tof = time0;
	updatenav(&svr->nav);

	/* set monitor stream */
	//svr->moni=moni;
	/* open input streams */
	for (i = 0; i<6; i++) {
		rw = i < 4 ? STR_MODE_R : STR_MODE_W;
		if (strs[i] != STR_FILE) rw |= STR_MODE_W;
        if (!stropen(svr->stream + i, strs[i], rw, streamPaths[i]))
		{
			for (i--; i >= 0; i--) strclose(svr->stream + i);
			return 0;
		}
		/* set initial time for rtcm and raw */
		if (i<4) {
			time = utc2gpst(timeget());
			svr->raw[i].time = strs[i] == STR_FILE ? strgettime(svr->stream + i) : time;
			svr->rtcm[i].time = strs[i] == STR_FILE ? strgettime(svr->stream + i) : time;
		}
	}
	/* sync input streams */
	//strsync(svr->stream, svr->stream + 1);
	//strsync(svr->stream, svr->stream + 2);

	// write start commands to input streams */
	/*for (i=0;i<3;i++) {
	  if (cmds[i]) strsendcmd(svr->stream+i,cmds[i]);
	  }*/
	// write solution header to solution streams */
	//for (i = 3; i<5; i++) {
	//	writesolhead(svr->stream + i, svr->solopt + i - 3);
	//}
	//1.表示线程内核对象的安全属性，一般传入NULL表示使用默认设置。
	//2.表示线程栈空间大小。传入0表示使用默认大小（1MB）
	//3.表示新线程所执行的线程函数地址，多个线程可以使用同一个函数地址。
	//4.是传给线程函数的参数
	//5.指定额外的标志来控制线程的创建，为0表示线程创建之后立即就可以进行调度，
	//如果为CREATE_SUSPENDED则表示线程创建后暂停运行，这样它就无法调度，直到调用ResumeThread()
	//6.将返回线程的ID号，传入NULL表示不需要返回该线程ID号。
	/* create rtk server thread */
#ifdef WIN32
	if (!(svr->thread = CreateThread(NULL, 0, rtksvrthread, svr, 0, NULL))) {
#else
		if (pthread_create(&svr->thread,NULL,rtksvrthread,svr)) {
#endif
			for (i = 0; i<MAXSTRRTK; i++) strclose(svr->stream + i);
			return 0;
		}
		//	CloseHandle(svr->thread);
#ifdef WIN32
		WaitForSingleObject(svr->thread, 1000);
		CloseHandle(svr->thread);
#else
		pthread_join(svr->thread, NULL);
#endif
        //Sleep(9000000000000000000);
		//pause();
		return 1;
	}

	/* close output/log stream -----------------------------------------------------
	 * close output/log stream
	 * args   : rtksvr_t *svr    IO rtk server
	 *          int     index    I  output/log stream index
	 *                              (3:solution 1,4:solution 2,5:log rover,
	 *                               6:log base station,7:log correction)
	 * return : none
	 *-----------------------------------------------------------------------------*/
	extern void rtksvrclosestr(rtksvr_t *svr, int index)
	{
		tracet(3, "rtksvrclosestr: index=%d\n", index);

		if (index<3 || index>7 || !svr->state) return;

		rtksvrlock(svr);

		strclose(svr->stream + index);

		rtksvrunlock(svr);
	}
	/* get observation data status -------------------------------------------------
	 * get current observation data status
	 * args   : rtksvr_t *svr    I  rtk server
	 *          int     rcv      I  receiver (0:rover,1:base,2:ephem)
	 *          gtime_t *time    O  time of observation data
	 *          int     *sat     O  satellite prn numbers
	 *          double  *az      O  satellite azimuth angles (rad)
	 *          double  *el      O  satellite elevation angles (rad)
	 *          int     **snr    O  satellite snr for each freq (dBHz)
	 *                              snr[i][j] = sat i freq j snr
	 *          int     *vsat    O  valid satellite flag
	 * return : number of satellites
	 *-----------------------------------------------------------------------------*/
    extern int rtksvrostat(rtksvr_t *svr, int rcv, gtime_t *time, int *sat,
                           double *az, double *el, int **snr, int *vsat)
    {
        int i,j,ns;

        tracet(4,"rtksvrostat: rcv=%d\n",rcv);

        if (!svr->state) return 0;
        rtksvrlock(svr);
        ns=svr->obs[rcv][0].n;
        if (ns>0) {
            *time=svr->obs[rcv][0].data[0].time;
        }
        for (i=0;i<ns;i++) {
            sat [i]=svr->obs[rcv][0].data[i].sat;
            az  [i]=svr->rtk.ssat[sat[i]-1].azel[0];
            el  [i]=svr->rtk.ssat[sat[i]-1].azel[1];
            for (j=0;j<NFREQ;j++) {
                snr[i][j]=(int)(svr->obs[rcv][0].data[i].SNR[j]*0.25);
            }

            if (svr->rtk.sol.stat==SOLQ_NONE||svr->rtk.sol.stat==SOLQ_SINGLE)
            {
                vsat[i]=svr->rtk.ssat[sat[i]-1].vs;
            }
            else {
                vsat[i]=svr->rtk.ssat[sat[i]-1].vsat[0];
            }
        }
        rtksvrunlock(svr);
        return ns;
    }
	/* get stream status -----------------------------------------------------------
	 * get current stream status
	 * args   : rtksvr_t *svr    I  rtk server
	 *          int     *sstat   O  status of streams
	 *          char    *msg     O  status messages
	 * return : none
	 *-----------------------------------------------------------------------------*/
	extern void rtksvrsstat(rtksvr_t *svr, int *sstat, char *msg)
	{
		int i;
		char s[MAXSTRMSG], *p = msg;

		tracet(4, "rtksvrsstat:\n");

		rtksvrlock(svr);
		for (i = 0; i<MAXSTRRTK; i++) {
			sstat[i] = strstat(svr->stream + i, s);
			if (*s) p += sprintf(p, "(%d) %s ", i + 1, s);
		}
		rtksvrunlock(svr);
	}
