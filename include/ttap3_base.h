/** 
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
  \file ttap3_base.h
  \brief TTAK.KO-06.0288 Part 3 를 구현한 라이브러리 베이스

    TTA Part 3 라이브러리 베이스에서는 실제 전송되는 데이터와
	프로그램상에서 사용하는 메세지의 구조체를 상호변환 해주기 위한
	코드를 담고 있다.
  */

#ifndef __LIB_TTA_P3_BASE_H__
#define __LIB_TTA_P3_BASE_H__

typedef unsigned char byte;

/** 
 * 메세지 전송 단위 타입
 * length, data, checksum 으로 이루어진다.
 * data 는 checksum 을 포함한다.
 */
typedef struct {
	int length;			///< 메세지 프레임의 데이터부분 길이. +4를 해야 전체 길이임
	byte *data;			///< 메세지 프레임의 데이터. 맨 마지막 2바이트 체크썸을 포함.
	int checksum;		///< 메세지 프레임 데이터의 체크썸
} tp3_frame_t, *ptp3_frame_t;

/** 
 * 메세지 구성 단위 타입
 * 메세지는 다수의 필드로 구성된다.
 * length, data, checksum 으로 이루어진다.
 * data 는 checksum 을 포함한다.
 */
typedef struct {
	unsigned int code:16;		///< 메세지 프레임 필드의 코드
	unsigned int length:16;		///< 메세지 프레임 필드의 길이
	byte data;					///< 메세지 프레임 필드의 데이터의 첫 데이터. 해당 위치를 알기 위해서 사용됨.
} tp3_framefield_t, *ptp3_framefield_t;

/** 2 byte x에 y를 세팅 */
#define tp3_set2bytes(x,y) (x)[1] = (y) & 0xFF; (x)[0] = (((y) & 0xFF00) >> 8);
/** 3 byte x에 y를 세팅 */
#define tp3_set3bytes(x,y) (x)[2] = (y) & 0xFF; (x)[1] = (((y) & 0xFF00) >> 8);\
					(x)[0] = (((y) & 0xFF0000) >> 16);
/** 4 byte x에 y를 세팅 */
#define tp3_set4bytes(x,y) (x)[3] = (y) & 0xFF; (x)[2] = (((y) & 0xFF00) >> 8);\
					(x)[1] = (((y) & 0xFF0000) >> 16); (x)[0] = (((y) & 0xFF000000) >> 24);

/**
 * byte 배열에 세팅된 숫자값을 읽는다.
 * @param pbyte 바이트 배열 포인터
 * @param len 바이트 길이
 * @return 숫자값
 */
int
tp3_readnum (byte *pbyte, int len);

#define _TP3_HEADER_INTERFACE_LEN 10
#define _TP3_HEADER_DATESTR_LEN	8
#define _TP3_HEADER_TIMESTR_LEN	6
#define _TP3_HEADER_MSGSEQ_LEN	6

#define _TP3_HEADER_LENGTH		70			// 2 * 8 + 2 * 8 + 38
#define _TP3_FIELDINFO_LENGTH	4
#define _TP3_CHECKSUM_LENGTH	2
#define _TP3_MAX_ITEM			256

#define _TP3_INTERFACETYPE_LEN	7
#define _TP3_INTERFACETYPE		"GOS_GCG   "

#define _TP3_MSGSEQ_MAX			999999
#define _TP3_HEXCHARS 			"0123456789ABCDEF"

#define _TP3_FIXED_HEADER_LEN 	8


/** 필수 고정 헤더 데이터 타입.
  메세지 프레임을 파싱하면 고정헤더와 가변필드를 얻을 수 있다.
  */
typedef struct {
	char ifacetype[_TP3_HEADER_INTERFACE_LEN];	///< 메세지 구분자
	tp3_msgtype_t msgtype;						///< 메세지 타입
	tp3_transtype_t transtype;					///< 메세지 전송타입
	int gcgid;									///< 20bit 통합제어기 식별자
	int gosid;									///< 20bit 운영시스템 식별자
	char tdate[_TP3_HEADER_DATESTR_LEN];		///< 전송날짜
	char ttime[_TP3_HEADER_TIMESTR_LEN];		///< 전송시간
	int msgseq;									///< 메세지 일련번호

	tp3_rescode_t rescode;						///< 메세지 응답코드 (고정헤더는 아니지만 편의를 위해 포함됨)
} tp3_header_t, *ptp3_header_t;

