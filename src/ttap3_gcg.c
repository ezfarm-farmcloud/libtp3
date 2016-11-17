/** 
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
   \file ttap3_gcg.c
   \brief TTAK.KO-06.0288 Part 3 구현에서 GCG 관련 함수
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
#include <ttap3_gcg.h>

#include <../util/mem.h>

/* data structure for gcg internal data */
static struct {
	int gcgid;
	int gosid;
	tp3_nsetdata_t snode;
	tp3_nsetdata_t anode;
	tp3_gcgprof_t profile;
	int msgseq;
} gcgdata;

void
tp3_initgcg (int gcgid, int gosid, ptp3_gcgprof_t profile) {
	gcgdata.gcgid = gcgid;
	gcgdata.gosid = gosid;
	gcgdata.msgseq = 0;

	tp3_initnset (&(gcgdata.snode));
	tp3_initnset (&(gcgdata.anode));

	memcpy ((void *)(&(gcgdata.profile)), (void *)profile, sizeof (tp3_gcgprof_t));

	tp3_initcb ();
}

tp3_stat_t
tp3_setgcgsnodestat (int nodeid, tp3_nodestat_t stat) {
	tp3_setnodestat (&(gcgdata.snode), nodeid, stat);
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_setgcganodestat (int nodeid, tp3_nodestat_t stat) {
	tp3_setnodestat (&(gcgdata.anode), nodeid, stat);
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_updategcgnode (int nodeid, ptp3_nodedata_t pnode) {
	if (pnode->devtype == TP3_DEV_SNODE) {
		return tp3_updatenset (&(gcgdata.snode), nodeid, pnode);
	} else {
		return tp3_updatenset (&(gcgdata.anode), nodeid, pnode);
	}
}

void
tp3_releasegcg () {
	tp3_releasenset (&(gcgdata.snode));
	tp3_releasenset (&(gcgdata.anode));
}

tp3_stat_t
tp3_isok_gcg (ptp3_msg_t preq) {
	if ((preq->header).gosid != gcgdata.gosid)
		return TP3_SC_GOS_ERR;
	if ((preq->header).gcgid != gcgdata.gcgid)
		return TP3_SC_GCG_ERR;
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_connectmsg (ptp3_msg_t preq) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_CONNAPPROVAL, TP3_TT_CG_OS_REQ, gcgdata.gcgid, gcgdata.gosid, gcgdata.msgseq++));

	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFSN, gcgdata.snode.numofnode));
	TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFSNID, &(gcgdata.snode)));

	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFAN, gcgdata.anode.numofnode));
	TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFANID, &(gcgdata.anode)));

	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_SERIALNUM, strlen ((const char *)gcgdata.profile.serial), gcgdata.profile.serial));
	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_MODELNUM, strlen ((const char *)gcgdata.profile.model), gcgdata.profile.model));
	TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_SWVER, gcgdata.profile.swver));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_keepalivemsg (ptp3_msg_t preq) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_KEEPALIVE, TP3_TT_CG_OS_REQ, gcgdata.gcgid, gcgdata.gosid, gcgdata.msgseq++));

	TP3_STAT_NOERROR (tp3_addfield_nbytes (preq, TP3_FC_IPV4ADDR, 4, gcgdata.profile.ipv4));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_notifystatmsg (ptp3_msg_t preq) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_STATINFO, TP3_TT_CG_OS_REQ, gcgdata.gcgid, gcgdata.gosid, gcgdata.msgseq++));

		TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFSN, gcgdata.snode.numofnode));
	if (gcgdata.snode.numofnode > 0) {
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFSNID, &(gcgdata.snode)));
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFSNSTAT, &(gcgdata.snode)));
	}

		TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFAN, gcgdata.anode.numofnode));
	if (gcgdata.anode.numofnode > 0) {
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFANID, &(gcgdata.anode)));
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFANSTAT, &(gcgdata.anode)));
	}

		TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFSENS, gcgdata.snode.numofdev));
	if (gcgdata.snode.numofdev > 0) {
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFGSID, &(gcgdata.snode)));
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFSENSSTAT, &(gcgdata.snode)));
	}

		TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFACTS, gcgdata.anode.numofdev));
	if (gcgdata.anode.numofdev > 0) {
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFGAID, &(gcgdata.anode)));
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFACTSSTAT, &(gcgdata.anode)));
	}

	TP3_STAT_NOERROR (tp3_addfield_timestamp (preq, TP3_FC_TIMESTAMP, time(NULL)));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_notifyenvmsg (ptp3_msg_t preq) {
	tp3_initmsg (preq);

	TP3_STAT_NOERROR (tp3_setheader (preq, TP3_MT_ENVINFO, TP3_TT_CG_OS_REQ, gcgdata.gcgid, gcgdata.gosid, gcgdata.msgseq++));

		TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFSENS, gcgdata.snode.numofdev));
	if (gcgdata.snode.numofdev > 0) {
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFGSID, &(gcgdata.snode)));
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFSENSVAL, &(gcgdata.snode)));
	}

		TP3_STAT_NOERROR (tp3_addfield_num (preq, TP3_FC_NUMOFACTS, gcgdata.anode.numofdev));
	if (gcgdata.anode.numofdev > 0) {
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFGAID, &(gcgdata.anode)));
		TP3_STAT_NOERROR (tp3_addfield_nsetdata (preq, TP3_FC_ARROFACTSVAL, &(gcgdata.anode)));
	}


	TP3_STAT_NOERROR (tp3_addfield_timestamp (preq, TP3_FC_TIMESTAMP, time(NULL)));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_defaultresponse (ptp3_msg_t preq, ptp3_msg_t pres, void *data) {
	tp3_initmsg (pres);

	TP3_STAT_NOERROR (tp3_setresponse (preq, pres, TP3_RC_NORMAL));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_stathistres (ptp3_msg_t preq, ptp3_msg_t pres, void *data) {
	tp3_initmsg (pres);

	TP3_STAT_NOERROR (tp3_setresponse (preq, pres, TP3_RC_NORMAL));

	return TP3_SC_NOERROR;
}
