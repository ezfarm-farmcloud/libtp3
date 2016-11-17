/**
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
  \file ttap3_process.h
  \brief 메세지 콜백 처리를 위한 타입 및 함수
*/

#ifndef __LIB_TTA_P3_PROCESS_H__
#define __LIB_TTA_P3_PROCESS_H__

/** 요청 메세지를 처리하기 위한 콜백 */
typedef tp3_stat_t (*ptp3_rescb_t) (ptp3_msg_t preq, ptp3_msg_t pres, void *data);

/**
 * 메세지 콜백함수 처리를 위한 구조체를 초기화 한다.
 *
 * @param pcbinfo 메세지 콜백함수 처리를 위한 구조체
 */
void
tp3_initcb ();

/**
 * 메세지 처리를 위한 콜백함수를 메세지별로 등록한다.
 * 중요한 점은 응답에 대한 확은을 위해서도 콜백을 등록할 수 있다.
 * 콜백을 등록하지 않는 경우 디폴트 함수가 사용된다.
 *
 * @param msgtype 메세지 타입
 * @param resfunc 해당 메세지타입의 요청에 대한 처리를 위한 콜백함수
 * @param data 사용자 정의 데이터에 대한 포인터. 단, tp3_response의 data가 우선함.
 * @see tp3_response
 */
void
tp3_registcbfunc (tp3_msgtype_t msgtype, ptp3_rescb_t resfunc, void *data);

/**
 * 요청에 대한 응답을 처리합니다.
 * 내부적으로 메세지 타입에 따라 등록된 콜백함수를 이용하여 
 * 요청 메세지를 처리하고 응답 메세지를 생성한다. 
 * 최종적으로 생성된 응답메세지를 이용하여 응답 메세지프레임을 세팅한다.
 *
 * @param preq 요청 프레임
 * @param pres 요청에 대한 응답 프레임
 * @param data 콜백함수의 인자 NULL이면 tp3_registcbfunc 에서 등록한 인자가 전달됨.
 * @return 상태코드. 
 * @see tp3_registcbfunc
 */
tp3_stat_t
tp3_response (ptp3_frame_t preq, ptp3_frame_t pres, void *data);

/** 
 * 디폴트 응답 메세지를 생성하기 위한 콜백함수이다. 
 *
 * @param preq 요청 메세지
 * @param pres 요청에 대한 응답 메세지
 * @param data 사용자 정의 데이터
 * @return 상태코드. 
 */
tp3_stat_t 
tp3_default_rescb (ptp3_msg_t preq, ptp3_msg_t pres, void *data);

#endif
