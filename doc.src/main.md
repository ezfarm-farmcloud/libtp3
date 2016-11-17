A library for TTAK.KO-06.0288 Part 3		{#mainpage}
====================================

# libtp3

libtp3 는 온실관제시스템 표준인 TTAK.KO-06.0288/R1 Part 3 표준을 구현한 라이브러리로, 온실 운영시스템과 온실 통합제어기 사이의 통신 메세지를 작성, 해석할 수 있다. 해당 표준에 대한 자세한 사항은 [TTA 표준문서](http://www.tta.or.kr/data/ttas_view.jsp?rn=1&pk_num=TTAK.KO-06.0288-Part3)를 참조하길 바란다.

# libtp3의 구성
libtp3 는 5가지 모듈로 구성된다.
 - ttap3_base : 표준 메세지 구조를 처리할 수 있는 데이터 타입, 상수 및 함수 관련 모듈
 - ttap3_util : 라이브러리에서 사용할 유틸리티 함수 및 관련 데이터 타입
 - ttap3_process : 메세지 처리를 위한 콜백함수 관련 모듈 
 - ttap3_gcg : 온실통합제어기에서 사용할 API
 - ttap3_gos : 온실운영시스템에서 사용할 API

# Implementation 
## Data structures
 libtp3 는 크게 2가지 메세지 표현을 위한 자료구조를 가지고 있다. 하나는 실제 통신에서 사용할 byte의 배열에 해당하는 자료구조로 [tp3_frame_t](@ref tp3_frame_t) 타입으로 표현되는 것이고, 다른 하나는 프로그램 코드상에서 메세지에 쉽게 접근할 수 있도록 돕는 자료구조로 [tp3_msg_t](@ref tp3_msg_t) 타입이다.

### tp3_frame_t
 [TTAK.KO-06.0288/R1 Part 3 표준](http://www.tta.or.kr/data/ttas_view.jsp?rn=1&pk_num=TTAK.KO-06.0288-Part3)에서는 메세지 프레임이 3가지 값(길이, 데이터, 체크섬)으로 구성된다. 길이는 4byte 정수이며, 데이터의 길이와 체크섬 2바이트를 합친 길이이다. 체크섬은 데이터의 바이트 단위 값을 XOR 한 값을 2byte로 표현한 것이다. 
 데이터는 다수의 필드로 구성되는데, 개별 필드 역시 3가지 값(필드코드, 필드길이, 필드데이터)으로 구성된다. 필드코드는 2byte 로 부여되는 값으로 [tp3_fieldcode_t](@ref tp3_fieldcode_t) 으로 정의된다. 필드길이는 2byte 정수형으로 필드데이터의 길이를 나타낸다. 필드데이터는 필드코드에 따라 길이가 고정되지만 몇몇 필드에 있어서는 동적으로 길이가 결정된다.

### tp3_msg_t
 [tp3_msg_t](@ref tp3_msg_t) 타입은 [tp3_frame_t](@ref tp3_frame_t)로 표현되는 메세지 프레임을 프로그램 코드상에서 쉽게 접근할 수 있도록 파싱한 결과값이라고 할 수 있다. 고정헤더필드와 가변필드로 구성된다. 고정헤더필드는 [tp3_header_t](@ref tp3_header_t)로 표현되며, 가변필드는 [tp3_field_t](@ref tp3_field_t)의 배열로 표현된다.

## Translation
 [tp3_frame_t](@ref tp3_frame_t) 와 [tp3_msg_t](@ref tp3_msg_t) 간의 전환은 두 함수 [tp3_parseframe](@ref tp3_parseframe) 과 [tp3_generateframe](@ref tp3_generateframe) 으로 이루어진다. [tp3_parseframe](@ref tp3_parseframe) 함수는 [tp3_frame_t](@ref tp3_frame_t) 을 [tp3_msg_t](@ref tp3_msg_t) 으로 파싱해주는 역할을 수행하며,  [tp3_generateframe](@ref tp3_generateframe) 함수는 [tp3_msg_t](@ref tp3_msg_t)을 이용하여 [tp3_frame_t](@ref tp3_frame_t) 를 생성해주는 역할을 수행한다. 

## Request Message Creation
 [TTAK.KO-06.0288/R1 Part 3 표준](http://www.tta.or.kr/data/ttas_view.jsp?rn=1&pk_num=TTAK.KO-06.0288-Part3) 에는 12가지 형식의 메세지가 정의되어 있다. 대부분의 메세지는 요청, 응답형식의 메세지로, 메세지의 종류에 따라 운영시스템 혹은 통합제어기 측에서 전송을 시작하게 된다. 통합제어기에서 전송할 수 있는 메세지는 4종이며, 운영시스템에서 전송할 수 있는 메세지는 8종이다. 각 메세지의 생성을 위해서는 다음의 메세지 생성 함수를 활용할 수 있다.

### 통합제어기
 - 연결요청 : [tp3_connectmsg](@ref tp3_connectmsg)
 - 네트워크 체크 : [tp3_keepalivemsg](@ref tp3_keepalivemsg)
 - 상태정보 알림 : [tp3_notifystatmsg](@ref tp3_notifystatmsg)
 - 환경정보 알림 : [tp3_notifyenvmsg](@ref tp3_notifyenvmsg)

### 운영시스템
 - 상태정보 이력 : [tp3_stathistmsg](@ref tp3_stathistmsg)
 - 환경정보 이력 : [tp3_envhistmsg](@ref tp3_envhistmsg)
 - 프로파일 조회 : [tp3_gcgconf](@ref tp3_gcgconf), [tp3_snodeconf](@ref tp3_snodeconf), [tp3_anodeconf](@ref tp3_anodeconf), [tp3_gcgattr](@ref tp3_gcgattr), [tp3_senattr](@ref tp3_senattr), [tp3_actattr](@ref tp3_actattr)
 - 프로파일 설정 : [tp3_senattrupdate](@ref tp3_senattrupdate), [tp3_actattrupdate](@ref tp3_actattrupdate)
 - 장애 관리 : [tp3_faultmng](@ref tp3_faultmng)
 - 엑추에이터 제어 : [tp3_actcontrol](@ref tp3_actcontrol)
 - 상태정보 요청 : [tp3_statquery](@ref tp3_statquery)
 - 환경정보 요청 : [tp3_envquery](@ref tp3_envquery)

## Response Message
 위의 메세지 생성함수에서 생성된 메세지는 2가지 방식으로 처리될 수 있다. 하나는 콜백함수를 사용하는 것이고, 다른 하나는 직접 메세지를 파싱하고, 처리하여 응답메세지를 만드는 방법이다. 콜백함수를 사용하는 경우 해당 메세지에 대한 직접적인 처리만을 수행할 수 있기 때문에 더 편하다. 직접 메세지를 처리하는 것은 특별한 이유가 있지 않는 한 권장하지 않는다.

### 콜백 함수의 사용
 메세지 처리를 위한 콜백함수는 다음과 같이 정의된다.
~~~~~~~~~~~~~~{.c}
typedef tp3_stat_t 
(*ptp3_rescb_t) (ptp3_msg_t preq, ptp3_msg_t pres, void *data);
~~~~~~~~~~~~~~
 preq는 요청메세지에 대한 포인터이고, pres는 생성할 응답메세지에 대한 포인터 이다. data는 사용자 정의 데이터에 대한 포인터로 사용자가 사전에 등록해 놓은 포인터를 전달한다.  콜백함수는 [tp3_registcbfunc](@ref tp3_registcbfunc)를 통해서 등록할 수 있으며, 이때 사용자 정의 데이터도 등록이 가능하다.

### 요청 메세지의 전/후처리
 요청 메세지를 [tp3_frame_t](@ref tp3_frame_t) 형식으로 전달받게 되면, 해당 변수를 [tp3_msg_t](@ref tp3_msg_t) 형식으로 파싱한다. 파싱된 메세지를 이용하여 응답 메세지 헤더를 설정할 수 있고, 메세지를 처리한뒤, 응답코드를 설정한 [tp3_msg_t](@ref tp3_msg_t) 형식의 변수를 만든다. [tp3_msg_t](@ref tp3_msg_t) 타입의 응답 메세지를 이용하여 [tp3_frame_t](@ref tp3_frame_t) 형식의 메세지를 생성하면 요청메세지에 대한 처리가 완료된다. 이를 코드로 정리하면 다음과 같다.

~~~~~~~~~~~~~~{.c}
tp3_frame_t frame;
tp3_msg_t req, res;
tp3_stat_t stat;

.....

tp3_parseframe (&frame, &req);

tp3_initmsg (&res);

stat = tp3_executecallback (&req, &res);
tp3_setresponse (&req, &res, stat);

tp3_generateframe (&res, &frame);
.....
~~~~~~~~~~~~~~

# Device Type
 [TTAK.KO-06.0288/R1 Part 3 표준](http://www.tta.or.kr/data/ttas_view.jsp?rn=1&pk_num=TTAK.KO-06.0288-Part3)에서는 센서의 타입이나 제어기의 타입을 정의하고 있지 않다. libtp3 에서는 다음과 같이 센서와 제어기의 타입을 정의하여 사용하고 있다.

## Sensor Type

코드 | 값 | 대상
-----|------|------
TP3_SENT_RAW | 0x00 | RAW
TP3_SENT_INTTEMP| 0x01 | 내부온도
TP3_SENT_INTDEWPNT | 0x02 | 내부이슬점
TP3_SENT_INTREHUM | 0x03 |  내부상대습도
TP3_SENT_EXTTEMP | 0x04 | 외부온도
TP3_SENT_EXTHUM | 0x05 | 외부습도
TP3_SENT_WINDDIR | 0x08 | 풍향
TP3_SENT_WINDSPD | 0x09 | 풍속
TP3_SENT_ILLUM | 0x0A | 조도
TP3_SENT_SOLRAD | 0x0B | 일사량
TP3_SENT_PRECIP | 0x0C | 강우

## Actuator Type

코드 | 값 | 작동방식
-----|------|------
TP3_ACTT_RAW | 0x00 | RAW
TP3_ACTT_ONOFF | 0x01 | 온/오프
TP3_ACTT_TIME | 0x02 | 초단위 시간제어
TP3_ACTT_PERCENT | 0x03 |% 제어

# How to build
~~~~~~~~~~~~~~~
cmake .
make clean
make
make test
make doc
make install
~~~~~~~~~~~~~~~

# Unit tests & Sample
## Base test 
 - test_base_frame : Generate & Parse frame
 - test_base_addnreadfield : Add field using 4 functions & read the field
 - test_base_errormsg : Error message
 - test_base_response : Response message creation

## Util
 - test_util_nodedata : Initialize & release data for a node
 - test_util_nsetdata : Initialize & release data for a node set
 - test_util_addfield : Add field from node data
 - test_util_timestamp : Translate between epoch & timestamp
 - test_util_statblock : Generate & parse status history block
 - test_util_envblock : Generate & parse environment history block

## GCG & GOS
 - test_gng_connect : Connection message
 - test_gos_resmsgs : Response 4 messages
 - test_gos_genmsgs : Generate 8 messages started from GOS
 - test_gcg_resmsgs : Response 8 messages
 - test_gos_cfmmsgs : Confirm 4 meesages

## Process
 - test_process_callback : Callback Test

# How to use

