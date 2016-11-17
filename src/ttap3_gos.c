/** 
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
   \file ttap3_gcg.c
   \brief TTAK.KO-06.0288 Part 3 구현에서 GOS 관련 함수
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ttap3_define.h>
#include <ttap3_config.h>
#include <ttap3_base.h>
#include <ttap3_util.h>
#include <ttap3_process.h>
#include <ttap3_gos.h>

#include <../util/mem.h>

/* data structure for gos internal data */
static struct {
	int gosid;
	int numofgcg;
	int gcgids[_TP3_MAX_GCG];
	int msgseq;
} gosdata;

void
tp3_initgos (int gosid, int *gcgids, int numofgcg) {
	gosdata.gosid = gosid;
	gosdata.numofgcg = numofgcg;
	gosdata.msgseq = 0;

	memcpy ((void *)(&(gosdata.gcgids)), (void *)gcgids, sizeof(int) * numofgcg);

	tp3_initcb ();
	tp3_registcbfunc (TP3_MT_CONNAPPROVAL, tp3_acceptmsg, NULL);
	tp3_registcbfunc (TP3_MT_STATINFO, tp3_defaultconfirm, NULL);
	tp3_registcbfunc (TP3_MT_ENVINFO, tp3_defaultconfirm, NULL);
}

void
tp3_releasegos () {
	;
}

tp3_stat_t
tp3_isok_gos (ptp3_msg_t preq) {
	int i;
	int gcgid = (preq->header).gcgid;

	if ((preq->header).gosid != gosdata.gosid) {
		return TP3_SC_GOS_ERR;
	}
	for (i = 0; i < gosdata.numofgcg; i++) {
		if (gcgid == (gosdata.gcgids)[i]) {
			return TP3_SC_NOERROR;
		} 
	}
	return TP3_SC_GCG_ERR;
}


