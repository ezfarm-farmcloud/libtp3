/**
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
   \file test_gcgngos.c
   \brief gcg와 gos간 메세지 생성 및 응답생성 테스트
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ttap3_define.h>
#include <ttap3_config.h>
#include <ttap3_base.h>
#include <ttap3_util.h>
#include <ttap3_gcg.h>
#include <ttap3_gos.h>
#include <mem.h>

#include "test.h"

#define _T_GOSID	1001
#define _T_GCGID	5001

#define _T_SNID 	101
#define _T_NUMOFSEN	3
#define _T_GSID_1	5
#define _T_GSID_2	6
#define _T_GSID_3	7

#define _T_ANID 	202
#define _T_NUMOFACT	3
#define _T_GAID_1	10
#define _T_GAID_2	11
#define _T_GAID_3	12


void
make_snodedata (ptp3_nodedata_t pnodedata) {
	tp3_senattr_t senattr;
	int sgids[_T_NUMOFSEN] = {_T_GSID_1, _T_GSID_2,_T_GSID_3};

	ASSERT_NOERROR (tp3_initnodedata (pnodedata, _T_NUMOFSEN, sgids, TP3_DEV_SNODE));
	ASSERT_NOERROR (tp3_setdevenv (pnodedata, sgids[0], 50));
	ASSERT_NOERROR (tp3_setdevenv (pnodedata, sgids[1], 60));
	ASSERT_NOERROR (tp3_setdevenv (pnodedata, sgids[2], 70));

	ASSERT_NOERROR (tp3_setdevstat (pnodedata, sgids[0], TP3_DEVST_NORMAL));
	ASSERT_NOERROR (tp3_setdevstat (pnodedata, sgids[1], TP3_DEVST_ERROR));
	ASSERT_NOERROR (tp3_setdevstat (pnodedata, sgids[2], TP3_DEVST_NORMAL));

	tp3_setsenattr (&senattr, sgids[0], TP3_SENT_RAW, TP3_SENDT_INT, TP3_SENGT_PERIOD, 10, 1024, 0, 1, 5, TP3_THRSDT_GROREQ, 0);
	ASSERT_NOERROR (tp3_setdevattr (pnodedata, sgids[0], (void *)&senattr));
	tp3_setsenattr (&senattr, sgids[1], TP3_SENT_RAW, TP3_SENDT_INT, TP3_SENGT_EVENT, 0, 1024, 0, 1, 5, TP3_THRSDT_GROREQ, 10);
	ASSERT_NOERROR (tp3_setdevattr (pnodedata, sgids[1], (void *)&senattr));
	tp3_setsenattr (&senattr, sgids[2], TP3_SENT_RAW, TP3_SENDT_INT, TP3_SENGT_NOMONITOR, 0, 1024, 0, 1, 5, TP3_THRSDT_GROREQ, 10);
	ASSERT_NOERROR (tp3_setdevattr (pnodedata, sgids[2], (void *)&senattr));

}

void
make_anodedata (ptp3_nodedata_t pnodedata) {
	tp3_actattr_t actattr;
	int agids[_T_NUMOFACT] = {_T_GAID_1, _T_GAID_2,_T_GAID_3};

	ASSERT_NOERROR (tp3_initnodedata (pnodedata, _T_NUMOFACT, agids, TP3_DEV_ANODE));
	ASSERT_NOERROR (tp3_setdevenv (pnodedata, agids[0], 50));
	ASSERT_NOERROR (tp3_setdevenv (pnodedata, agids[1], 60));
	ASSERT_NOERROR (tp3_setdevenv (pnodedata, agids[2], 70));

	ASSERT_NOERROR (tp3_setdevstat (pnodedata, agids[0], TP3_DEVST_NORMAL));
	ASSERT_NOERROR (tp3_setdevstat (pnodedata, agids[1], TP3_DEVST_ERROR));
	ASSERT_NOERROR (tp3_setdevstat (pnodedata, agids[2], TP3_DEVST_NORMAL));

	tp3_setactattr (&actattr, agids[0], TP3_ACTT_RAW, TP3_ACTDT_DIGITAL, TP3_ACTGT_PERIOD, 10);
	ASSERT_NOERROR (tp3_setdevattr (pnodedata, agids[0], (void *)&actattr));
	tp3_setactattr (&actattr, agids[1], TP3_ACTT_RAW, TP3_ACTDT_DIGITAL, TP3_ACTGT_REQUEST, 0);
	ASSERT_NOERROR (tp3_setdevattr (pnodedata, agids[1], (void *)&actattr));
	tp3_setactattr (&actattr, agids[2], TP3_ACTT_RAW, TP3_ACTDT_DIGITAL, TP3_ACTGT_CHANGED, 0);
	ASSERT_NOERROR (tp3_setdevattr (pnodedata, agids[2], (void *)&actattr));
}

void
test_gng_initialize () {
	tp3_gcgprof_t gcgprof;
	byte ipv4[4];
	byte ipv6[6];
	int gosid = _T_GOSID;
	int gcgids[1] = {_T_GCGID};
	tp3_nodedata_t node;

	ipv4[0] = 192; ipv4[1] = 168; ipv4[2] = 0; ipv4[3] = 4;
	ipv6[0] = 0; ipv6[1] = 0; ipv6[2] = 0; ipv6[3] = 0; ipv6[4] = 0; ipv6[5] = 0;

	tp3_setgcgprof (&gcgprof, (byte *)"serial", (byte *)"model", ipv4, ipv6, 1, 2, 3, 4, 5, 6, 7);
	tp3_initgcg (gcgids[0], gosid, &gcgprof);

	make_snodedata (&node);
	ASSERT_NOERROR (tp3_updategcgnode (_T_SNID, &node));
	make_anodedata (&node);
	ASSERT_NOERROR (tp3_updategcgnode (_T_ANID, &node));
   
	tp3_initgos (gosid, gcgids, 1);
}

void
test_gng_finalize () {
	tp3_releasegcg ();
	tp3_releasegos ();
}

/**
 * GCG에서 GOS 로의 커넥션
 */