/*
#define TP3_ID_SIZE		3
#define TP3_GID_SIZE	4
#define TP3_SVAL_SIZE	4
#define TP3_AVAL_SIZE	2
*/

/** 가변필드 데이터를 위한 타입.
  가변필드 데이터의 경우 메모리 할당을 줄이기 위해 4바이트까지는 값으로 저장하고,
  4바이트초과의 경우에만 메모리를 할당하여 처리한다.
 */
typedef union {
	int data;			///< 1~4 바이트
	byte *pbyte;		///< N 바이트
} tp3_fdata_t, *ptp3_fdata_t;

/** 가변필드를 위한 필드데이터 타입 */
typedef struct {
	tp3_fieldcode_t code;		///< 필드 코드
	int length;					///< 필드 데이터 길이
	tp3_fdata_t data;			///< 데이터
} tp3_field_t, *ptp3_field_t;

/** 메세지 프레임이 파싱되어 저장되는 메세지 타입 
  성능을 위해 가변 필드는 _TP3_FILED_STEP 만큼씩 커진다.
  따라서, 가변 필드의 전체 크기는 가변 필드의 실제 개수보다 크다.

  @see _TP3_FIELD_STEP
 */
typedef struct {
	tp3_header_t header;		///< 메세지 고정 헤더
	ptp3_field_t fields;		///< 메세지 가변 필드
	int numoffields;			///< 가변 필드의 실제 개수
	int sizeoffields;			///< 가변 필드의 전체 크기 
} tp3_msg_t, *ptp3_msg_t;