tp3_stat_t
tp3_acceptmsg (ptp3_msg_t preq, ptp3_msg_t pres, void *data) {
	tp3_stat_t stat = tp3_isok_gos (preq);
	TP3_STAT_NOERROR (tp3_setresponse (preq, pres, stat));
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_defaultconfirm (ptp3_msg_t preq, ptp3_msg_t pres, void *data) {
	tp3_stat_t stat = tp3_isok_gos (preq);
	TP3_STAT_NOERROR (tp3_setresponse (preq, pres, stat));
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_parse_statmsg (ptp3_msg_t preq, tp3_dev_t devtype, int *num, int *arrid, int *arrval, int len) {
	tp3_fieldcode_t fc[3];
	int n;

	if (devtype == TP3_DEV_SENSOR) {
		fc[0] = TP3_FC_NUMOFSENS;
		fc[1] = TP3_FC_ARROFGSID;
		fc[2] = TP3_FC_ARROFSENSSTAT;
	} else if (devtype == TP3_DEV_ACTUATOR) {
		fc[0] = TP3_FC_NUMOFACTS;
		fc[1] = TP3_FC_ARROFGAID;
		fc[2] = TP3_FC_ARROFACTSSTAT;
	} else if (devtype == TP3_DEV_SNODE) {
		fc[0] = TP3_FC_NUMOFSN;
		fc[1] = TP3_FC_ARROFSNID;
		fc[2] = TP3_FC_ARROFSNSTAT;
	} else if (devtype == TP3_DEV_ANODE) {
		fc[0] = TP3_FC_NUMOFAN;
		fc[1] = TP3_FC_ARROFANID;
		fc[2] = TP3_FC_ARROFANSTAT;
	} else {
		TP3_STAT_PRETURN ("devtype is not matched", TP3_SC_NOT_PROPER_DEVTYPE);
	}

	n = tp3_readfield_num (preq, fc[0], num);
	if (*num == 0)
		return TP3_SC_NOERROR;

	TP3_STAT_RETURN (n < 0, TP3_SC_FRAMEFMT_ERR);
	TP3_STAT_RETURN (*num > len, TP3_SC_NOT_ENOUGH_SPACE);
	TP3_STAT_RETURN (*num != tp3_readfield_array (preq, fc[1], arrid, len), TP3_SC_FRAMEFMT_ERR);
	TP3_STAT_RETURN (*num != tp3_readfield_array (preq, fc[2], arrval, len), TP3_SC_FRAMEFMT_ERR);

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_parse_envmsg (ptp3_msg_t preq, tp3_dev_t devtype, int *num, int *arrid, int *arrval, int len) {
	tp3_fieldcode_t fc[3];
	int n;
	if (devtype == TP3_DEV_SENSOR) {
		fc[0] = TP3_FC_NUMOFSENS;
		fc[1] = TP3_FC_ARROFGSID;
		fc[2] = TP3_FC_ARROFSENSVAL;
	} else if (devtype == TP3_DEV_ACTUATOR) {
		fc[0] = TP3_FC_NUMOFACTS;
		fc[1] = TP3_FC_ARROFGAID;
		fc[2] = TP3_FC_ARROFACTSVAL;
	} else {
		TP3_STAT_PRETURN ("devtype is not matched", TP3_SC_NOT_PROPER_DEVTYPE);
	}

	n = tp3_readfield_num (preq, fc[0], num);
	if (*num == 0)
		return TP3_SC_NOERROR;

	TP3_STAT_RETURN (n < 0, TP3_SC_FRAMEFMT_ERR);
	TP3_STAT_RETURN (*num > len, TP3_SC_NOT_ENOUGH_SPACE);
	TP3_STAT_RETURN (*num != tp3_readfield_array (preq, fc[1], arrid, len), TP3_SC_FRAMEFMT_ERR);
	TP3_STAT_RETURN (*num != tp3_readfield_array (preq, fc[2], arrval, len), TP3_SC_FRAMEFMT_ERR);

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_stathistmsg (ptp3_msg_t preq, int gcgid, tp3_hreq_t reqtype, time_t stime, time_t etime) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_STATHIST, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));

	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_HISTREQTYPE, reqtype));
	TP3_STAT_NOERROR (tp3_addfield_timestamp (preq, TP3_FC_SDATETIME, stime));
	TP3_STAT_NOERROR (tp3_addfield_timestamp (preq, TP3_FC_EDATETIME, etime));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_envhistmsg (ptp3_msg_t preq, int gcgid, tp3_hreq_t reqtype, time_t stime, time_t etime) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_ENVHIST, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));

	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_HISTREQTYPE, reqtype));
	TP3_STAT_NOERROR (tp3_addfield_timestamp (preq, TP3_FC_SDATETIME, stime));
	TP3_STAT_NOERROR (tp3_addfield_timestamp (preq, TP3_FC_EDATETIME, etime));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_gcgconfmsg (ptp3_msg_t preq, int gcgid) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_PROFQUERY, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_snodeconfmsg (ptp3_msg_t preq, int gcgid, int snid) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_PROFQUERY, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_SNID, snid));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_anodeconfmsg (ptp3_msg_t preq, int gcgid, int anid) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_PROFQUERY, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_ANID, anid));
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_gcgattrmsg (ptp3_msg_t preq, int gcgid) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_PROFQUERY, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_senattrmsg (ptp3_msg_t preq, int gcgid, int snid, int sid) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_PROFQUERY, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_SNID, snid));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_SID, sid));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_actattrmsg (ptp3_msg_t preq, int gcgid, int anid, int aid) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_PROFQUERY, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_ANID, anid));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_AID, aid));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_senattrupdatemsg (ptp3_msg_t preq, int gcgid, int snid, int sid, tp3_sengathertype_t gathertype, int interval, tp3_senthrsdtype_t thrsdtype, int value) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_PROFUPDATE, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_SNID, snid));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_SID, sid));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_SENGETTYPE, gathertype));
	if (gathertype == TP3_SENGT_PERIOD) {
		TP3_STAT_NOERROR (tp3_addfield_interval (preq, TP3_FC_SENGETINTV, interval));
	} else if (gathertype == TP3_SENGT_EVENT) {
		TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_THRESHOLDTYPE, thrsdtype));
		TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_THRESHOLD, value));
	}

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_actattrupdatemsg (ptp3_msg_t preq, int gcgid, int anid, int aid, tp3_actgathertype_t gathertype, int interval) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_PROFUPDATE, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_ANID, anid));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_AID, aid));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_ACTGETTYPE, gathertype));
	if (gathertype == TP3_ACTGT_PERIOD) {
		TP3_STAT_NOERROR (tp3_addfield_interval (preq, TP3_FC_ACTGETINTV, interval));
	}

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_faultmngmsg (ptp3_msg_t preq, int gcgid, tp3_faultmng_t faultmng) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_FAULTMNG, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_FALUTMETHOD, faultmng));
	
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_actcontrolmsg (ptp3_msg_t preq, int gcgid, int length, int *gaids, int *values) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_ACTCTRL, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFACTS, length));
	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_ARROFGAID, length * sizeof(int), (byte *)gaids));
	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_ARROFACTSVAL, length * sizeof(int), (byte *)values));
	
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_envquerymsg (ptp3_msg_t preq, int gcgid, int numofsens, int *gsids, int numofacts, int *gaids) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_ENVQUERY, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFSENS, numofsens));
	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_ARROFGSID, numofsens * sizeof(int), (byte *)gsids));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFACTS, numofacts));
	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_ARROFGAID, numofacts * sizeof(int), (byte *)gaids));

	return TP3_SC_NOERROR;

}
tp3_stat_t
tp3_statquerymsg (ptp3_msg_t preq, int gcgid, int numofsn, int *snids, int numofsens, int *gsids, int numofan, int *anids, int numofacts, int *gaids) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_STATQUERY, TP3_TT_OS_CG_REQ, gcgid, gosdata.gosid, gosdata.msgseq++));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFSN, numofsn));
	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_ARROFSNID, numofsn * sizeof(int), (byte *)snids));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFSENS, numofsens));
	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_ARROFGSID, numofsens * sizeof(int), (byte *)gsids));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFAN, numofan));
	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_ARROFANID, numofan * sizeof(int), (byte *)anids));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFACTS, numofacts));
	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_ARROFGAID, numofacts * sizeof(int), (byte *)gaids));

	return TP3_SC_NOERROR;
}