tp3_stat_t
test_gng_connect () {
	int gosid = _T_GOSID;
	int gcgids[1];
	tp3_msg_t req, res;

	/* 접속가능한 gcgid를 다른 것으로 변경 */
	gcgids[0] = 10;
	tp3_initgos (gosid, gcgids, 1);

	ASSERT_NOERROR (tp3_connectmsg (&req));
	ASSERT_NOERROR (tp3_acceptmsg (&req, &res, NULL));

	ASSERT (res.header.rescode != TP3_RC_NORMAL);

	tp3_releasemsg (&req);
	tp3_releasemsg (&res);

	/* 접속가능한 gcgid를 원래데로 변경 */
	gcgids[0] = _T_GCGID;
	tp3_initgos (gosid, gcgids, 1);

	ASSERT_NOERROR (tp3_connectmsg (&req));
	ASSERT_NOERROR (tp3_acceptmsg (&req, &res, NULL));

	ASSERT (res.header.rescode == TP3_RC_NORMAL);

	tp3_releasemsg (&req);
	tp3_releasemsg (&res);

	return TP3_SC_NOERROR;
}

/**
 * GCG의 접속 유지 메세지
 */
tp3_stat_t
test_gng_keepalive () {
	tp3_msg_t req;
	ASSERT_NOERROR (tp3_keepalivemsg (&req));
	tp3_releasemsg (&req);
	return TP3_SC_NOERROR;
}

/**
 * 현재 상태정보 전송을 위한 메세지
 */
tp3_stat_t
test_gng_notifystat () {
	tp3_msg_t req, res;
	tp3_fdata_t data;
	int tmpids[10];

	ASSERT_NOERROR (tp3_notifystatmsg (&req));
	ASSERT_NOERROR (tp3_defaultconfirm (&req, &res, NULL));

	ASSERT (res.header.rescode == TP3_RC_NORMAL);

	ASSERT (0 < tp3_readfield (&req, TP3_FC_NUMOFSN, &data));
	ASSERT (1 == data.data);
	ASSERT (0 < tp3_readfield (&req, TP3_FC_NUMOFAN, &data));
	ASSERT (1 == data.data);
	ASSERT (1 == tp3_readfield_array (&req, TP3_FC_ARROFSNID, tmpids, 10));
	ASSERT (_T_SNID == tmpids[0]);
	ASSERT (3 == tp3_readfield_array (&req, TP3_FC_ARROFGSID, tmpids, 10));
	ASSERT (_T_GSID_1 == tmpids[0]);
	ASSERT (3 == tp3_readfield_array (&req, TP3_FC_ARROFSENSSTAT, tmpids, 10));
	ASSERT (TP3_DEVST_NORMAL == tmpids[0]);

	tp3_releasemsg (&req);
	tp3_releasemsg (&res);

	return TP3_SC_NOERROR;
}

/**
 * 현재 환경정보 전송을 위한 메세지
 */
tp3_stat_t
test_gng_notifyenv () {
	tp3_msg_t req, res;
	tp3_frame_t frame;
	tp3_fdata_t data;
	int tmpids[10];

	tp3_initmsg (&req);
	tp3_initmsg (&res);

	ASSERT_NOERROR (tp3_notifyenvmsg (&req));
	ASSERT (3 == tp3_readfield_array (&req, TP3_FC_ARROFACTSVAL, tmpids, 10));
	ASSERT (50 == tmpids[0]);

	ASSERT_NOERROR (tp3_generateframe (&req, &frame));
	tp3_releasemsg (&req);
	tp3_printframe (&frame);
	tp3_initmsg (&req);
	ASSERT_NOERROR (tp3_parseframe (&frame, &req));

	ASSERT_NOERROR (tp3_defaultconfirm (&req, &res, NULL));

	ASSERT (res.header.rescode == TP3_RC_NORMAL);

	ASSERT (3 == tp3_readfield_array (&req, TP3_FC_ARROFGAID, tmpids, 10));
	ASSERT (_T_GAID_1 == tmpids[0]);
	ASSERT (3 == tp3_readfield_array (&req, TP3_FC_ARROFGSID, tmpids, 10));
	ASSERT (_T_GSID_1 == tmpids[0]);
	ASSERT (3 == tp3_readfield_array (&req, TP3_FC_ARROFACTSVAL, tmpids, 10));
	ASSERT (50 == tmpids[0]);

	tp3_releasemsg (&req);
	tp3_releasemsg (&res);

	return TP3_SC_NOERROR;
}

int
main () {
	test_gng_initialize ();

	TEST (test_gng_connect ());
	TEST (test_gng_keepalive ());
	TEST (test_gng_notifystat ());
	TEST (test_gng_notifyenv ());

	test_gng_finalize();

	return 0;
}
