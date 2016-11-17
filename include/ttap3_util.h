/** 
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
  \file ttap3_util.h
  \brief TTAK.KO-06.0288 Part 3 구현에 사용되는 유틸리티 함수 및 데이터타입
 */

#ifndef __LIB_TTA_P3_UTIL_H__
#define __LIB_TTA_P3_UTIL_H__

/** 통합제어기의 프로파일 정보 구조체 */
typedef struct {
	byte serial[_TP3_BUFSIZE];
	byte model[_TP3_BUFSIZE];
	byte ipv4[4];
	byte ipv6[6];
	int installsite;
	byte mcutype;
	byte ostype;
	int osver;
	byte ramsize;
	byte powertype;
	int swver;
} tp3_gcgprof_t, *ptp3_gcgprof_t;

/**
 * 통합제어기의 프로파일 정보 구조체를 세팅한다.
 * @param serial 시리얼번호
 * @param model 모델명
 * @param ipv4 IPv4 주소
 * @param ipv6 IPv6 주소
 * @param installsite 설치위치
 * @param mcutype mcu 종류
 * @param ostype 운영체제 종류
 * @param osver 운영체제 버전
 * @param ramsize 램사이즈
 * @param powertype 전원 타입
 * @param swver	소프트웨어 버전
 */
void
tp3_setgcgprof (ptp3_gcgprof_t pgcgprof, byte *serial, byte *model, byte *ipv4, byte *ipv6, int installsite, byte mcutype, byte ostype, int osver, byte ramsize, byte powertype, int swver);


/** 센서의 속성정보 구조체  */
typedef struct {
	int gsid;
	tp3_sentype_t sentype;
	tp3_sendatatype_t datatype;
	tp3_sengathertype_t gathertype;
	int gatherinterval;

	int maxval;
	int minval;
	int precision;
	int errrange;
	tp3_senthrsdtype_t thresholdtype;
	int threshold;
} tp3_senattr_t, *ptp3_senattr_t;

/** 구동기의 속성정보 구조체  */
typedef struct {
	int gaid;
	tp3_acttype_t acttype;
	tp3_actdatatype_t datatype;
	tp3_actgathertype_t gathertype;
	int gatherinterval;
} tp3_actattr_t, *ptp3_actattr_t;


/**
 * GCG 프로파일 정보를 메세지에서 읽습니다.
 * @param pmsg 메세지 구조체의 포인터
 * @param pgcgprof GCG 프로파일 구조체의 포인터
 * @return 상태코드 
 */
tp3_stat_t
tp3_readfields_gcgprof (ptp3_msg_t pmsg, ptp3_gcgprof_t pgcgprof);

/**
 * 센서 프로파일 정보를 메세지에서 읽습니다.
 * @param pmsg 메세지 구조체의 포인터
 * @param psenattr 센서 속성 구조체의 포인터
 * @return 상태코드 
 */
tp3_stat_t
tp3_readfields_senattr (ptp3_msg_t pmsg, ptp3_senattr_t psenattr);

/**
 * 구동기 프로파일 정보를 메세지에서 읽습니다.
 * @param pmsg 메세지 구조체의 포인터
 * @param pactattr 구동기 속성 구조체의 포인터
 * @return 상태코드 
 */
tp3_stat_t
tp3_readfields_actattr (ptp3_msg_t pmsg, ptp3_actattr_t pactattr);

/**
 * GCG 프로파일 정보를 메세지에 세팅합니다.
 * @param pmsg 메세지 구조체의 포인터
 * @param pgcgprof
 * @return 상태코드 
 */
tp3_stat_t
tp3_addfields_gcgprof (ptp3_msg_t pmsg, ptp3_gcgprof_t pgcgprof);

/**
 * 센서 프로파일 정보를 메세지에 세팅합니다.
 * @param pmsg 메세지 구조체의 포인터
 * @param psenattr 센서 속성 구조체의 포인터
 * @return 상태코드 
 */
