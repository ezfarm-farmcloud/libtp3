/** 
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
 \file ttap3_define.h
 \brief TTAK.KO-06.0288 Part 3 의 상수값 정의

TTA Part 3 에서 정의된 상수값들을 정의한다.
 */

#ifndef __LIB_TTA_P3_DEFINE_H__
#define __LIB_TTA_P3_DEFINE_H__

#define _TP3_NODEID_SIZE	3	///< 노드식별자의 크기
#define _TP3_GID_SIZE		4	///< 센서 및 구동기의 글로벌 식별자 크기
#define _TP3_DEVID_SIZE		1	///< 센서 및 구동기의 식별자 크기
#define _TP3_STAT_SIZE		1	///< 상태정보의 크기
#define _TP3_SENDATA_SIZE	4	///< 센서 환경정보의 크기
#define _TP3_ACTDATA_SIZE	2	///< 구동기 환경(구동)정보의 크기

/** 메세지타입 정의 */
typedef enum {
	TP3_MT_KEEPALIVE = 0x00,		///< 네트워크 체크 메세지
	TP3_MT_CONNAPPROVAL = 0x01,		///< 연결승인 메세지 
	TP3_MT_STATINFO = 0x02,			///< 상태정보 알림 메세지
	TP3_MT_ENVINFO = 0x03,			///< 환경정보 알림 메세지
	TP3_MT_STATHIST = 0x04,			///< 상태정보 이력 메세지
	TP3_MT_ENVHIST = 0x05,			///< 환경정보 이력 메세지
	TP3_MT_PROFQUERY = 0x06,		///< 프로파일 조회 메세지
	TP3_MT_PROFUPDATE = 0x07,		///< 프로파일 설정 메세지
	TP3_MT_STATQUERY = 0x08,		///< 상태정보 요청 메세지
	TP3_MT_FAULTMNG = 0x09,			///< 장애관리 메세지
	TP3_MT_ACTCTRL = 0x0A,			///< 액추에이터 제어 메세지
	TP3_MT_ENVQUERY = 0x0B			///< 환경정보 요청 메세지
} tp3_msgtype_t;

#define _TP3_MSGTYPE_MAX	0x0C

/** 메세지 전송타입 정의 */
typedef enum {
	TP3_TT_CG_OS_REQ = 0x00,		///< 2단계 통합제어기 요청
	TP3_TT_OS_CG_RES = 0x01,		///< 2단계 통합제어기 요청에 대한 응답
	TP3_TT_OS_CG_REQ = 0x02,		///< 2단계 운영시스템 요청
	TP3_TT_CG_OS_RES = 0x03,		///< 2단계 운영시스템 요청에 대한 응답
	TP3_TT_CG_OS_NOTI = 0x04,		///< 1단계 통합제어기 노티
	TP3_TT_OS_CG_REQA = 0x05,		///< 3단계 운영시스템 요청
	TP3_TT_CG_OS_RESA = 0x06,		///< 3단계 운영시스템 요청에 대한 응답
	TP3_TT_OS_CG_ACKA = 0x07		///< 3단계 운영시스템 확인
} tp3_transtype_t;

/** 메세지 응답코드 정의 */
typedef enum {
	TP3_RC_NORMAL = 0x00,			///< 정상응답
	TP3_RC_MSGTYPE_ERR = 0x01,		///< 정의되지 않은 메세지
	TP3_RC_TRANSTYPE_ERR = 0x02,	///< 정의되지 않은 전송타입
	TP3_RC_GCG_ERR = 0x03,			///< 통합제어기 식별자 상이
	TP3_RC_GOS_ERR = 0x04,			///< 운영시스템 식별자 상이
	TP3_RC_TRANTIMEDATE_ERR = 0x05,	///< 전송시간 오류
	TP3_RC_DUPMSGSEQ_ERR = 0x06,	///< 메세지 일련번호 오류
	TP3_RC_NOREQFIELD_ERR = 0x07,	///< 필수 필드 부재

	TP3_RC_INTERNAL_ERR = 0x10,		///< 메세지 처리중 내부 오류. TTAK.06-0288 에는 없는 값
	
	TP3_RC_FRAMEFMT_ERR = 0xFF		///< 메세지 프레임 구조 불일치
} tp3_rescode_t;

