TTAK.KO-06.0288/R1 Part 3 분석 결과		{#P3R1analysis}
===================================

# 개요
TTAK.KO-06.0288/R1은 TTAK.KO-06.0288의 다음 버전 표준으로 한국정보통신기술협회의 승인을 기다리고 있다. 본 문서는 Cflora의 개발을 위하여 TTAK.KO-06.0288/R1표준 중 Part 3를 중심으로 구현이슈를 분석한 결과이다. 개정 전 버전에 대한 분석결과는 TTA 파트 3 분석결과에서 확인할 수 있다. 


# 구현 이슈의 유형
TTA 표준 구현시 발생할 수 있는 이슈들을 5가지 유형으로 구분하여 정리하였다.
 - 표준간 통일성 이슈 : 표준간 정의가 달라 문제 발생의 여지가 있는 이슈

 - 단일 표준내 충돌 이슈 : 단일 표준내에서 동일한 항목에 대한 설명이 상이하여 그 중 한 가지를 취할 수 없는 이슈

 - 운영시 발생 가능한 이슈 : 구현후 운영시에 발생할 것으로 기대되는 이슈

 - 오탈자 이슈 : 표준문서상에서 오탈자에 관한 것으로 표준 구현자에게 착각을 불러일으킬 수 있는 이슈

 - 기타 이슈: 상기의 4가지 유형에 포함되지 않는 이슈

# 유형별 이슈 리스트 
## 표준간 통일성 이슈
 - 파트 1,3: 파트 3의 센서 노드 프로파일 필드중 센서 유형, 센서 데이터 유형, 수집 유형, 수집 주기, 센서값 상한, 센서값 하한, 정밀도, 오차 범위 등은 TTA 파트 1에서 전송되지 않기 때문에 온실통합제어기에서 알 수 있는 방법이 없음. (파트3 표 6-5)

 - 파트 2, 3: 파트3의 제어 노드 프로파일 필드중 액추에이터 유형, 액추에이터 데이터 유형은 TTA 파트 2에서 전송되지 않기 때문에 온실통합제어기에서 알 수 있는 방법이 없음. (파트3 표 6-6)

 - 파트 1, 3: 파트 1의 센서값은 2 byte이나 파트 3의 센서값은 4byte이기 때문에 값의 범위를 벗어나는 경우 상호간 데이터 전송시 해당값을 제대로 표시하지 못하는 문제가 발생 가능함. 특히, 음수나 실수형에 대한 표현방식이 정해지지 않아 음수나 실수값 전송시 문제가 될 수 있음.

 - 개정전 버전에서는 센서타입에 2바이트를 할당하고 있는데, 개정버전에서는 센터타입에 1바이트를 할당하고 있음. 액추에이터 타입도 마찬가지임.

## 단일 표준내 충돌 이슈

 - 표 6-5와 표 6-6의 센서노드 프로파일과 제어노드 프로파일 비슷한 유형의 항목을 서로 다르게 취급하고 있음. 예를 들어 SenDataType 과 ActDataType의 경우, SenDataGatheringType과 ActDataGatheringType 이 이런 유형의 예임.

 - 8.5절과 8.6절의 이력정보처리에 있어서 상태정보 미전송내역과 환경정보 미전송내역이 마치 운영시스템의 요청없이 전송되는 것 처럼 작성되어 있는데, 이는 메세지 전송 절차에 위배되는 것으로 보인다.

 - 7.7.1 메세지와 7.8.1 메세지를 구분할 방법이 없다. 

# 운영시 발생 가능한 이슈

 - 개정된 표준과 이전 표준간의 차이가 있을텐데, 이를 구분할 수 있는 표시가 없음. 인터페이스 타입 문자열등에 표준에 대한 확인 표시가 포함되어야 할 것으로 생각됨.

 - 표 6-5의 Field Code 0x200C, 0x200D를 활용하면 이벤트 발생을 위한 임계값 설정이 가능함. 하지만 한쪽에 대한 임계치만 설정이 가능하기 때문에 문제가 될 수 있음. 예를 들어 온도의 경우 너무 높아져도 안되지만 너무 낮아져도 안됨 이런경우 온도의 적절한 범위(즉 상한과 하한 양쪽)에 대한 임계치가 필요함.

 - 운영시스템에서 제어기에게 환경정보를 요청시, 해당 요청이 상세하지 않음. 예를 들면 특정 센서를 지정할 수 없음. (표 6-9)

 - 위의 이슈와 관련하여 환경정보응답의경우 전송할 수 있는 응답이 최대 255개(NumberofBlock 이 1 바이트)라 센서의 개수를 생각하면 너무 작음. (표 6-9)

 - 위의 이유와 관련하여 표6-9에서 ContinuousInfo 의 값이 0x01인 경우 연속정도가 있는 것인데, 연속정보가 있다면 그 다음에 대한 요청을 보내는 것인지 아니면 새로운 응답 메세지가 이어서 바로 오는 것인지가 애매함. 그 다음에 대한 요청을 보낸다면 다음을 지정할 방법이 없기 때문에, 시간의 범위를 다시 세팅해야 하는데 경우에 따라 마지막 블록이 중복되거나 임의의 블록이 빠진 정보를 받을 수도 있음. 새로운 응답 메세지를 바로 보내는 경우에는 기본 전송타입에 맞지 않음. HistoryReqType 의 0x00을 활용하는 것은 기존의 어떤 요청에 대한 것인지 지정할 수 없기 때문에 불가능.

 - 표 6-10에서 장애관리시 프로그램 설치를 사용하는 경우 SoftwareBinary 가 가질수 있는 최대값은 64KB임. 필드 블럭의 길이로 2바이트가 할당되어 있기 때문임. 통합제어기를 64KB 이하로 만들어야 하는 제약이 생기며, 파일 명을 설정하거나 할 수 없기 때문에 설정파일을 업데이트하거나 하는 등의 작업도 쉽지 않음. 개별 구현에 따른다고 하면 굳이 표준에 넣을 이유도 없는 항목으로 보임.

 - 표 7-4, 표 7-6, 표 7-9, 표 7-12에는 TimeStamp가 포함되어 있는데, 전체 데이터 블럭에 하나의 TimeStamp 만이 포함됨. 개별 센서 혹은 개별 센서노드 단위로 센서값의 측정시간이 다를 수 있는데, 센서값은 그룹으로 묶으면서 하나의 TimeStamp 만을 허용하면 데이터의 왜곡이 생길 수 있음.  대표 시각으로 사용한다고 하면 고정헤더에 포함된 전송시간과 큰 차이가 없을 수 있음.

 - 7.6과 7.11 의 메세지와 7.5와 7.12의 메세지는 서로 매우 유사하고 일종의 상보적 관계가 있기 때문에 서로 결합하는 것이 유용할 것으로 보임

 - 8장에서 상태정보 및 환경정보의 노티와 상태정보 및 환경정보의 이력 조회 부분의 설명이 운영상의 문제를 발생할 수 있음. 해당 부분의 설명에 따르면, 통합제어기가 운영시스템에 접속이 된 시점에서 상태정보와 환경정보 노티를 해야한다. 더불어 상태정보 미전송내역과 환경정보 미전송 내역 역시 전송해야 한다. 후자의 정보가 전자의 정보를 포함할 수 있기 때문에 이러한 형태의 전송은 문제의 소지가 있다.

## 오탈자 이슈

 - 그림 1-1 에서 표준의 Part 번호가 잘못 표기 되어 있음

 - 6.1.3 Check Sum 영역의 마지막 부분에 있는 16진수값은 0x3443이 아니라 0x3843이어야 함

 - 표 6-3 7번의Field Name에 오타가 있음. Transfer Time 이어야 함.

 - 표 6-4의 RAMSize에서 Field Data설명 중 지수표시가 잘못되어 있음. 

 - 표 6-6의 Field Code 0x3005의 경우 r 을 제거하고 ActDataType 이 되어야 할듯함. 8.8.8 절에도 동일한 내용이 있음

 - 표 6-6의 Field Code 0x3007의 Field Data 설명에서 0x02가 아니라 0x00 이어야 함. 혹은 0x3006의 설명을 수정해야함.

 - 6.5.8 MsgSeqNumber 의 설명중 “최초 온실통합제어기가 설정한” 의 부분은 “최초 요청 메세지를 전송한 쪽에서 설정한” 으로 하는 것이 맞을 것 같음. 온실운영시스템에서 먼저 전송하는 요청 메세지도 있기 때문임.

 - 표 7-33의 폰트가 다른 것과 다름.

 - 8.1 Connection Approval 메세지 절차 의 설명중 아래에서 셋째 줄의 “자신이 가지고 정보” 는 “자신이 가지고 있는 정보”로 수정되어야 함

 - 8.7 Configuration Profile 메세지 절차에서 설명부분의 폰트가 다름.

 - 8.9.1 에서 메세지에 포함될 필드에 대한 폰트가 다름

 - 표 7-6 의 타임스탬프의 필드코드값이 잘못되어 있음 (0x400c -> 0x4006)

# 기타이슈

 - 5.1.1 온실통합제어기 설명이 살짝 중복적임

 - 5.3.2 Information Management 기능의 첫번째 단락 마지막 문장은 다음과 같이 변경하는 것이 조금 더 명확할 것으로 생각됨. “아날로그 제어가 가능한 액추에이터의 경우 액추에이터 값은 해당 아날로그 값이 된다. 예를 들어, 천창/측장 제어용 엑추에이터가 아날로그제어가 가능하다면, 천창/측장의 위치값이 해당 액추에이터의 값이 된다.”

 - 표 6-3의 Field Length 의 합계인 38 바이트는 틀린 값은 아니지만 마치 전송된 메세지 프레임에서 앞의 38바이트가 고정헤더값이라는 느낌을 주기 때문에 불필요한 오해의 소지가 있는 것 같음 오히려 Field Name과 Field Code 의 각16바이트씩을 모두 표현해주고, 전체 70바이트라고 표현하는 것이 더 좋을 것으로 생각됨

 - 표 6-4에 있는 온실통합제어기 프로파일에 MCUType, OSType, OSVersion, RAMSize, PowerType, SWVersion 이 과연 실효성 있는 데이터인지 의구심이 듬

 - 표 6-4에 있는 온실통합제어기 프로파일에 SWVersion 의 경우 다른 방식의 버전 표기도 가능할텐데 굳이 2 바이트로 고정할 필요가 있는지 의구심이 듬

 - 날짜와시간을 표현하기 위해서 Pack(“YYYYMMDDHHMMSS”) 구조로 7바이트를 부여했는데, 공간을 절약하는 효과가 있을지도 모르지만 epoch 타임을 쓰는 것보다 큰 효과가 없으며 다른 시간을 표현하는 항목들에서 “HHMMSS”의 형태로 6바이트를 사용하는 것과 비교해봐도 특별히 공간을 절약해야할 만한 이슈가 없음. (표 6-5, 표 6-6) 또한 통일성 측면에서도 좋지 못함.

 - 표 6-6의 ActDataType 의 설명중 0x01: 아날로그는 그 의미가 모호함. 본 표준은 디지털화 된 데이터를 이용한 통신이므로 실제 제어기가 아날로그 타입이더라도 해당 제어기의 상태를 정수형이든 실수형이든 디지털화된 값으로 전송하게 됨. 그런데 그 데이터형에 대한 표현없이 아날로그라고만 정의하는 것은 해당항목의 의미를 약화시킴.

 - 8.3 Status Information 메세지 절차에서 마지막 부분의 메세지 전송시점에서 3번째 설명은 Status Information-Notify의 메세지에 대한 것이 아니기 때문에 빼야함.

 - 8.4 Environment Information 설명중 49페이지 마지막에 “온실운영시스템으로부터” 부분은 “ 온실통합제어기로부터”로 변경되어야 함.