tp3_stat_t
tp3_addfields_senattr (ptp3_msg_t pmsg, ptp3_senattr_t psenattr);

/**
 * 구동기 프로파일 정보를 메세지 세팅합니다.
 * @param pmsg 메세지 구조체의 포인터
 * @param pactattr 구동기 속성 구조체의 포인터
 * @return 상태코드 
 */
tp3_stat_t
tp3_addfields_actattr (ptp3_msg_t pmsg, ptp3_actattr_t pactattr);

/**
 * GID를 조합합니다.
 * @param nid 노드 아이디
 * @param devid 센서 아이디 혹은 구동기 아이디
 * @return gid
 */
int
tp3_getgid (int nid, int devid);

/**
 * GID에서 센서 혹은 구동기 아이디를 추출합니다.
 * @param gid 글로벌 아이디
 * @return sid or aid
 */
int
tp3_getdevid (int gid);

/**
 * GID에서 노드 아이디를 추출합니다.
 * @param gid 글로벌 아이디
 * @return nodeid
 */
int
tp3_getnid (int gid);

/** 하나의 노드에 속한 센서 및 제어기들의 현재정보를 다루기 위한 구조체 */
typedef struct {
	tp3_dev_t devtype;
	int numofdev;
	int *gids;				/**< GSID or GAID */
	tp3_devstat_t *stats;	/**< status of devices (sensors or actuators) */
	int *envs;				/**< sensor value, actuator value */
	void *attrs;			/**< ptp_senattr_t, ptp_actattr_t */
} tp3_nodedata_t, *ptp3_nodedata_t;

/** 센서노드 및 제어노드들의 현재정보를 다루기 위한 구조체 */
typedef struct {
	int numofdev;			/**< total number of devices */
	int numofnode;			/**< number of nodes */
	int *nids;				/**< snid, anid */
	tp3_nodestat_t *stats;	/**< status of nodes */
	tp3_nodedata_t *nodedata;
} tp3_nsetdata_t, *ptp3_nsetdata_t;

/**
 * 노드 정보 구조체를 초기화 한다.
 * @param pnode 노드 정보 구조체
 * @param numofdev 디바이스의 개수
 * @param gids 해당 디바이스의 글로벌 아이디들
 * @param devtype 디바이스 타입 (TP_DEV_SENSOR 와 TP_DEV_ACTUATOR 만가능)
 * @return 상태코드
 */
tp3_stat_t
tp3_initnodedata (ptp3_nodedata_t pnode, int numofdev, int *gids, tp3_dev_t devtype);

/**
 * 노드 정보 중 특정 디바이스의 환경정보값을 업데이트 한다.
 * @param pnode 노드 정보 구조체
 * @param gid 해당 디바이스의 글로벌 아이디
 * @param env 환경정보값
 * @return 상태코드
 */
tp3_stat_t
tp3_setdevenv (ptp3_nodedata_t pnode, int gid, int env);

/**
 * 노드 정보 중 특정 디바이스의 상태 정보값을 업데이트 한다.
 * @param pnode 노드 정보 구조체
 * @param gid 해당 디바이스의 글로벌 아이디
 * @param stat 상태정보값
 * @return 상태코드
 */
tp3_stat_t
tp3_setdevstat (ptp3_nodedata_t pnode, int gid, tp3_devstat_t stat);

/**
 * 노드 정보 중 특정 디바이스의 상태 정보값을 업데이트 한다.
 * @param pnode 노드 정보 구조체
 * @param gid 해당 디바이스의 글로벌 아이디
 * @param attr 속성정보에 대한 포인터 (ptp3_senattr_t or ptp3_actattr_t)
 * @return 상태코드
 */
tp3_stat_t
tp3_setdevattr (ptp3_nodedata_t pnode, int gid, void *attr);