/** 메세지를 구성하는 개별 필드들의 코드 정의 */
typedef enum {
	TP3_FC_INTERFACETYPE = 0x0000,		///< 메세지 구분 필드코드
	TP3_FC_MESSAGETYPE = 0x0001,		///< 메세지 타입 필드코드
	TP3_FC_TRANSTYPE = 0x0002,			///< 메세지 전송타입 필드코드
	TP3_FC_GCGID = 0x0003,				///< 통합제어기 식별자 필드코드
	TP3_FC_GOSID = 0x0004,				///< 운영시스템 식별자  필드코드
	TP3_FC_TRANSDATE = 0x0005,			///< 전송날짜 필드코드
	TP3_FC_TRANSTIME = 0x0006,			///< 전송시간 필드코드
	TP3_FC_MSGSEQNUM = 0x0007,			///< 메세지 일련번호 필드코드
	TP3_FC_RESCODE = 0x0008,			///< 메세지 응답코드 필드코드

	TP3_FC_NUMOFSN = 0x1000,			///< 센서노드개수 필드코드
	TP3_FC_ARROFSNID = 0x1001,			///< 센서노드아이디배열 필드코드
	TP3_FC_NUMOFAN = 0x1002,			///< 제어노드개수 필드코드
	TP3_FC_ARROFANID = 0x1003,			///< 제어노드아이디배열 필드코드
	TP3_FC_SERIALNUM = 0x1004,			///< 시리얼번호 필드코드
	TP3_FC_MODELNUM = 0x1005,			///< 모델번호 필드코드
	TP3_FC_IPV4ADDR = 0x1006,			///< IPv4 필드코드
	TP3_FC_IPV6ADDR = 0x1007,			///< IPv6 필드코드
	TP3_FC_INSTSITE = 0x1008,			///< 설치장소 필드코드
	TP3_FC_MCUTYPE = 0x1009,			///< MCP타입 필드코드
	TP3_FC_OSTYPE = 0x100A,				///< OS타입 필드코드
	TP3_FC_OSVER = 0x100B,				///< OS버전 필드코드
	TP3_FC_RAMSIZE = 0x100C,			///< RAM사이즈 필드코드
	TP3_FC_POWTYPE = 0x100D,			///< 전원타입 필드코드
	TP3_FC_SWVER = 0x100E,				///< 소프트웨어버전 필드코드
		
	TP3_FC_SNID = 0x2000,				///< 센서노드아이디 필드코드
	TP3_FC_NUMOFSENS = 0x2001,			///< 센서개수 필드코드
	TP3_FC_ARROFGSID = 0x2002,			///< 센서아이디배열 필드코드
	TP3_FC_SID = 0x2003,				///< 센서아이디 필드코드
	TP3_FC_SENTYPE = 0x2004,			///< 센서타입 필드코드
	TP3_FC_SENDATATYPE = 0x2005,		///< 센서데이터타입 필드코드
	TP3_FC_SENGETTYPE = 0x2006,			///< 센서데이터수집타입 필드코드
	TP3_FC_SENGETINTV = 0x2007,			///< 센서데이터수집간격 필드코드
	TP3_FC_MAXVALUE = 0x2008,			///< 센서최대값 필드코드
	TP3_FC_MINVALUE = 0x2009,			///< 센서최소값 필드코드
	TP3_FC_PRECISION = 0x200A,			///< 센서정밀도 필드코드
	TP3_FC_ERRRANGE = 0x200B,			///< 센서오차범위 필드코드
	TP3_FC_THRESHOLDTYPE = 0x200C,		///< 센서임계치 설정방식 필드코드
	TP3_FC_THRESHOLD = 0x200D,			///< 센서임계치 필드코드
			
	TP3_FC_ANID = 0x3000,				///< 제어노드아이디 필드코드
	TP3_FC_NUMOFACTS = 0x3001,			///< 제어기개수 필드코드
	TP3_FC_ARROFGAID = 0x3002,			///< 제어기아이디배열 필드코드
	TP3_FC_AID = 0x3003,				///< 제어기아이디 필드코드
	TP3_FC_ACTTYPE = 0x3004,			///< 제어기종류 필드코드
	TP3_FC_ACTDATATYPE = 0x3005,		///< 제어기 제어를위한 데이터타입 필드코드
	TP3_FC_ACTGETTYPE = 0x3006,			///< 제어기 데이터 수집방식 필드코드
	TP3_FC_ACTGETINTV = 0x3007,			///< 제어기 데이터 수집주기 필드코드
				
	TP3_FC_GCGSTAT = 0x4000,			///< 통합제어기 동작상태 필드코드
	TP3_FC_ARROFSNSTAT = 0x4001,		///< 센서노드상태 필드코드
	TP3_FC_ARROFANSTAT = 0x4002,		///< 제어노드상태 필드코드
	TP3_FC_ARROFSENSSTAT = 0x4003,		///< 센서상태 필드코드
	TP3_FC_ARROFACTSSTAT = 0x4005,		///< 제어기상태 필드코드
	TP3_FC_TIMESTAMP = 0x4006,			///< 발생시점 필드코드
	
	TP3_FC_ARROFSENSVAL = 0x5000,		///< 센서값 필드코드
	TP3_FC_ARROFACTSVAL = 0x5001,		///< 제어기값 필드코드
			
	TP3_FC_HISTREQTYPE = 0x6000,		///< 이력요청구분자 필드코드
	TP3_FC_SDATETIME = 0x6001,			///< 시작날짜시각 필드코드
	TP3_FC_EDATETIME = 0x6002,			///< 종료날짜시각 필드코드
	TP3_FC_NUMOFBLK = 0x6003,			///< 블럭의 개수 필드코드
	TP3_FC_BLKOFSTAT = 0x6004,			///< 상태정보블럭 필드코드
	TP3_FC_BLKOFENV = 0x6005,			///< 환경정보블럭 필드코드
	TP3_FC_CONTINFO = 0x6006,			///< 연속메세지여부 필드코드
				
	TP3_FC_FALUTMETHOD = 0x7000,		///< 장애처리방법 필드코드
	TP3_FC_SWBINARY = 0x7001			///< 설치프로그램 필드코드
} tp3_fieldcode_t;

