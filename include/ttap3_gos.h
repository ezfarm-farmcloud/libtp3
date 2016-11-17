/**
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
  \file ttap3_gos.h
  \brief 운영시스템에서 사용할 API
*/

#ifndef __LIB_TTA_P3_GOS_H__
#define __LIB_TTA_P3_GOS_H__

/**
 * 운영시스템 운영데이터를 초기화 한다.
 * @param gosid 운영시스템 식별자
 * @param gcgids 통합제어기 식별자들
 * @param numofgcgs 통합제어기 식별자들의 개수
 */
void
tp3_initgos (int gosid, int *gcgids, int numofgcgs);

/**
 * 운영시스템 운영데이터를 해제한다.
 */
void
tp3_releasegos ();

/**
 * 통합제어기의 접속 요청을 처리한다.
 * ptp3_rescb_t 타입의 함수로 작성된다.
 * @param preq 접속 요청
 * @param pres 접속 요청에 대한 응답
 * @return 상태코드. 
 */
tp3_stat_t
tp3_acceptmsg (ptp3_msg_t preq, ptp3_msg_t pres, void *data);

/**
 * 통합제어기에서 전송된 메세지에 대한 확인 메세지를 생성한다.
 * 특별한 동작은 없다.
 * @param preq 통합제어기에서 생성한 전송메세지
 * @param pres 메세지에 대한 응답
 * @param data 사용자 정의 데이터
 * @return 상태코드. 
 */
tp3_stat_t
tp3_defaultconfirm (ptp3_msg_t preq, ptp3_msg_t pres, void *data);

/**
 * 통합제어기에게 상태이력을 요청하기 위한 메세지를 생성한다.
 * @param preq 상태 이력 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param reqtype 이력정보 요청 타입
 * @param stime 시작시간
 * @param etime 종료시간
 * @return 상태코드. 
 */
tp3_stat_t
tp3_stathistmsg (ptp3_msg_t preq, int gcgid, tp3_hreq_t reqtype, time_t stime, time_t etime);

/**
 * 통합제어기에게 환경정보 이력을 요청하기 위한 메세지를 생성한다.
 * @param preq 환경정보 이력 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param reqtype 이력정보 요청 타입
 * @param stime 시작시간
 * @param etime 종료시간
 * @return 상태코드. 
 */
tp3_stat_t
tp3_envhistmsg (ptp3_msg_t preq, int gcgid, tp3_hreq_t reqtype, time_t stime, time_t etime);

/**
 * 통합제어기에게 연결설정상태를 요청하기 위한 메세지를 생성한다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @return 상태코드. 
 */
tp3_stat_t
tp3_gcgconfmsg (ptp3_msg_t preq, int gcgid);

/**
 * 통합제어기에게 센서노드의 연결설정상태를 요청하기 위한 메세지를 생성한다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param snid 센서노드 식별자
 * @return 상태코드. 
 */
tp3_stat_t
tp3_snodeconfmsg (ptp3_msg_t preq, int gcgid, int snid);

/**
 * 통합제어기에게 엑츄에이터노드의 연결설정상태를 요청하기 위한 메세지를 생성한다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param anid 엑츄에이터노드 식별자
 * @return 상태코드. 
 */
tp3_stat_t
tp3_anodeconfmsg (ptp3_msg_t preq, int gcgid, int anid);

/**
 * 통합제어기의 속성정보를 요청하기 위한 메세지를 생성한다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @return 상태코드. 
 */
tp3_stat_t
tp3_gcgattrmsg (ptp3_msg_t preq, int gcgid);

/**
 * 통합제어기에게 센서의 속성정보를 요청하기 위한 메세지를 생성한다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param snid 센서노드 식별자
 * @param sid 센서의 식별자
 * @return 상태코드. 
 */
tp3_stat_t
tp3_senattrmsg (ptp3_msg_t preq, int gcgid, int snid, int sid);

/**
 * 통합제어기에게 엑츄에이터의 속성정보를 요청하기 위한 메세지를 생성한다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param anid 엑츄에이터노드 식별자
 * @param aid 엑츄에이터 식별자
 * @return 상태코드. 
 */
tp3_stat_t
tp3_actattrmsg (ptp3_msg_t preq, int gcgid, int anid, int aid);

/**
 * 통합제어기에게 센서의 속성정보 업데이트를 요청하기 위한 메세지를 생성한다. 
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param snid 센서노드 식별자
 * @param sid 센서의 식별자
 * @param gathertype 센서 데이터 수집방법
 * @param interval 센서 데이터 수집 주기
 * @param thrsdtype 센서 데이터 이벤트 발생 조건
 * @param value 센서 데이터 이벤트 발생 조건값
 * @return 상태코드 
 */
