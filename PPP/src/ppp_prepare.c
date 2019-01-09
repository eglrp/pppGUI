#include"rtklib.h"
extern gtime_t obs_time; 
extern gtime_t ssr_time;


/* carrier-phase bias (fcb) correction ---------------------------------------*/
extern void corr_phase_bias(obsd_t *obs, int n, const nav_t *nav)
{
	double lam;
	int i, j, code;

	for (i = 0; i<n; i++) for (j = 0; j<NFREQ; j++) {

		if (!(code = obs[i].code[j])) continue;
		if ((lam = nav->lam[obs[i].sat - 1][j]) == 0.0) continue;

		/* correct phase bias (cyc) */
		obs[i].L[j] -= nav->ssr[obs[i].sat - 1].pbias[code - 1] / lam;
	}
}

/* set antenna parameters ----------------------------------------------------*/
extern void setpcv(rtksvr_t *svr,gtime_t time, prcopt_t *popt, nav_t *nav, const pcvs_t *pcvs,
	const pcvs_t *pcvr, const sta_t *sta)
{
	pcv_t *pcv;
	double pos[3], del[3], dt;
	int i, j, k = 0 ,sys;
	char id[64];
	/* set satellite antenna parameters */
	for (i = 0; i<MAXSAT; i++) {
		sys = svr->sFlag[i].sys;
		if (!(sys&popt->navsys)) continue;
		if (!(pcv = searchpcv(i + 1, "", time, pcvs))) {
			satno2id(i + 1, id);
			continue;
		}
		nav->pcvs[i] = *pcv;

		if (pcv->dzen == 0.0) j = 10;
		else j = myRound((pcv->zen2 - pcv->zen1) / pcv->dzen);

		if (sys == SYS_GPS) k = 0;
		else if (sys == SYS_GLO) k = 0 + 1 * NFREQ;
		else if (sys == SYS_CMP) k = 0 + 2 * NFREQ;
		else if (sys == SYS_GAL) k = 0 + 3 * NFREQ;
		//double dt=norm(pcv->var[0],j);
		dt = norm(pcv->var[k], j);
		if (dt <= 0.0001) {
		//printf( "ATTENTION! PRELIMINARY PHASE CENTER CORRECTIONS!\n");
			//outDebug(OUTWIN, OUTFIL, 0);
		}
	}
	for (i = 0; i<1; i++) {
		if (!strcmp(&popt->anttype[0][0], "*")) { /* set by station parameters */
			strcpy(&popt->anttype[0][0], sta[i].antdes);
			if (sta[i].deltype == 1) { /* xyz */
				if (norm(sta[i].pos, 3)>0.0) {
					ecef2pos(sta[i].pos, pos);
					ecef2enu(pos, sta[i].del, del);
					for (j = 0; j<3; j++) popt->antdel[0][j] = del[j];
				}
			}
			else { /* enu */
				//for (j = 0; j<3; j++) popt->antdel[j] = stas[i].del[j];
			}
		}
		if (!(pcv = searchpcv(0, &popt->anttype[0][0], time, pcvr))) {
			*popt->anttype[0] = '\0';
			continue;
		}
		strcpy(&popt->anttype[0][0], pcv->type);
		popt->pcvr[0] = *pcv;
	}
}
/* read ocean tide loading parameters */
extern void readotl(prcopt_t *popt, const char *file, const sta_t *sta)
{
	int i, mode = PMODE_DGPS <= popt->mode&&popt->mode <= PMODE_FIXED;

	for (i = 0; i<(mode ? 2 : 1); i++) {
		readblq(file, sta[i].name, popt->odisp[i]);
	}
}