/** 이력정보 요청 타입 */
typedef enum {
	TP3_HREQ_ALL = 0x00,		///< 미전송내역모두
	TP3_HREQ_FIRST = 0x01,		///< 지정된 시점 이후 첫번째
	TP3_HREQ_BETWEEN = 0x02		///< 지정된 구간내 발생건
} tp3_hreq_t;

/** 메세지 응답코드로 부족한 에러상황을 표현하기 위해 확장된 상태코드 */
typedef enum {
	TP3_SC_NOERROR = 0x00,
	TP3_SC_MSGTYPE_ERR = 0x01,
	TP3_SC_TRANSTYPE_ERR = 0x02,
	TP3_SC_GCG_ERR = 0x03,
	TP3_SC_GOS_ERR = 0x04,
	TP3_SC_TRANTIMEDATE_ERR = 0x05,
	TP3_SC_DUPMSGSEQ_ERR = 0x06,
	TP3_SC_NOREQFIELD_ERR = 0x07,
	TP3_SC_FRAMEFMT_ERR = 0xFF,

	TP3_SC_INTERNAL_ERR = 0x10,					///< 내부 처리 오류
	TP3_SC_MEMORY_ALLOCATION_FAIL = 0x11,		///< 메모리 할당 실패
	TP3_SC_INTERFACETYPE_NOT_MATCHED = 0x12,	///< 인터페이스 타입불일치
	TP3_SC_CHECKSUM_NOT_MATCHED = 0x13,			///< 체크썸 불일치
	TP3_SC_IS_NOT_REQUEST = 0x14,				///< 요청이 아닌 메세지에 대한 응답생성시도
	TP3_SC_EXIST_NODEID = 0x15,					///< 노드아이디가 이미 존재함
	TP3_SC_NOT_FOUND_NODEID = 0x16,				///< 노드아이디를 찾을 수 없음
	TP3_SC_NOT_PROPER_DEVTYPE = 0x17,			///< 적절하지 않은 디바이스 타입
	TP3_SC_NOT_FOUND_DEVID = 0x18,				///< 디바이스 아이디를 찾을 수 없음
	TP3_SC_NOT_ENOUGH_SPACE = 0x19				///< 할당된 공간이 충분치 않음
} tp3_stat_t, *ptp3_stat_t;

#define TP3_SC_MAXSIZE			0x1A	

/** 장애발생시 대응방법 */
typedef enum {
	TP3_FHM_HWRESET = 0x00,			///< 하드웨어 리셋
	TP3_FHM_SWRESET = 0x01,			///< 소프트웨어 리셋
	TP3_FHM_SHUTDOWN = 0x02,		///< 시스템 종료
	TP3_FHM_SWINSTALL = 0x03		///< Not supported
} tp3_faultmng_t;