/**
 * 센서 속성 정보를 세팅한다.
 * @param psenattr 센서 속성 정보 구조체
 * @param gsid 센서의 글로벌 아이디
 * @param sentype 센서의 타입
 * @param datatype 센서가 받아들일 수 있는 제어값의 타입
 * @param gathertype 데이터 수집 방법
 * @param gatherinterval 데이터 수집 주기 (수집방법이 주기적일때)
 * @param maxval 최대값
 * @param minval 최소값
 * @param precision 정밀도
 * @param errrange 에러값의 범위
 * @param thresholdtype 임계치 확인 방법
 * @param threshold 임계치값
 * @return 상태코드
 */
void
tp3_setsenattr (ptp3_senattr_t psenattr, int gsid, tp3_sentype_t sentype, tp3_sendatatype_t datatype, tp3_sengathertype_t gathertype, int gatherinterval, int maxval, int minval, int precision, int errrange, tp3_senthrsdtype_t thresholdtype, int threshold);

/**
 * 구동기 속성 정보를 세팅한다.
 * @param pactattr 구동기 속성 정보 구조체
 * @param gaid 구동기의 글로벌 아이디
 * @param acttype 구동기의 타입
 * @param datatype 구동기가 받아들일 수 있는 제어값의 타입
 * @param gathertype 데이터 수집 방법
 * @param gatherinterval 데이터 수집 주기 (수집방법이 주기적일때)
 * @return 상태코드
 */
void
tp3_setactattr (ptp3_actattr_t pactattr, int gaid, tp3_acttype_t acttype, tp3_actdatatype_t datatype, tp3_actgathertype_t gathertype, int gatherinterval);


/**
 * 노드 정보 구조체의 내부 데이터를 해제한다.
 * @param pnode 노드 정보 구조체
 */
void
tp3_releasenodedata (ptp3_nodedata_t pnode);

/**
 * 노드셋 정보 구조체를 초기화 한다.
 * @param pnset 노드셋 정보 구조체의 포인터
 */
void
tp3_initnset (ptp3_nsetdata_t pnset);

/**
 * 노드셋 정보 구조체에 노드정보를 업데이트한다.
 * 노드아이디가 없다면 새로 만들고, 노드아이디가 있다면 nodedata 를 덮어쓴다. 단, pnode 가 NULL 이라면 해당 노드아이디에 값을 제거한다.
 * @param pnset 노드셋  정보 구조체
 * @param nodeid 노드 아이디
 * @param pnode 노드 정보 구조체
 * @return 상태코드
 * @see tp3_delnodefromndata tp3_addnodetondata
 */
tp3_stat_t
tp3_updatenset (ptp3_nsetdata_t pnset, int nodeid, ptp3_nodedata_t pnode);

/**
 * 노드셋 정보 구조체에 노드를 추가한다. 
 * tp3_update_nsetdata 사용을 권장하며, 이 함수의 직접 사용을 권장하지 않는다. 
 * 노드 정보 구조체를 복사할때 딥카피를 하지 않기 때문에, 노드정보를 추가한뒤에 tp3_releasenodedata 를 실행하지 않아야 한다. 추후 tp3_delnodefromndata 수행시 자동으로 tp3_releasenodedata 가 호출된다.
 * @param pnset 노드셋  정보 구조체
 * @param nodeid 노드 아이디
 * @param pnode 디바이스정보 구조체에 대한 포인터
 * @return 상태코드
 * @see tp3_update_nsetdata
 */
tp3_stat_t
tp3_addnset (ptp3_nsetdata_t pnset, int nodeid, ptp3_nodedata_t pnode);
    
/**
 * 노드셋 정보 구조체에서 하나의 노드를 제거한다.
 * tp3_update_nsetdata 사용을 권장하며, 이 함수의 직접 사용을 권장하지 않는다. 
 * 노드 정보 구조체에 대한 해제도 동시에 수행된다.
 * @param pnset 노드셋  정보 구조체
 * @param nodeid 노드 아이디
 * @return 상태코드
 * @see tp3_update_nsetdata
 */
tp3_stat_t
tp3_delnset (ptp3_nsetdata_t pnset, int nodeid);

