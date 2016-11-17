# libtp3 : A library for TTAK.KO-06.0288 Part 3

## Introduction

libtp3 는 온실관제시스템 표준인 TTAK.KO-06.0288/R1 Part 3 표준을 구현한 라이브러리로, 온실 운영시스템과 온실 통합제어기 사이의 통신 메세지를 작성, 해석할 수 있다. 해당 표준에 대한 자세한 사항은 [TTA 표준문서](http://www.tta.or.kr/data/ttas_view.jsp?rn=1&pk_num=TTAK.KO-06.0288-Part3)를 참조하길 바란다.


## libtp3의 구성
libtp3 는 5가지 모듈로 구성된다.
 - ttap3_base : 표준 메세지 구조를 처리할 수 있는 데이터 타입, 상수 및 함수 관련 모듈
 - ttap3_util : 라이브러리에서 사용할 유틸리티 함수 및 관련 데이터 타입
 - ttap3_process : 메세지 처리를 위한 콜백함수 관련 모듈 
 - ttap3_gcg : 온실통합제어기에서 사용할 API
 - ttap3_gos : 온실운영시스템에서 사용할 API

## Dependency
libtp3 는 비동기 이벤트방식의 I/O를 위해서 libuv를 사용합니다.
* [libuv](https://github.com/libuv/libuv)
다음과 같이 설치가 가능하다.
```
wget https://github.com/libuv/libuv/archive/v1.x.zip
unzip v1.x.zip -d v1.x
cd v1.x/libuv-1.x
./autogen.sh
./configure
make
make install
```

## Build Instructions

```
mkdir release
cd release
cmake ..
make
make install
```

## Documentation
libtp3의 사용법은 [레퍼런스](http://)를 참조한다.

## Unit tests & Sample
libtp3는 다음 명령을 통해 단위테스트를 수행할 수 있다.
단위테스트를 보고 사용법을 확인하는 것도 한가지 방법이다.

```
make test
```

### Base test 
 - test_base_frame : Generate & Parse frame
 - test_base_addnreadfield : Add field using 4 functions & read the field
 - test_base_errormsg : Error message
 - test_base_response : Response message creation

### Util
 - test_util_nodedata : Initialize & release data for a node
 - test_util_nsetdata : Initialize & release data for a node set
 - test_util_addfield : Add field from node data
 - test_util_timestamp : Translate between epoch & timestamp
 - test_util_statblock : Generate & parse status history block
 - test_util_envblock : Generate & parse environment history block

### GCG & GOS
 - test_gng_connect : Connection message
 - test_gos_resmsgs : Response 4 messages
 - test_gos_genmsgs : Generate 8 messages started from GOS
 - test_gcg_resmsgs : Response 8 messages
 - test_gos_cfmmsgs : Confirm 4 meesages

### Process
 - test_process_callback : Callback Test


