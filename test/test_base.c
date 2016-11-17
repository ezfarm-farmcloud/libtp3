/**
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
  \file test_base.c
  \brief base 모듈 테스트
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ttap3_define.h>
#include <ttap3_config.h>
#include <ttap3_base.h>
#include <ttap3_util.h>
#include <mem.h>

#include "test.h"

/**
 * 메세지(tp3_msg_t)와 메세지 프레임(tp3_frame_t) 사이의 전환을 테스트
 */
tp3_stat_t
test_base_frame () {
	tp3_msg_t msg;
	tp3_frame_t frame;
	tp3_fdata_t data;
	int snids[3] = {1, 3, 5};
	int tmp[4];

	tp3_initmsg (&msg);
	ASSERT_NOERROR (tp3_setheader (&msg, TP3_MT_CONNAPPROVAL, TP3_TT_CG_OS_REQ, 1, 5, 100));

	data.data = 5;
	ASSERT_NOERROR (tp3_addfield (&msg, TP3_FC_NUMOFSN, 1, &data));

	//data.pbyte = (byte *)snids;
	//ASSERT_NOERROR (tp3_addfield (&msg, TP3_FC_ARROFACTSVAL, 3 * sizeof(int), &data));

	data.data = 521;
	ASSERT_NOERROR (tp3_addfield (&msg, TP3_FC_ARROFACTSVAL, 1 * sizeof(int), &data));

	data.pbyte = (byte *)"CFLORA-GCG";
	ASSERT_NOERROR (tp3_addfield (&msg, TP3_FC_MODELNUM, strlen((const char *)"CFLORA-GCG"), &data));

	//printf ("generating a msg\n");
	tp3_printmsg (&msg);

	ASSERT_NOERROR (tp3_generateframe (&msg, &frame));

	//printf ("generating a frame\n");
	tp3_printframe(&frame);

	tp3_releasemsg (&msg);

	tp3_initmsg (&msg);
	//printf ("parsing a frame\n");
	ASSERT_NOERROR (tp3_parseframe (&frame, &msg));

	tp3_printmsg (&msg);

	ASSERT (0 < tp3_readfield (&msg, TP3_FC_NUMOFSN, &data));
	ASSERT (5 == data.data);

	ASSERT (1 == tp3_readfield_array (&msg, TP3_FC_ARROFACTSVAL, tmp, 4));
	ASSERT (521 == tmp[0]);

	ASSERT (0 < tp3_readfield (&msg, TP3_FC_MODELNUM, &data));
	ASSERT (0 == strncmp((const char *)"CFLORA-GCG", (const char *)data.pbyte, strlen((const char *)"CFLORA-GCG")));

	//tp3_printmsg (&msg);
	tp3_releasemsg (&msg);
	tp3_releaseframe (&frame);

	return TP3_SC_NOERROR;
}
	
/**
 * 메세지(tp3_msg_t)에서 가변 필드의 값을 읽고 쓰는 테스트
 */
tp3_stat_t
test_base_addnreadfield () {
	tp3_msg_t msg;
	tp3_fdata_t data;
	byte sw[2];
	byte model[10];
	byte *ptr;

	tp3_initmsg (&msg);
	ASSERT_NOERROR (tp3_setheader (&msg, TP3_MT_CONNAPPROVAL, TP3_TT_CG_OS_REQ, 1, 5, 100));

	ASSERT_NOERROR (tp3_addfield_num (&msg, TP3_FC_NUMOFSN, 10));
	ASSERT (0 < tp3_readfield (&msg, TP3_FC_NUMOFSN, &data));
	ASSERT (10 == data.data);

	sw[0] = 10; sw[1] = 20;
	ASSERT_NOERROR (tp3_addfield_bytes (&msg, TP3_FC_SWVER, sw));
	ASSERT (0 < tp3_readfield (&msg, TP3_FC_SWVER, &data));
	//tp3_set2bytes(sw, data.data);
	ptr = (byte *)(&(data.data));
	ASSERT ((10 == ptr[0]) && (20 == ptr[1]));

	strcpy ((char *)model, (const char *)"GOS");
	ASSERT_NOERROR (tp3_addfield_nbytes (&msg, TP3_FC_MODELNUM, 3, model));
	ASSERT (1 == tp3_isnumberinfield (&msg, TP3_FC_MODELNUM));
	ASSERT (3 == tp3_readfieldlen (&msg, TP3_FC_MODELNUM));
	ASSERT (0 < tp3_readfield (&msg, TP3_FC_MODELNUM, &data));
	//tp3_set3bytes(model, data.data);
	ptr = (byte *)(&(data.data));
	ASSERT (('G' == ptr[0]) && ('O' == ptr[1]) && ('S' == ptr[2]));

	strcpy ((char *)model, (const char *)"GOS-123");
	ASSERT_NOERROR (tp3_addfield_nbytes (&msg, TP3_FC_SERIALNUM, strlen((const char *)model), model));
	ASSERT (0 == tp3_isnumberinfield (&msg, TP3_FC_SERIALNUM));
	ASSERT (strlen((const char *)model) == tp3_readfieldlen (&msg, TP3_FC_SERIALNUM));
	ASSERT (strlen((const char *)model) == tp3_readfield (&msg, TP3_FC_SERIALNUM, &data));
	ASSERT (0 == strncmp((const char *)model, (const char *)data.pbyte, strlen((const char *)model)));

	tp3_releasemsg (&msg);

	return TP3_SC_NOERROR;
}

/**
 * tp3_stat_t 에 대응하는 에러메세지들이 존재하는지를 확인
 */
tp3_stat_t
test_base_errormsg () {
	tp3_stat_t stat;

	for (stat = TP3_SC_NOERROR; stat <= TP3_SC_FRAMEFMT_ERR; stat++)
		ASSERT (NULL != tp3_geterrormsg (stat));

	for (stat = TP3_SC_INTERNAL_ERR; stat < TP3_SC_MAXSIZE; stat++)
		ASSERT (NULL != tp3_geterrormsg (stat));

	return TP3_SC_NOERROR;
}

/**
 * 요청 메세지에 대한 응답메세지 해더에 대한 세팅이 잘 되는지를 확인 
 */
tp3_stat_t
test_base_response () {
	tp3_msg_t req, res;

	tp3_initmsg (&req);
	ASSERT_NOERROR (tp3_setheader (&req, TP3_MT_CONNAPPROVAL, TP3_TT_CG_OS_REQ, 1, 5, 100));

	tp3_initmsg (&res);
	ASSERT_NOERROR (tp3_setresponse (&req, &res, TP3_RC_NORMAL));

	ASSERT (req.header.msgtype == res.header.msgtype);
	ASSERT (req.header.gosid == res.header.gosid);
	ASSERT (req.header.gcgid == res.header.gcgid);
	ASSERT (req.header.transtype + 1 == res.header.transtype);
	ASSERT (req.header.msgseq == res.header.msgseq);
	ASSERT (TP3_RC_NORMAL == res.header.rescode);

	return TP3_SC_NOERROR;
}

int
main () {
	TEST (test_base_frame ());
	TEST (test_base_addnreadfield ());
	TEST (test_base_errormsg ());
	TEST (test_base_response());

	return 0;
}