/**
 * 노드의 상태정보를 노드셋  정보 구조체에 세팅한다.
 * @param pnset 노드정보 구조체의 포인터
 * @param nodeid 노드 아이디 
 * @param nstat  노드의 상태
 * @return 상태코드. 
 */
tp3_stat_t
tp3_setnodestat (ptp3_nsetdata_t pnset, int nodeid, tp3_nodestat_t nstat);

/**
 * 노드셋 정보 구조체의 내부 메모리 해제
 * @param pnset 노드셋 정보 구조체의 포인터
 */
void
tp3_releasenset (ptp3_nsetdata_t pnset);

/**
 * 노드셋 정보 구조체를 메세지 프레임에 넣을 수 있는 바이트배열로 추출한다.
 * @param pnset 노드셋 정보 구조체의 포인터
 * @param fc tp3_fieldcode_t 의 값을 이용해서 결과 배열의 형태를 결정
 * @param length 결과값이 담길 바이트 배열의 길이를 돌려주기 위한 포인터
 * @param pbyte 결과값 바이트 배열의 포인터를 돌려주기 위한 포인터
 * @return 상태코드 
 */
tp3_stat_t
tp3_nsetdatatobytes(ptp3_nsetdata_t pnset, tp3_fieldcode_t fc, int *length, byte **pbyte);

/**
 * 노드셋 정보 구조체를 메세지 프레임에 넣을 수 있는 바이트배열로 추출하고, 해당 필드에 넣는다.
 * @param pmsg 메세지 구조체의 포인터
 * @param fc tp3_fieldcode_t 의 값을 이용해서 결과 배열의 형태를 결정
 * @param pnset 노드정보 구조체의 포인터
 * @return 상태코드 
 */
tp3_stat_t
tp3_addfield_nsetdata (ptp3_msg_t pmsg, tp3_fieldcode_t fc, ptp3_nsetdata_t pnset);

/**
 * 메세지에서 특정 필드의 배열형식의 데이터를 추출한다.
 * @param pmsg 메세지 구조체의 포인터
 * @param fc tp3_fieldcode_t 의 값을 이용해서 결과 배열의 형태를 결정
 * @param arr 값을 넣을 공간
 * @param maxsize arr의 크기
 * @return 읽은 아이템의 개수
 */
int
tp3_readfield_array (ptp3_msg_t pmsg, tp3_fieldcode_t fc, int *arr, int maxsize);

/**
 * 7바이트의 타임스탬프값을 time_t로 변환한다.
 * @param ptr 타임스탬프 문자열 포인터
 * @return epoch 타임
 */
time_t
tp3_getepoch (byte *ptr);

/**
 * 6바이트의 interval값을 초로 변환한다.
 * @param ptr interval 문자열 포인터
 * @return interval 초
 */
int
tp3_getinterval (byte *ptr);

/**
 * time_t 값을 7바이트의 타임스탬프값으로 변환하여 해당 필드에 넣는다.
 * @param pmsg 메세지 구조체의 포인터
 * @param fc 메세지 필드
 * @param stamp epoch 타임
 * @return 상태코드 
 */
tp3_stat_t
tp3_addfield_timestamp (ptp3_msg_t pmsg, tp3_fieldcode_t fc, time_t stamp);

/**
 * 시간간격값을 6바이트의 배열로 변환하여 해당 필드에 넣는다.
 * @param pmsg 메세지 구조체의 포인터
 * @param fc 메세지 필드
 * @param interval 타임인터벌 (초단위)
 * @return 상태코드 
 */
tp3_stat_t
tp3_addfield_interval (ptp3_msg_t pmsg, tp3_fieldcode_t fc, int interval);

#define _TP3_NUMOFBLKDATA	4

/** 상태정보이력조회 혹은 환경정보이력조회의 결과의 한 레코드를 표현하는 구조체 */
typedef struct {
	time_t timestamp;
	tp3_gcgstat_t gcgstat;
	int ids[_TP3_NUMOFBLKDATA][_TP3_MAX_DEVICES];
	int vals[_TP3_NUMOFBLKDATA][_TP3_MAX_DEVICES];
} tp3_blockitem_t, *ptp3_blockitem_t;