/** expr 이 에러라면 에러 상황을 출력하고 해당에러를 리턴 */
#define TP3_STAT_NOERROR(expr)		\
	do {						\
		tp3_stat_t __stat;	\
		if ((__stat = expr) != TP3_SC_NOERROR) {			\
			fprintf (stderr, "Status error (%s) returned in %s on line %d: %s\n",      \
				tp3_geterrormsg (__stat),	\
				__FILE__,               \
				__LINE__,               \
				#expr);             	\
			return __stat;				\
		}								\
	} while (0);
	
/** expr 이 참이라면 stat 에 해당하는 에러 상황을 출력하고 해당에러를 리턴 */
#define TP3_STAT_RETURN(expr,stat)	\
	do {						\
		if ((expr)) {			\
			fprintf (stderr, "Status error (%s) occured in %s on line %d: %s\n",      \
				tp3_geterrormsg (stat),	\
				__FILE__,               \
				__LINE__,               \
				#expr);             	\
			return stat;				\
		}								\
	} while (0);

/** stat 에 해당하는 에러와 msg 의 문자열을 출력하고 stat의 값을 리턴 */
#define TP3_STAT_PRETURN(msg,stat)  \
	do {                        \
		fprintf (stderr, "Status error (%s) occured in %s on line %d: %s\n",      \
			tp3_geterrormsg (stat),  \
			__FILE__,               \
			__LINE__,               \
			msg);                   \
		return stat;				\
	} while (0);

/** expr 이 참이라면 stat 에 해당하는 에러 상황을 출력하고 ret를 리턴 */
#define TP3_EXP_RETURN(expr,ret,stat)  \
	do {                        \
		if ((expr)) {			\
			fprintf (stderr, "Status error (%s) occured in %s on line %d.\n",      \
				tp3_geterrormsg (stat),  \
				__FILE__,               \
				__LINE__);              \
			return ret;					\
		}								\
	} while (0);

/**
 * 메세지를 전송하기 위해서 메세지 프레임을 바이트 배열로 만들때의 길이를 알려준다.
 * @param pframe 타겟 프레임
 * @return 메세지 프레임을 바이트 배열로 만들었을때의 길이
 */
int
tp3_getframesize (ptp3_frame_t pframe);

/**
 * 메세지를 전송하기 위해서 메세지 프레임을 바이트 배열에 기록한다.
 * @param pframe 타겟 프레임
 * @param buf 바이트 배열
 * @param len 길이
 * @return 상태코드, TP3_SC_FRAMEFMT_ERR 를 리턴하는 경우 버퍼의 길이가 부족한 것임.
 */
tp3_stat_t
tp3_writeframe (ptp3_frame_t pframe, byte *buf, int len);

/**
 * 전송된 바이트 배열을 읽어서 메세지 프레임을 구성한다.
 * @param pframe 타겟 프레임
 * @param buf 바이트 배열
 * @param len 길이
 * @return 버퍼에서 사용한 길이. 음수라면 부족한 버퍼의 길이. 0 이라면 메세지 프레임을 위한 메모리 할당 실패.
 */
int
tp3_readframe (ptp3_frame_t pframe, byte *buf, int len);

/**
 * 메세지를 이용하여 전송하기 위한 프레임을 셋업한다.
 * 프레임 자체를 위한 메모리를 할당받지는 않는다.
 * @param pmsg 소스 메세지
 * @param pframe 타겟 프레임
 * @see tp3_parseframe
 * @return 상태코드
 */
tp3_stat_t
tp3_generateframe (ptp3_msg_t pmsg, ptp3_frame_t pframe);

/**
 * 전송받은 프레임을 파싱하여 메세지를 셋업한다.
 * 메세지 자체를 할당받지는 않는다.
 * @param pframe 소스 프레임
 * @param pmsg 타겟 메세지
 * @see tp3_generateframe
 * @return 상태코드, checksum 이 맞지 않으면 TP3_SC_CHECKSUM_NOT_MATCHED, \
 인터페이스 문자열이 맞지 않으면 TP3_SC_INTERFACETYPE_NOT_MATCHED 를 리턴함
 */
tp3_stat_t
tp3_parseframe (ptp3_frame_t pframe, ptp3_msg_t pmsg);

/**
 * 메세지 내부에서 필드데이터를 위해 할당된 메모리를 해제한다.
 * 메세지 포인터 자체를 해제하지는 않는다.
 * @param pmsg 타겟 메세지
 * @see tp3_releaseframe
 */
void
tp3_releasemsg (ptp3_msg_t pmsg);

/**
 * 프레임 내부에 필드데이터를 위해 할당된 메모리를 해제한다.
 * 프레임 포인터 자체를 해제하지는 않는다.
 * @param pframe 타겟 프레임
 * @see tp3_releasemsg
 */
void
tp3_releaseframe (ptp3_frame_t pframe);

/**
 * 메세지에 필드를 추가한다.
 * @param pmsg 타겟 메세지
 * @param code 추가할 필드코드
 * @param length 추가할 필드데이터의 길이
 * @param data 추가할 데이터
 * @see
 * @return 상태코드
 */
tp3_stat_t
tp3_addfield (ptp3_msg_t pmsg, tp3_fieldcode_t code, int length, ptp3_fdata_t data);

/**
 * 메세지에 4byte 이내의 데이터를 추가한다.
 * @param pmsg 타겟 메세지
 * @param code 추가할 필드코드
 * @param data 추가할 데이터
 * @see
 * @return 상태코드
 */
tp3_stat_t
tp3_addfield_num (ptp3_msg_t pmsg, tp3_fieldcode_t code, int data);

/**
 * 메세지에 길이를 알고있는 bytes 데이터를 추가한다.
 * @param pmsg 타겟 메세지
 * @param code 추가할 필드코드
 * @param ptr 추가할 데이터의 포인터
 * @see
 * @return 상태코드
 */
tp3_stat_t
tp3_addfield_bytes (ptp3_msg_t pmsg, tp3_fieldcode_t code, byte *ptr);

/**
 * 메세지에 nbyte 데이터 필드를 추가한다.
 * @param pmsg 타겟 메세지
 * @param code 추가할 필드코드
 * @param length 추가할 필드데이터의 길이
 * @param ptr 추가할 데이터의 포인터
 * @see
 * @return 상태코드
 */
tp3_stat_t
tp3_addfield_nbytes (ptp3_msg_t pmsg, tp3_fieldcode_t code, int length, byte *ptr);


/**
 * 메세지를 초기화 한다.
 * @param pmsg 초기화할 메세지
 * @see tp3_setheader
 */
void
tp3_initmsg (ptp3_msg_t pmsg);

/**
 * 메세지프레임을 초기화 한다.
 * @param pframe 초기화할 메세지 프레임
 * @see tp3_initmsg
 */
void
tp3_initframe (ptp3_frame_t pframe);

/**
 * 메세지에 고정해더를 세팅한다.
 * @param pmsg 세팅할 메세지
 * @param msgtype 메세지타입
 * @param transtype 메세지 전송타입
 * @param gcgid 통합제어기 식별자
 * @param gosid 운영시스템 식별자
 * @param msgseq 메세지 일련번호
 * @see tp3_addfield
 * @return 상태코드
 */
tp3_stat_t
tp3_setheader (ptp3_msg_t pmsg, tp3_msgtype_t msgtype, tp3_transtype_t transtype, int gcgid, int gosid, int msgseq);

/**
 * 메세지의 가변필드에서 데이터를 읽는다.
 * @param pmsg 읽을 메세지
 * @param code 읽을 필드코드
 * @param pdata 읽은 데이터를 저장할 위치
 * @return 읽은 필드의 길이. -1 일 경우 해당 필드코드가 메세지상에 없는 것임.
 */
int
tp3_readfield (ptp3_msg_t pmsg, tp3_fieldcode_t code, ptp3_fdata_t pdata);

/**
 * 메세지의 가변필드에서 숫자를 읽는다.
 * @param pmsg 읽을 메세지
 * @param code 읽을 필드코드
 * @param pdata 읽은 데이터를 저장할 위치
 * @return 읽은 필드의 길이. -1 일 경우 해당 필드코드가 메세지상에 없는 것임. -2라면 해당필드가 4bytes 이상이라 정수변환이 안된다는 의미.
 */
int
tp3_readfield_num (ptp3_msg_t pmsg, tp3_fieldcode_t code, int *pdata);

/**
 * 메세지 가변필드의 길이를 알려준다.
 * @param pmsg 읽을 메세지
 * @param code 읽을 필드코드
 * @return 필드의 길이. 해당 필드가 없다면 -1을 리턴.
 */
int
tp3_readfieldlen (ptp3_msg_t pmsg, tp3_fieldcode_t code);

/**
 * 메세지 가변필드에 들어있는 값이 숫자형인지 확인한다.
 * 이는 tp3_fdata_t를 바이트배열로 읽을 것인지 숫자형 데이터로 읽을 것인지를 확인하는 용도로 사용된다.
 * @param pmsg 읽을 메세지
 * @param code 읽을 필드코드
 * @return 0이면 바이트배열형, 1이면 숫자형. 해당 필드가 없다면 -1을 리턴.
 */
int
tp3_isnumberinfield (ptp3_msg_t pmsg, tp3_fieldcode_t code);

/**
 * 확인을 위해 메세지를 콘솔에 출력한다.
 * @param pmsg 출력할 메세지
 * @see tp3_printframe
 */
void
tp3_printmsg (ptp3_msg_t pmsg);

/**
 * 확인을 위해 프레임을 콘솔에 출력한다.
 * @param pframe 출력할 프레임
 * @see tp3_printmsg
 */
void
tp3_printframe (ptp3_frame_t pframe);

/**
 * 확인을 위해 에러내역을 문자열로 반환합니다.
 * @param stat 상태코드
 * @return 에러내용 문자열
 */
char *
tp3_geterrormsg (tp3_stat_t stat);

/**
 * 확인을 위해 에러내역을 출력합니다.
 * @param stat 상태코드
 */
void
tp3_printerror (tp3_stat_t stat);

/**
 * 요청에 대한 응답을 생성합니다. 
 * @param preq 요청
 * @param pres 요청에 대한 응답
 * @param rescode 요청에 대한 응답코드
 * @return 상태코드. 
 */
tp3_stat_t
tp3_setresponse (ptp3_msg_t preq, ptp3_msg_t pres, tp3_rescode_t rescode);

#endif