/* decode download file ------------------------------------------------------*/
extern void decodefile(rtksvr_t *svr, int index)
{
	nav_t nav = { 0 };
	//  char file[1024]="igr19834.sp3";
	char file[1024] = "igr19856.sp3";
	int nb;

	tracet(4, "decodefile: index=%d\n", index);

	rtksvrlock(svr);
	rtksvrunlock(svr);

	if (svr->format[index] == STRFMT_SP3) { /* precise ephemeris */

											/* read sp3 precise ephemeris */
		readsp3(file, &nav, 0);
		if (nav.ne <= 0) {
			tracet(1, "sp3 file read error: %s\n", file);
			return;
		}
		/* update precise ephemeris */
		rtksvrlock(svr);

		if (svr->nav.peph)
			free(svr->nav.peph);
		svr->nav.ne = svr->nav.nemax = nav.ne;
		svr->nav.peph = nav.peph;
		svr->ftime[index] = utc2gpst(timeget());
		strcpy(svr->files[index], file);

		rtksvrunlock(svr);
	}
	else if (svr->format[index] == STRFMT_RNXCLK) { /* precise clock */

													/* read rinex clock */
		if (readrnxc("igr19856.clk", &nav) <= 0) {
			tracet(1, "rinex clock file read error: %s\n", file);
			return;
		}
		/* update precise clock */
		rtksvrlock(svr);

		if (svr->nav.pclk) free(svr->nav.pclk);
		svr->nav.nc = svr->nav.ncmax = nav.nc;
		svr->nav.pclk = nav.pclk;
		svr->ftime[index] = utc2gpst(timeget());
		strcpy(svr->files[index], file);

		rtksvrunlock(svr);
	}
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

/* update rtk server struct --------------------------------------------------*/
static void updatesvr(rtksvr_t *svr, int ret, obs_t *obs, nav_t *nav, int sat,
	int index, int iobs)
{
	eph_t *eph1, *eph2, *eph3;
	geph_t *geph1, *geph2, *geph3;
	gtime_t tof;
	double pos[3], del[3] = { 0 }, dr[3];
	int i, n = 0, prn, sbssat = svr->rtk.opt.sbassatsel, sys, iode;

	tracet(4, "updatesvr: ret=%d sat=%2d index=%d\n", ret, sat, index);

	if (ret == 1) { /* observation data */ 
		if (iobs<MAXOBSBUF) {
            for (i = 0; i<obs->n; i++)
            {
                if (svr->rtk.opt.exsats[obs->data[i].sat - 1] == 1 ||!(satsys(obs->data[i].sat, NULL)&svr->rtk.opt.navsys)) continue;
				svr->obs[index][iobs].data[n] = obs->data[i];
				obs_time = svr->obs[index][iobs].data[n].time;
				svr->obs[index][iobs].data[n++].rcv = index + 1;
			}
			svr->obs[index][iobs].n = n;
			sortobs(&svr->obs[index][iobs]);
		}
		svr->nmsg[index][0]++;
	}

	else if (ret == 2) { /* ephemeris */
		if (satsys(sat, &prn) != SYS_GLO) {
			if (!svr->navsel || svr->navsel == index + 1) {
				eph1 = nav->eph + sat - 1;
				eph2 = svr->nav.eph + sat - 1;
				eph3 = svr->nav.eph + sat - 1 + MAXSAT;
				if (eph2->ttr.time == 0 ||
					(eph1->iode != eph3->iode&&eph1->iode != eph2->iode) ||
					(timediff(eph1->toe, eph3->toe) != 0.0&&
						timediff(eph1->toe, eph2->toe) != 0.0)) {
					*eph3 = *eph2;
					*eph2 = *eph1;
					updatenav(&svr->nav);
				}
			}
			svr->nmsg[index][1]++;

			//("output eph clk:%d", svr->nav.eph[0]);
		}
		else {
			if (!svr->navsel || svr->navsel == index + 1) {
				geph1 = nav->geph + prn - 1;
				geph2 = svr->nav.geph + prn - 1;
				geph3 = svr->nav.geph + prn - 1 + MAXPRNGLO;
				if (geph2->tof.time == 0 ||
					(geph1->iode != geph3->iode&&geph1->iode != geph2->iode)) {
					*geph3 = *geph2;
					*geph2 = *geph1;
					updatenav(&svr->nav);
					//updatefcn(svr);
				}
			}
			svr->nmsg[index][6]++;
		}
	}

	else if (ret == 9) { /* ion/utc parameters */
		if (svr->navsel == index || svr->navsel >= 3) {
			for (i = 0; i<8; i++) svr->nav.ion_gps[i] = nav->ion_gps[i];
			for (i = 0; i<4; i++) svr->nav.utc_gps[i] = nav->utc_gps[i];
			for (i = 0; i<4; i++) svr->nav.ion_gal[i] = nav->ion_gal[i];
			for (i = 0; i<4; i++) svr->nav.utc_gal[i] = nav->utc_gal[i];
			for (i = 0; i<8; i++) svr->nav.ion_qzs[i] = nav->ion_qzs[i];
			for (i = 0; i<4; i++) svr->nav.utc_qzs[i] = nav->utc_qzs[i];
			svr->nav.leaps = nav->leaps;
		}
		svr->nmsg[index][2]++;
	}
	else if (ret == 5) { /* antenna postion parameters */
		if (svr->rtk.opt.refpos == 4 && index == 1) {
			for (i = 0; i<3; i++) {
				svr->rtk.rb[i] = svr->rtcm[1].sta.pos[i];
			}
			/* antenna delta */
			ecef2pos(svr->rtk.rb, pos);
			if (svr->rtcm[1].sta.deltype) { /* xyz */
				del[2] = svr->rtcm[1].sta.hgt;
				enu2ecef(pos, del, dr);
				for (i = 0; i<3; i++) {
					svr->rtk.rb[i] += svr->rtcm[1].sta.del[i] + dr[i];
				}
			}
			else { /* enu */
				enu2ecef(pos, svr->rtcm[1].sta.del, dr);
				for (i = 0; i<3; i++) {
					svr->rtk.rb[i] += dr[i];
				}
			}
		}
		svr->nmsg[index][4]++;
	}
	else if (ret == 7) { /* dgps correction */
		svr->nmsg[index][5]++;
	}
	else if (ret == 10) { /* ssr message */
		for (i = 0; i<MAXSAT; i++) {
			if (!svr->rtcm[index].ssr[i].update) continue;

			/* check consistency between iods of orbit and clock */
			if (svr->rtcm[index].ssr[i].iod[0] !=
				svr->rtcm[index].ssr[i].iod[1]) continue;

			svr->rtcm[index].ssr[i].update = 0;

			iode = svr->rtcm[index].ssr[i].iode;
			sys = satsys(i + 1, &prn);

			//if (sys == SYS_CMP)
			//  printf("sys=%d ssr iode=%d\n", sys, iode);

			/* check corresponding ephemeris exists */
			if (sys == SYS_GPS || sys == SYS_GAL || sys == SYS_QZS)
			{
				if (svr->nav.eph[i].iode != iode &&
					svr->nav.eph[i + MAXSAT].iode != iode) {
					continue;
				}
			}
			//else if (sys == SYS_CMP)
			//{
			//if (svr->nav.eph[i].iode != iode &&
			//svr->nav.eph[i + MAXSAT].iode != iode) {
			//continue;
			//}
			//}
			else if (sys == SYS_GLO) {
				if (svr->nav.geph[prn - 1].iode != iode &&
					svr->nav.geph[prn - 1 + MAXPRNGLO].iode != iode) {
					continue;
				}
			}
			if (svr->nav.ssr[i].t0[0].time != 0) {
				svr->rtk.sol.ssr_time = svr->nav.ssr[i].t0[0];
				ssr_time = svr->nav.ssr[i].t0[0];
			}

			svr->nav.ssr[i] = svr->rtcm[index].ssr[i];
			//printf("svr->nav.ssr[%d]=%d\n", i, i);
		}
		svr->nmsg[index][7]++;
	}

	else if (ret == -1) { /* error */
		svr->nmsg[index][9]++;
	}
}


/* decode receiver raw/rtcm data ---------------------------------------------*/
extern int decoderaw(rtksvr_t *svr, int index)
{
	obs_t *obs=NULL;
	nav_t *nav=NULL;
	int i, ret, sat, fobs = 0;

	tracet(4, "decoderaw: index=%d\n", index);

	rtksvrlock(svr);

	for (i = 0; i<svr->nb[index]; i++)
	{
		svr->rtcm[1].week = svr->rtcm[2].week = svr->rtcm[3].week = svr->rtcm[0].week;
		if (svr->format[index] == STRFMT_RTCM3) {
			ret = input_rtcm3(svr->rtcm + index, svr->buff[index][i]);
			obs = &svr->rtcm[index].obs;
			nav = &svr->rtcm[index].nav;
			sat = svr->rtcm[index].ephsat;

           // printf("obs.n=%d\n",obs->n);
		}
#if 0 /* record for receiving tick */
		if (ret == 1) {
			trace(0, "%d %10d T=%s NS=%2d\n", index, tickget(),
				time_str(obs->data[0].time, 0), obs->n);
		}
#endif
		/* update rtk server */
        if (ret>0)
			updatesvr(svr, ret, obs, nav, sat, index, fobs);

		/* observation data received */ //fobs表示从流里面读到的观测值组数
		if (ret == 1) {
			if (fobs<MAXOBSBUF)
				fobs++;
			else
				svr->prcout++;
		}

	}
	svr->nb[index] = 0;

	//time_t time =svr ->rtk->sol.time;
	rtksvrunlock(svr);

	return fobs;
}
/* open output/log stream ------------------------------------------------------
* open output/log stream
* args   : rtksvr_t *svr    IO rtk server
*          int     index    I  output/log stream index
*                              (3:solution 1,4:solution 2,5:log rover,
*                               6:log base station,7:log correction)
*          int     str      I  output/log stream types (STR_???)
*          char    *path    I  output/log stream path
*          solopt_t *solopt I  solution options
* return : status (1:ok 0:error)*/

extern int rtksvropenstr(rtksvr_t *svr, int index, int str, const char *path,
	const solopt_t *solopt)
{
	tracet(3, "rtksvropenstr: index=%d str=%d path=%s\n", index, str, path);

	if (index<3 || index>7 || !svr->state) return 0;

	rtksvrlock(svr);

	if (svr->stream[index].state>0) {
		rtksvrunlock(svr);
		return 0;
	}
	if (!stropen(svr->stream + index, str, STR_MODE_W, path)) {
		tracet(2, "stream open error: index=%d\n", index);
		rtksvrunlock(svr);
		return 0;
	}
	if (index <= 4) {
		svr->solopt[index - 3] = *solopt;

		/* write solution header to solution stream */
		writesolhead(svr->stream + index, svr->solopt + index - 3);
	}
	rtksvrunlock(svr);
	return 1;
}
/* stop rtk server -------------------------------------------------------------
* start rtk server thread
* args   : rtksvr_t *svr    IO rtk server
*          char    **cmds   I  input stream stop commands
*                              cmds[0]=input stream rover (NULL: no command)
*                              cmds[1]=input stream base  (NULL: no command)
*                              cmds[2]=input stream ephem (NULL: no command)
* return : none
*-----------------------------------------------------------------------------*/
//extern void rtksvrstop(rtksvr_t *svr, char **cmds)
extern void rtksvrstop(rtksvr_t *svr)
{
	int i;

	tracet(3, "rtksvrstop:\n");

	/* write stop commands to input streams */
	rtksvrlock(svr);
//	for (i = 0; i<3; i++) {
//		if (cmds[i]) strsendcmd(svr->stream + i, cmds[i]);
//	}
	rtksvrunlock(svr);

	/* stop rtk server */
	svr->state = 0;

	/* free rtk server thread */
#ifdef WIN32
	WaitForSingleObject(svr->thread, 10000);
	CloseHandle(svr->thread);
#else
	pthread_join(svr->thread, NULL);
#endif
}