tp3_stat_t
tp3_senattrupdatemsg (ptp3_msg_t preq, int gcgid, int snid, int sid, tp3_sengathertype_t gathertype, int interval, tp3_senthrsdtype_t thrsdtype, int value);

/**
 * 통합제어기에게 엑츄에이터의 속성정보 업데이트를 요청하기 위한 메세지를 생성한다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param anid 엑츄에이터노드 식별자
 * @param aid 엑츄에이터 식별자
 * @param gathertype 엑츄에이터 데이터 수집방법
 * @param interval 엑츄에이터 데이터 수집 주기
 * @return 상태코드 
 */
tp3_stat_t
tp3_actattrupdatemsg (ptp3_msg_t preq, int gcgid, int anid, int aid, tp3_actgathertype_t gathertype, int interval);

/**
 * 통합제어기 문제 발생시 해결을 위한 메세지를 생성한다.
 * 장애처리 방법중 소프트웨어 재설치 기능은 제공하지 않는다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param faultmng 장애처리 방법
 * @return 상태코드 
 */
tp3_stat_t
tp3_faultmngmsg (ptp3_msg_t preq, int gcgid, tp3_faultmng_t fm);

/**
 * 통합제어기에게 엑츄에이터의 제어를 요청하기 위한 메세지를 생성한다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param length 엑츄에이터들의 개수
 * @param gaids 엑츄에이터 식별자들
 * @param values 엑츄에이터 제어값들
 * @return 상태코드. 
 */
tp3_stat_t
tp3_actcontrolmsg (ptp3_msg_t preq, int gcgid, int length, int *gaids, int *values);

/**
 * 통합제어기에게 현재의 환경정보를 요청하기 위한 메세지를 생성한다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param numofsens 센서들의 개수
 * @param gsids 센서 식별자들
 * @param numofacts 엑츄에이터들의 개수
 * @param gaids 엑츄에이터 식별자들
 * @return 상태코드. 
 */
tp3_stat_t
tp3_envquerymsg (ptp3_msg_t preq, int gcgid, int numofsens, int *gsids, int numofacts, int *gaids);

/**
 * 통합제어기에게 현재의 상태정보를 요청하기 위한 메세지를 생성한다.
 * @param preq 요청메세지
 * @param gcgid 통합제어기 식별자
 * @param numofsn 센서 노드들의 개수
 * @param snids 센서 노드 식별자들
 * @param numofsens 센서들의 개수
 * @param gsids 센서 식별자들
 * @param numofan 엑츄에이터 노드들의 개수
 * @param anids 엑츄에이터 노드 식별자들
 * @param numofacts 엑츄에이터들의 개수
 * @param gaids 엑츄에이터 식별자들
 * @return 상태코드. 
 */
tp3_stat_t
tp3_statquerymsg (ptp3_msg_t preq, int gcgid, int numofsn, int *snids, int numofsens, int *gsids, int numofan, int *anids, int numofacts, int *gaids);


/**
 * 통합제어기에서 보내온 상태정보를 파싱하여 원하는 배열에 값을 채워준다. 
 * @param preq 요청메세지
 * @param devtype 디바이스 타입
 * @param num 전송된 메세지에서 저장된 디바이스의 개수
 * @param arrid 디바이스 아이디를 저장할 배열
 * @param arrval 디바이스별 상태값을 저장할 배열
 * @param len 함수 인자로 전달된 배열의 길이
 * @return 상태코드. 
 */
tp3_stat_t
tp3_parse_statmsg (ptp3_msg_t preq, tp3_dev_t devtype, int *num, int *arrid, int *arrval, int len);

/**
 * 통합제어기에서 보내온 환경정보를 파싱하여 원하는 배열에 값을 채워준다. 
 * @param preq 요청메세지
 * @param devtype 디바이스 타입
 * @param num 전송된 메세지에서 저장된 디바이스의 개수
 * @param arrid 디바이스 아이디를 저장할 배열
 * @param arrval 디바이스별 환경값을 저장할 배열
 * @param len 함수 인자로 전달된 배열의 길이
 * @return 상태코드. 
 */
tp3_stat_t
tp3_parse_envmsg (ptp3_msg_t preq, tp3_dev_t devtype, int *num, int *arrid, int *arrval, int len);

#endif
