/**
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
  \file ttap3_gcg.h
  \brief 통합제어기에서 사용할 API
*/

#ifndef __LIB_TTA_P3_GCG_H__
#define __LIB_TTA_P3_GCG_H__

/**
 * 통합제어기 운영데이터를 초기화 한다.
 * @param gcgid 통합제어기 식별자
 * @param gosid 운영시스템 식별자
 * @param profile 통합제어기 프로파일 정보
 */
void
tp3_initgcg (int gcgid, int gosid, ptp3_gcgprof_t profile);

/**
 * gcg 내부 데이터에 노드 정보를 업데이트한다.
 * @param nodeid 노드 아이디
 * @param pnode 노드 정보 구조체, 이 값이 NULL 이면 정보를 삭제한다.
 * @return 상태코드. 
 */
tp3_stat_t
tp3_updategcgnode (int nodeid, ptp3_nodedata_t pnode);

/**
 * gcg 센서 노드 상태를 업데이트한다.
 * @param nodeid 노드 아이디
 * @param stat 노드 상태
 * @return 상태코드. 
 */
tp3_stat_t
tp3_setgcgsnodestat (int nodeid, tp3_nodestat_t stat);

/**
 * gcg 액츄에이터 노드 상태를 업데이트한다.
 * @param nodeid 노드 아이디
 * @param stat 노드 상태
 * @return 상태코드. 
 */
tp3_stat_t
tp3_setgcganodestat (int nodeid, tp3_nodestat_t stat);

/**
 * 통합제어기 운영데이터를 해제한다.
 */
void
tp3_releasegcg ();

/**
 * 운영시스템으로 접속을 요청하기 위한 메세지를 만든다.
 * @param preq 접속요청 메세지
 * @return 상태코드. 
 */
tp3_stat_t
tp3_connectmsg (ptp3_msg_t preq);

/**
 * 운영시스템과의 접속을 유지하기 위한 메세지를 만든다.
 * @param preq 접속유지 메세지
 * @return 상태코드. 
 */
tp3_stat_t
tp3_keepalivemsg (ptp3_msg_t preq);

/**
 * 상태정보를 알려주기위한 메세지를 만든다.
 * @param preq 상태 정보 전송메세지
 * @return 상태코드. 
 */
tp3_stat_t
tp3_notifystatmsg (ptp3_msg_t preq);

/**
 * 환경정보를 알려주기위한 메세지를 만든다.
 * @param preq 환경정보 전송메세지
 * @return 상태코드. 
 */
tp3_stat_t
tp3_notifyenvmsg (ptp3_msg_t preq);

/**
 * 임의의 요청 메세지에 대한 응답을 생성한다.
 * @param preq 요청메세지
 * @param pres 응답
 * @param data 사용자 정의 데이터
 * @return 상태코드. 
 */
tp3_stat_t
tp3_defaultrespone (ptp3_msg_t preq, ptp3_msg_t pres, void *data);

/**
 * 상태이력 요청 메세지에 대한 응답을 생성한다.
 * @param preq 상태 이력 요청메세지
 * @param pres 상태 이력 요청에 대한 응답
 * @param data 사용자 정의 데이터
 * @return 상태코드. 
 */
tp3_stat_t
tp3_stathistres (ptp3_msg_t preq, ptp3_msg_t pres, void *data);

#endif
