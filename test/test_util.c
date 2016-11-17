/**
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
   \file test_util.c
   \brief util 모듈 테스트
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ttap3_define.h>
#include <ttap3_config.h>
#include <ttap3_base.h>
#include <ttap3_util.h>
#include <mem.h>

#include "test.h"

void
make_snodedata (ptp3_nodedata_t pnodedata) {
	tp3_senattr_t senattr;
	int sgids[3] = {5, 6, 7};

	ASSERT_NOERROR (tp3_initnodedata (pnodedata, 3, sgids, TP3_DEV_SNODE));
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
	int agids[3] = {10, 11, 12};

	ASSERT_NOERROR (tp3_initnodedata (pnodedata, 3, agids, TP3_DEV_ANODE));
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

tp3_stat_t
test_util_nodedata () {
	tp3_nodedata_t nodedata;

	make_snodedata (&nodedata);
	tp3_releasenodedata (&nodedata);

	make_anodedata (&nodedata);
	tp3_releasenodedata (&nodedata);

	return TP3_SC_NOERROR;
}

tp3_stat_t
test_util_nsetdata () {
	tp3_nsetdata_t ndata;
	tp3_nodedata_t sdata;
	tp3_nodedata_t adata;

	make_snodedata (&sdata);
	make_anodedata (&adata);

	tp3_initnset (&ndata);

	ASSERT_NOERROR (tp3_updatenset (&ndata, 101, &sdata));
	ASSERT_NOERROR (tp3_updatenset (&ndata, 102, &adata));
	ASSERT (TP3_SC_NOT_FOUND_NODEID == tp3_updatenset (&ndata, 103, NULL));
	ASSERT (6 == ndata.numofdev);
	ASSERT (2 == ndata.numofnode);
	ASSERT_NOERROR (tp3_updatenset (&ndata, 102, NULL));
	ASSERT (3 == ndata.numofdev);
	ASSERT (1 == ndata.numofnode);

	tp3_releasenset (&ndata);

	return TP3_SC_NOERROR;
}

tp3_stat_t
test_util_addfield () {
	tp3_nsetdata_t ndata;
	tp3_nodedata_t sdata;
	tp3_nodedata_t adata;
	tp3_msg_t msg;
	//tp3_frame_t frame;
	//tp3_fdata_t data;
	int tmpids[10];

	make_snodedata (&sdata);
	make_anodedata (&adata);

	tp3_initnset (&ndata);

	ASSERT_NOERROR (tp3_updatenset (&ndata, 101, &sdata));
	ASSERT_NOERROR (tp3_updatenset (&ndata, 102, &adata));

	tp3_initmsg (&msg);
	ASSERT_NOERROR (tp3_setheader (&msg, TP3_MT_CONNAPPROVAL, TP3_TT_CG_OS_REQ, 1, 5, 100));

	ASSERT_NOERROR (tp3_addfield_nsetdata (&msg, TP3_FC_ARROFSENSVAL, &ndata));
	ASSERT_NOERROR (tp3_addfield_nsetdata (&msg, TP3_FC_ARROFSENSSTAT, &ndata));

	ASSERT (6 == tp3_readfield_array (&msg, TP3_FC_ARROFSENSVAL, tmpids, 10));
	ASSERT (tmpids[0] == 50);
	ASSERT (tmpids[3] == 50);
	ASSERT (6 == tp3_readfield_array (&msg, TP3_FC_ARROFSENSSTAT, tmpids, 10));
	ASSERT (tmpids[0] == TP3_DEVST_NORMAL);

	return TP3_SC_NOERROR;
}

tp3_stat_t
test_util_timestamp () {
	tp3_msg_t msg;
	tp3_fdata_t data;
	int len;
	time_t cur = time (NULL);

	tp3_initmsg (&msg);

	ASSERT_NOERROR (tp3_addfield_timestamp (&msg, TP3_FC_TIMESTAMP, cur));
	len = tp3_readfield (&msg, TP3_FC_TIMESTAMP, &data);

	ASSERT (7 == len);
	ASSERT (cur == tp3_getepoch (data.pbyte));

	// 3:43:25
	ASSERT_NOERROR (tp3_addfield_interval (&msg, TP3_FC_SENGETINTV, 3*60*60 + 43*60 + 25));
	len = tp3_readfield (&msg, TP3_FC_SENGETINTV, &data);
	ASSERT (6 == len);
	ASSERT ((3*60*60 + 43*60 + 25) == tp3_getinterval (data.pbyte));

	return TP3_SC_NOERROR;
}

tp3_stat_t
test_util_statblock () {
	tp3_block_t block;
	time_t cur = time (NULL);
	int i, j;
	int snid = 101, anid = 102;
	int sgids[3] = {5, 6, 7};
	int agids[3] = {10, 11, 12};
	tp3_msg_t msg;
	tp3_fdata_t data;
	int len;

	tp3_initmsg (&msg);

	ASSERT_NOERROR (tp3_initblock (&block, 3, 1, 3, 1, 3));
	for (i = 0; i < 3; i++) {
		tp3_nextemptyblockitem (&block, cur + 10 * i, TP3_GCGST_NORMAL);
		tp3_setblockitem (&block, snid, TP3_NODEST_NORMAL + i, TP3_DEV_SNODE);
		tp3_setblockitem (&block, anid, TP3_NODEST_NORMAL + i, TP3_DEV_ANODE);
		for (j = 0; j < 3; j++) {
			tp3_setblockitem (&block, sgids[j], TP3_DEVST_NORMAL + j, TP3_DEV_SENSOR);
			tp3_setblockitem (&block, agids[j], TP3_DEVST_NORMAL + j, TP3_DEV_ACTUATOR);
		}
	}

	ASSERT_NOERROR (tp3_addfield_statblock (&msg, &block));
	tp3_releaseblock (&block);

	len = tp3_readfield (&msg, TP3_FC_BLKOFSTAT, &data);
	tp3_parsestatblock (&block, 3, data.pbyte, len);

	ASSERT (3 == block.rows);
	ASSERT (TP3_GCGST_NORMAL == (block.pitems)->gcgstat);
	ASSERT (TP3_GCGST_NORMAL == (block.pitems + 1)->gcgstat);
	ASSERT (TP3_GCGST_NORMAL == (block.pitems + 2)->gcgstat);
	ASSERT (3 == (block.numofdev)[TP3_DEV_SENSOR]);
	ASSERT (cur == (block.pitems)->timestamp);
	ASSERT (TP3_DEVST_NORMAL == ((block.pitems)->vals)[TP3_DEV_SENSOR][0]);
	ASSERT (TP3_DEVST_NORMAL + 1 == ((block.pitems)->vals)[TP3_DEV_SENSOR][1]);

	return TP3_SC_NOERROR;
}

tp3_stat_t
test_util_envblock () {
	tp3_block_t block;
	time_t cur = time (NULL);
	int i, j;
	//int snid = 101, anid = 102;
	int sgids[3] = {5, 6, 7};
	int agids[3] = {10, 11, 12};
	tp3_msg_t msg;
	tp3_fdata_t data;
	int len;

	tp3_initmsg (&msg);

	ASSERT_NOERROR (tp3_initblock (&block, 3, 1, 3, 1, 3));
	for (i = 0; i < 3; i++) {
		tp3_nextemptyblockitem (&block, cur + 10 * i, TP3_GCGST_NORMAL);
		for (j = 0; j < 3; j++) {
			tp3_setblockitem (&block, sgids[j], 14 * j, TP3_DEV_SENSOR);
			tp3_setblockitem (&block, agids[j], 99 * j, TP3_DEV_ACTUATOR);
		}
	}

	ASSERT_NOERROR (tp3_addfield_envblock (&msg, &block));
	tp3_releaseblock (&block);
	len = tp3_readfield (&msg, TP3_FC_BLKOFENV, &data);
	tp3_parseenvblock (&block, 3, data.pbyte, len);

	ASSERT (3 == block.rows);
	ASSERT (3 == (block.numofdev)[TP3_DEV_SENSOR]);
	ASSERT (cur == (block.pitems)->timestamp);
	ASSERT (0 == ((block.pitems)->vals)[TP3_DEV_SENSOR][0]);
	ASSERT (14 == ((block.pitems)->vals)[TP3_DEV_SENSOR][1]);
	ASSERT (99 == ((block.pitems)->vals)[TP3_DEV_ACTUATOR][1]);

	return TP3_SC_NOERROR;
}


int
main () {
	TEST(test_util_nodedata ());
	TEST(test_util_nsetdata ());
	TEST(test_util_addfield ());
	TEST(test_util_timestamp ());
	TEST(test_util_statblock ());
	TEST(test_util_envblock ());
	return 0;
}