/** 상태정보이력조회 혹은 환경정보이력조회의 결과블럭을 만들기 위한 구조체 */
typedef struct {
	int rows;
	int numofdev[_TP3_NUMOFBLKDATA];
	ptp3_blockitem_t pitems;

	ptp3_blockitem_t pcuritem;
	int itemidx[_TP3_NUMOFBLKDATA];
} tp3_block_t, *ptp3_block_t;


/**
 * 상태정보이력조회 혹은 환경정보이력조회의 결과 블럭 구조체를 초기화한다.
 * 단, 환경정보이력시 numofan과 numofsn 값은 무시된다.
 * @param pblock 블럭 구조체의 포인터
 * @param rows 블럭내 레코드의 개수
 * @param numofsn 센서노드의 개수
 * @param numofsens 센서의 개수
 * @param numofan 제어노드의 개수
 * @param numofacts 구동기의 개수
 * @return 상태코드 
 */
tp3_stat_t
tp3_initblock (ptp3_block_t pblock, int rows, int numofsn, int numofsens, int numofan, int numofacts);

/**
 * 하나의 데이터블럭 아이템을 초기화한다.
 * @param pblock 블럭 구조체의 포인터
 * @param timestamp 타임스탬프
 * @param gcgstat 통합제어기 상태
 */
void
tp3_nextemptyblockitem (ptp3_block_t pblock, time_t timestamp, tp3_gcgstat_t gcgstat);

/**
 * 하나의 데이터블럭 아이템에 값을 추가한다.
 * @param pblock 블럭 구조체의 포인터
 * @param id 디바이스 아이디 (센서노드아이디, 제어노드아이디, 센서아이디, 액츄에이터아이디)
 * @param value 값 (상태값 혹은 환경값)
 * @param devtype 디바이스의 종류
 */
void
tp3_setblockitem (ptp3_block_t pblock, int id, int value, tp3_dev_t devtype);

/**
 * 상태정보이력 블럭바이트를 생성하고, 메세지 필드에 추가한다.
 * @param pmsg 메세지 구조체의 포인터
 * @param pblock 블럭 구조체의 포인터
 * @return 상태코드 
 */
tp3_stat_t
tp3_addfield_statblock (ptp3_msg_t pmsg, ptp3_block_t pblock);

/**
 * 환경정보이력 블럭바이트를 생성하고, 메세지 필드에 추가한다.
 * @param pmsg 메세지 구조체의 포인터
 * @param pblock 블럭 구조체의 포인터
 * @return 상태코드 
 */
tp3_stat_t
tp3_addfield_envblock (ptp3_msg_t pmsg, ptp3_block_t pblock);

/**
 * 블럭 정보 구조체의 내부 데이터를 해제한다.
 * @param pblock 블럭 정보 구조체
 */
void
tp3_releaseblock (ptp3_block_t pblock);

/**
 * 상태정보이력 블럭바이트를 파싱한다.
 * @param pblock 블럭 구조체의 포인터
 * @param rows 블럭아이템의 개수
 * @param ptr 메세지 필드 데이터에 대한 포인터
 * @param len 메세지 필드 데이터의 길이 
 * @return 상태코드 
 */
tp3_stat_t
tp3_parsestatblock (ptp3_block_t pblock, int rows, byte *ptr, int len);

/**
 * 환경정보이력 블럭바이트를 파싱한다.
 * @param pblock 블럭 구조체의 포인터
 * @param rows 블럭아이템의 개수
 * @param ptr 메세지 필드 데이터에 대한 포인터
 * @param len 메세지 필드 데이터의 길이 
 * @return 상태코드 
 */
tp3_stat_t
tp3_parseenvblock (ptp3_block_t pblock, int rows, byte *ptr, int len);

#endif
