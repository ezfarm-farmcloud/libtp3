/** 
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
  \file ttap3_config.h
  \brief libtp3 구동을 위해 변경할 수 있는 설정값 정의
	
	libtp3에서 사용하는 버퍼의 크기등을 정의하고 있는 파일
 */

#ifndef __LIB_TTA_P3_CONFIG_H__
#define __LIB_TTA_P3_CONFIG_H__

#define _TP3_MAX_GCG		3		//< 연결가능한 통합제어기수 
#define _TP3_MAX_DEVICES	32		//< 연결가능한 센서수, 연결가능한 구동기수의 두가지 중 더 많은 것의 개수 예) 센서 10개, 구동기 5개라면 10이 됨. 모를 경우 적당히 큰 값으로 하면 됨.

#define _TP3_BUFSIZE 		50		//< 내부 버퍼 크기 (시리얼, 모델번호 등에 사용됨)

#define _TP3_FIELD_STEP     10		//< 가변필드의 스텝. 키우면 메모리 할당이 줄어들어서 성능이 올라가지만 공간 낭비가 있을 수 있음.

#define _TP3_FDATA_LIMIT	4		//< tp3_fdata_t 의 경계로 sizeof(int)와 같음. 즉, 4byte 이하면 data 에 저장하고, 그 이상이면 동적메모리 할당을 사용하여 pbyte에 저장함. 

#endif