/** 통합제어기의 상태값 */
typedef enum {
	TP3_GCGST_NORMAL = 0x00,		///< 정상
	TP3_GCGST_COMMERR = 0x01,		///< 통신오류
	TP3_GCGST_SVCERR = 0x02			///< 서비스오류
} tp3_gcgstat_t;

/** 센서노드 혹은 제어노드의 상태값 */
typedef enum {
	TP3_NODEST_NORMAL = 0x00,		///< 정상
	TP3_NODEST_COMMERR = 0x01,		///< 통신오류
	TP3_NODEST_SVCERR = 0x02		///< 서비스오류
} tp3_nodestat_t;

/** 센서 혹은 제어기의 상태값 */
typedef enum {
	TP3_DEVST_NORMAL = 0x00,		///< 정상
	TP3_DEVST_ERROR = 0x01,			///< 오류
} tp3_devstat_t;

/** 센서의 타입 정보  */
typedef enum {
	TP3_SENT_RAW = 0x00,			///< RAW
	TP3_SENT_INTTEMP= 0x01,			///< 내부온도
	TP3_SENT_INTDEWPNT = 0x02,		///< 내부이슬점
	TP3_SENT_INTREHUM = 0x03,		///< 내부상대습도
	TP3_SENT_EXTTEMP = 0x04,			///< 외부온도
	TP3_SENT_EXTHUM = 0x05,			///< 외부습도
	TP3_SENT_WINDDIR = 0x08,			///< 풍향
	TP3_SENT_WINDSPD = 0x09,			///< 풍속
	TP3_SENT_ILLUM = 0x0A,			///< 조도
	TP3_SENT_SOLRAD = 0x0B,			///< 일사량
	TP3_SENT_PRECIP = 0x0C			///< 강우
} tp3_sentype_t;

/** 센서의 데이터 타입 정보  */
typedef enum {
	TP3_SENDT_FLOAT = 0x00,			///< 실수형
	TP3_SENDT_INT = 0x01,			///< 정수형
	TP3_SENDT_UNSINT = 0x02			///< 양수형
} tp3_sendatatype_t;

/** 센서의 데이터 수집방법  */
typedef enum {
	TP3_SENGT_NOMONITOR = 0x00,		///< 모니터링하지 않음
	TP3_SENGT_REQUEST = 0x01,		///< 요청시
	TP3_SENGT_PERIOD = 0x02,			///< 주기적으로
	TP3_SENGT_EVENT = 0x03			///< 이벤트 발생시 (임계치 기준)
} tp3_sengathertype_t;

/** 센서의 데이터 임계치 비교방법  */
typedef enum {
	TP3_THRSDT_GROREQ = 0x01,		///< 이상
	TP3_THRSDT_LEOREQ = 0x02,		///< 이하
	TP3_THRSDT_GREATER = 0x03,		///< 초과
	TP3_THRSDT_LESS = 0x04,			///< 미만
	TP3_THRSDT_EQUAL = 0x05			///< 동일
} tp3_senthrsdtype_t;

/** 구동기의 타입 정보  */
typedef enum {
	TP3_ACTT_RAW = 0x00,			///< RAW
	TP3_ACTT_ONOFF = 0x01,			///< 온/오프
	TP3_ACTT_BIDIRECTIONAL = 0x02,	///< 양방향 온오프
	TP3_ACTT_TIME = 0x03,			///< 초단위 시간제어
	TP3_ACTT_PERCENT = 0x04			///< % 제어
} tp3_acttype_t;

/** 구동기의 데이터 타입 정보  */
typedef enum {
	TP3_ACTDT_DIGITAL = 0x00,		///< 디지털 타입
	TP3_ACTDT_ANALOG = 0x01			///< 아날로그 타입
} tp3_actdatatype_t;

/** 구동기의 데이터 수집방법  */
typedef enum {
	TP3_ACTGT_PERIOD = 0x00,		///< 주기적
	TP3_ACTGT_REQUEST = 0x01,		///< 요청시
	TP3_ACTGT_CHANGED = 0x02		///< 변경시
} tp3_actgathertype_t;

/** 디바이스 타입 */
typedef enum {
	TP3_DEV_SNODE = 0x00,
	TP3_DEV_ANODE = 0x01,
	TP3_DEV_SENSOR = 0x02,
	TP3_DEV_ACTUATOR = 0x03,
	TP3_DEV_GCG = 0x04,
	TP3_DEV_GOS = 0x05
} tp3_dev_t;

#endif
