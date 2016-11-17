/** 
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
  \file ttap3_process.c
  \brief 메세지 콜백 처리를 위한 타입 및 함수
 */

#include <stdio.h>
#include <stdlib.h>

#include <ttap3_define.h>
#include <ttap3_config.h>
#include <ttap3_base.h>
#include <ttap3_util.h>
#include <ttap3_process.h>

#include <../util/mem.h>

static struct {
	ptp3_rescb_t resfunc;
	void *userdata;
} registedfuncs[_TP3_MSGTYPE_MAX];

void
tp3_initcb () {
	int i;
	for (i = 0; i < _TP3_MSGTYPE_MAX; i++) {
		registedfuncs[i].resfunc = tp3_default_rescb;
		registedfuncs[i].userdata = NULL;
	}
}

void
tp3_registcbfunc (tp3_msgtype_t msgtype, ptp3_rescb_t resfunc, void *data) {
	registedfuncs[msgtype].resfunc = resfunc;
	registedfuncs[msgtype].userdata = data;
}

tp3_stat_t
tp3_response (ptp3_frame_t preq, ptp3_frame_t pres, void *data) {
	tp3_msg_t req, res;
	tp3_stat_t stat = TP3_SC_NOERROR;
	tp3_msgtype_t msgtype;

	tp3_initmsg (&req);
	tp3_initmsg (&res);

	stat = tp3_parseframe (preq, &req); 
	if (stat != TP3_SC_NOERROR) {
		//tp3_printerror (stat);
		return stat;
	}

	msgtype = (req.header).msgtype;
	if (data == NULL) {
		stat = (registedfuncs[msgtype].resfunc) (&req, &res, registedfuncs[msgtype].userdata);
	} else {
		stat = (registedfuncs[msgtype].resfunc) (&req, &res, data);
	}

	if (stat != TP3_SC_NOERROR) {
		//tp3_printerror (stat);
		tp3_releasemsg (&req);
		return stat;
	}

	stat = tp3_generateframe (&res, pres);
	if (stat != TP3_SC_NOERROR) {
		//tp3_printerror (stat);
	}
	tp3_releasemsg (&req);
	tp3_releasemsg (&res);
	return stat;
}

tp3_stat_t 
tp3_default_rescb (ptp3_msg_t preq, ptp3_msg_t pres, void *data) {
	return tp3_setresponse (preq, pres, TP3_RC_NORMAL);
}
