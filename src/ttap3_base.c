/** 
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
  \file ttap3_base.c
  \brief TTAK.KO-06.0288 Part 3 를 구현한 라이브러리 베이스

	TTA Part 3 라이브러리 베이스에서는 실제 전송되는 데이터와
	프로그램상에서 사용하는 메세지의 구조체를 상호변환 해주기 위한
	코드를 담고 있다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ttap3_define.h>
#include <ttap3_config.h>
#include <ttap3_base.h>
#include <../util/mem.h>

int
tp3_getframesize (ptp3_frame_t pframe) {
	return 4 + pframe->length;
}

tp3_stat_t
tp3_writeframe (ptp3_frame_t pframe, byte *buf, int len) {
	TP3_STAT_RETURN ((len < 4 + pframe->length), TP3_SC_FRAMEFMT_ERR);

	tp3_set4bytes (buf, (pframe->length));
	memcpy (buf + 4, pframe->data, pframe->length);
	return TP3_SC_NOERROR;
}


int
tp3_readframe (ptp3_frame_t pframe, byte *buf, int len) {
	if (len < 4) {
		return len - 4;
	}

	pframe->length = tp3_readnum (buf, 4);

	if (len < pframe->length + 4) {
		return len - (pframe->length + 4);
	}
	if (pframe->length < _TP3_HEADER_LENGTH) {
		return -1 * _TP3_HEADER_LENGTH;
	}

	pframe->data = (byte *) MALLOC ((size_t) (pframe->length));
	TP3_EXP_RETURN (pframe->data == NULL, 0, TP3_SC_MEMORY_ALLOCATION_FAIL);
	
	memcpy (pframe->data, buf + 4, pframe->length);

	return pframe->length + 4;
}

char
tp3_calculatechecksum (ptp3_frame_t pframe) {
	int length = pframe->length - _TP3_CHECKSUM_LENGTH - 1;
	byte *pbyte = (byte *)(pframe->data);
	byte checksum = *pbyte;

	do {
		pbyte++; 
		checksum ^= *pbyte;
	} while (pbyte - (byte *)(pframe->data) < length);

	//printf ("calculatedchecksum : %02x\n", checksum);
	return checksum;
}

int
tp3_setheadertoframedata (ptp3_header_t pheader, void *data) {
	tp3_fieldcode_t codes[_TP3_FIXED_HEADER_LEN] = {
		TP3_FC_INTERFACETYPE, TP3_FC_MESSAGETYPE, TP3_FC_TRANSTYPE, TP3_FC_GCGID, 
		TP3_FC_GOSID, TP3_FC_TRANSDATE, TP3_FC_TRANSTIME, TP3_FC_MSGSEQNUM};
	int lens[_TP3_FIXED_HEADER_LEN] = {10, 1, 1, 3, 3, 8, 6, 6};

	int i, length = 0;
	ptp3_framefield_t pfd;
	byte *pbyte;
	struct tm* tminfo;
	time_t t = time (NULL);

	tminfo = localtime(&t);

	for (i = 0; i < _TP3_FIXED_HEADER_LEN; i++) {
		pfd = (ptp3_framefield_t)((byte *)data + length);
		pfd->code = codes[i];
		pfd->length = lens[i];
		pbyte = &(pfd->data);
		switch (pfd->code) {
			case TP3_FC_INTERFACETYPE:
				strncpy ((char *)pbyte, (const char *)_TP3_INTERFACETYPE, lens[i]); 
				break;

			case TP3_FC_MESSAGETYPE:
				*pbyte = (byte)(pheader->msgtype);
				break;

			case TP3_FC_TRANSTYPE:
				*pbyte = (byte)(pheader->transtype);
				break;

			case TP3_FC_GCGID:
				tp3_set3bytes (pbyte, pheader->gcgid);
				break;

			case TP3_FC_GOSID:
				tp3_set3bytes (pbyte, pheader->gosid);
				break;

			case TP3_FC_TRANSDATE:
				strftime ((char *)pbyte, _TP3_HEADER_DATESTR_LEN + 1, (const char *)"%Y%m%d", tminfo);
				break;

			case TP3_FC_TRANSTIME:
				strftime ((char *)pbyte, _TP3_HEADER_TIMESTR_LEN + 1, (const char *)"%H%M%S", tminfo);
				break;

			case TP3_FC_MSGSEQNUM:
				sprintf ((char *)pbyte, "%06d", pheader->msgseq);
				break;
		}
		length += _TP3_FIELDINFO_LENGTH + lens[i];
	}

	return length;
}

void
tp3_copynbyte (byte *pdest, int *psrc, int base, int destlen) {
	int i;

	for (i = 0; i < destlen / base; i++) {
		switch (base) {
			case 1:
				*pdest = (byte)(*psrc);
				break;

			case 2: 
				tp3_set2bytes (pdest, *psrc);
				break;

			case 3: 
				tp3_set3bytes (pdest, *psrc);
				break;

			case 4:
				tp3_set4bytes (pdest, *psrc);
				break;
		}
		pdest += base;
		psrc++;
	}
}

int
tp3_getfieldlen (tp3_fieldcode_t code) {
	static int header[] = {10, 1, 1, 3, 3, 8, 6, 6};
	static int gprof[] = {1, -3, 1, -3, 0, 0, 4, 6, 2, 1, 1, 2, 1, 1, 2};
	static int sprof[] = {3, 2, -4, 1, 1, 1, 1, 6, 4, 4, 4, 4, 1, 4};
	static int aprof[] = {3, 2, -4, 1, 1, 1, 1, 6};
	static int stat[] = {1, -1, -1, -1, 0, -1, 7};
	static int env[] = {-4, -2};
	static int hist[] = {1, 7, 7, 1, 0, 0, 1};
	static int falut[] = {1, 0};

	int group = (code & 0xFF00) >> 8;
	int sub = code & 0xFF;

	switch (group) {
		case 0x00: return header[sub];
		case 0x10: return gprof[sub];
		case 0x20: return sprof[sub];
		case 0x30: return aprof[sub];
		case 0x40: return stat[sub];
		case 0x50: return env[sub];
		case 0x60: return hist[sub];
		case 0x70: return falut[sub];
	}

	return 0;
}

int
tp3_setfieldstoframedata (ptp3_msg_t pmsg, void *data) {
	int i, tmp, length = 0;
	ptp3_framefield_t pffd;
	ptp3_field_t pfd;
	ptp3_fdata_t pdata;
	byte *pbyte;

	for (i = 0; i < pmsg->numoffields; i++) {
		pffd = (ptp3_framefield_t) ((byte *)data + length);
		pfd = pmsg->fields + i;

		pffd->code = pfd->code;
		/*
		pffd->length = pfd->length;
		tmp = pfd->length;
		/*/
		tmp = tp3_getfieldlen (pfd->code);
		if (tmp >= 0) {
			pffd->length = pfd->length;
		} else {
			pffd->length = pfd->length * (tmp * -1) / sizeof(int);
		}
		//*/

		pdata = &(pfd->data);
		pbyte = &(pffd->data);

		switch (tmp) {
			case 1:
				*pbyte = (byte)(pdata->data);
				break;

			case 2: 
				tp3_set2bytes (pbyte, pdata->data);
				break;

			case 3: 
				tp3_set3bytes (pbyte, pdata->data);
				break;

			case 4:
				tp3_set4bytes (pbyte, pdata->data);
				break;

			default:
				if (tmp >= 0) {
					memcpy ((void *)pbyte, (void*)(pdata->pbyte), pffd->length);
				} else {
					if (pfd->length > _TP3_FDATA_LIMIT) {
						tp3_copynbyte (pbyte, (int *)(pdata->pbyte), tmp * -1, pffd->length);
					} else {
						tp3_copynbyte (pbyte, &(pdata->data), tmp * -1, pffd->length);
					}
				}
				break;
		}
		length += _TP3_FIELDINFO_LENGTH + pffd->length;
	}
	return length;
}

tp3_stat_t
tp3_generateframe (ptp3_msg_t pmsg, ptp3_frame_t pframe) {
	int i;
	char checksum;
	byte *pbyte;

	/// calculate length : 대략적인 값임. 실제값은 이값보다 작음.
	pframe->length = _TP3_HEADER_LENGTH + _TP3_CHECKSUM_LENGTH;
	for (i = 0; i < pmsg->numoffields; i++) {
		pframe->length += (pmsg->fields + i)->length + _TP3_FIELDINFO_LENGTH;
	}

	/// allocate data area
	pframe->data = (byte *) MALLOC ((size_t) (pframe->length));
	TP3_STAT_RETURN (pframe->data == NULL, TP3_SC_MEMORY_ALLOCATION_FAIL);

	/// set header & fields
	i = tp3_setheadertoframedata (&(pmsg->header), pframe->data);
	i += tp3_setfieldstoframedata (pmsg, pframe->data + i);
	
	pframe->length = i + _TP3_CHECKSUM_LENGTH;	 // 실제 길이임.

	/// set checksum
	checksum = tp3_calculatechecksum (pframe);
	pbyte = (byte *)(pframe->data);
	pbyte[pframe->length - 2] =  _TP3_HEXCHARS[(checksum & 0xF0)>>4];
	pbyte[pframe->length - 1] =  _TP3_HEXCHARS[(checksum & 0x0F)];

	return TP3_SC_NOERROR;
}

int
tp3_readnum (byte *pbyte, int len) {
	int num = 0, i = 0;
	while (i < len) {
		num = (num << 8) + pbyte[i];
		i++;
	}
	return num;
}

// 내부적으로만 사용
tp3_stat_t
tp3_addfield_array (ptp3_msg_t pmsg, tp3_fieldcode_t code, int basesize, int length, byte *ptr) {
	int i, n = length / basesize;
	int tmp[_TP3_MAX_ITEM];

	// debug
	if (n > _TP3_MAX_ITEM) {
		fprintf (stderr, "The number of item of a field is too big [%d]", n);
	}

	for (i = 0; i < n; i++) {
		tmp[i] = tp3_readnum (ptr, basesize);
		ptr += basesize;
	}
	return tp3_addfield_nbytes (pmsg, code, n * sizeof (int), (byte *)tmp);
}

tp3_stat_t
tp3_parseframe (ptp3_frame_t pframe, ptp3_msg_t pmsg) {
	int length = 0;
	byte *pbyte;
	ptp3_framefield_t pffd;
	int fldlen = 0;

	char checksum = tp3_calculatechecksum (pframe);

	/// check checksum
	pbyte = (byte *)(pframe->data);
	TP3_STAT_RETURN ((pbyte[pframe->length - 2] != _TP3_HEXCHARS[(checksum & 0xF0)>>4] 
		|| pbyte[pframe->length - 1] !=  _TP3_HEXCHARS[(checksum & 0x0F)]), 
		TP3_SC_CHECKSUM_NOT_MATCHED);

	/// parse frame
	while (length < pframe->length - 2) {
		pffd = (ptp3_framefield_t)((byte *)(pframe->data) + length);
		pbyte = &(pffd->data);

		if (pffd->code == TP3_FC_INTERFACETYPE) {
			TP3_STAT_RETURN (
				(strncmp ((const char *)_TP3_INTERFACETYPE, (const char *)pbyte,  
				  _TP3_INTERFACETYPE_LEN) != 0), TP3_SC_INTERFACETYPE_NOT_MATCHED);
		}

		fldlen = tp3_getfieldlen (pffd->code);
		if (fldlen > 0) {
			if ( fldlen == 1 ) {
			}
			if (pffd->length < (unsigned int)_TP3_FDATA_LIMIT) {
				// 숫자형 데이터
				// printf("") ; // 이부분 삭제하면 MSVC 컴파일러가 문제가 생김
				tp3_addfield_num (pmsg, pffd->code, tp3_readnum (pbyte, pffd->length));
			} else {	// Fixed length bytes data
				//ASSERT (fldlen == pffd->length);
				tp3_addfield_nbytes (pmsg, pffd->code, pffd->length, pbyte);
			}
		} else if (fldlen == 0) { // not fixed length bytes
			tp3_addfield_nbytes (pmsg, pffd->code, pffd->length, pbyte);
		} else {	// array type
			tp3_addfield_array (pmsg, pffd->code, fldlen * -1, pffd->length, pbyte);
		}

		/*
		if (pffd->length > _TP3_FDATA_LIMIT) {
			data.pbyte = pbyte;
		} else {
			if (0 < tp3_getfieldlen (pffd->code))
				data.data = tp3_readnum (pbyte, pffd->length);
			else
				memcpy (&(data.data), pbyte, pffd->length);
		}

		tp3_addfield (pmsg, pffd->code, pffd->length, &data);
		*/
		length += pffd->length + _TP3_FIELDINFO_LENGTH;
	}

	return TP3_SC_NOERROR;
}

void
tp3_releasemsg (ptp3_msg_t pmsg) {
	int i;
	for (i = 0; i < pmsg->numoffields; i++) {
		if ((pmsg->fields + i)->length > _TP3_FDATA_LIMIT)
			FREE (((pmsg->fields + i)->data).pbyte);
	}
	FREE (pmsg->fields);
}

void
tp3_releaseframe (ptp3_frame_t pframe) {
	FREE (pframe->data);
}

void
tp3_addheaderfield (ptp3_header_t pheader, tp3_fieldcode_t code, int length, ptp3_fdata_t pdata) {
	int i;

	switch (code) {
		case TP3_FC_MESSAGETYPE:
			pheader->msgtype = (tp3_msgtype_t) (pdata->data);
			break;

		case TP3_FC_TRANSTYPE:
			pheader->transtype = (tp3_transtype_t) (pdata->data);
			break;

		case TP3_FC_GCGID:
			//pheader->gcgid = data[0] << 16 + data[1] << 8 + data[2];
			pheader->gcgid = pdata->data;
			break;

		case TP3_FC_GOSID:
			//pheader->gosid = data[0] << 16 + data[1] << 8 + data[2];
			pheader->gosid = pdata->data;
			break;

		case TP3_FC_TRANSDATE:
			strncpy ((char *)pheader->tdate, (const char *)pdata->pbyte, _TP3_HEADER_DATESTR_LEN);
			break;

		case TP3_FC_TRANSTIME:
			strncpy ((char *)pheader->ttime, (const char *)pdata->pbyte, _TP3_HEADER_TIMESTR_LEN);
			break;

		case TP3_FC_MSGSEQNUM:
			pheader->msgseq = 0;
			for (i = 0; i < _TP3_HEADER_MSGSEQ_LEN; i++) {
				pheader->msgseq = pheader->msgseq * 10 + (pdata->pbyte)[i] - '0';
			}
			break;

		case TP3_FC_RESCODE:
			pheader->rescode = (tp3_rescode_t) (pdata->data);
			break;

		default:
			break;
	}
}

tp3_stat_t
tp3_extendfields (ptp3_msg_t pmsg) {
	if (pmsg->sizeoffields <= pmsg->numoffields) {
		ptp3_field_t ptmp;
		pmsg->sizeoffields += _TP3_FIELD_STEP;
		ptmp = (ptp3_field_t) MALLOC (sizeof (tp3_field_t) * pmsg->sizeoffields);

		if (ptmp == NULL) {
			pmsg->sizeoffields -= _TP3_FIELD_STEP;
			TP3_STAT_RETURN (1, TP3_SC_MEMORY_ALLOCATION_FAIL);
		}

		if (pmsg->fields != NULL) {
			memcpy (ptmp, pmsg->fields, sizeof (tp3_field_t) * pmsg->numoffields);
			FREE (pmsg->fields);
		}

		pmsg->fields = ptmp;
	}
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_addfield (ptp3_msg_t pmsg, tp3_fieldcode_t code, int length, ptp3_fdata_t data) {
	if (code < TP3_FC_RESCODE) {
		tp3_addheaderfield (&(pmsg->header), code, length, data);
	} else {
		ptp3_field_t pfd;

		if (code == TP3_FC_RESCODE) {
			tp3_addheaderfield (&(pmsg->header), code, length, data);
		}

		tp3_extendfields (pmsg);	// Field 공간 확보
		pfd = pmsg->fields + pmsg->numoffields;
		pfd->code = code;
		pfd->length = length;
		if (length > _TP3_FDATA_LIMIT) {
			(pfd->data).pbyte = MALLOC(length);
			TP3_STAT_RETURN (((pfd->data).pbyte == NULL), TP3_SC_MEMORY_ALLOCATION_FAIL);
			memcpy ((void *)((pfd->data).pbyte), (void *)(data->pbyte), length);
		} else {
			pfd->data = *data;
		}
		pmsg->numoffields ++;
	}

	return TP3_SC_NOERROR;
}


tp3_stat_t
tp3_addfield_num (ptp3_msg_t pmsg, tp3_fieldcode_t code, int data) {

	tp3_fdata_t fdata;
	int length = tp3_getfieldlen (code);

	fdata.data = data;

	return tp3_addfield (pmsg, code, length, &fdata);
}

tp3_stat_t
tp3_addfield_bytes (ptp3_msg_t pmsg, tp3_fieldcode_t code, byte *ptr) {
	tp3_fdata_t fdata;
	int length = tp3_getfieldlen (code);

	if (length > _TP3_FDATA_LIMIT)
		fdata.pbyte = ptr;
	else {
		memcpy (&(fdata.data), ptr, length);
	}

	return tp3_addfield (pmsg, code, length, &fdata);
}


tp3_stat_t
tp3_addfield_nbytes (ptp3_msg_t pmsg, tp3_fieldcode_t code, int length, byte *ptr) {
	tp3_fdata_t fdata;
	
	if (length > _TP3_FDATA_LIMIT)
		fdata.pbyte = ptr;
	else {
		memcpy (&(fdata.data), ptr, length);
	}

	return tp3_addfield (pmsg, code, length, &fdata);
}

tp3_stat_t
tp3_setheader (ptp3_msg_t pmsg, tp3_msgtype_t msgtype, tp3_transtype_t transtype, int gcgid, int gosid, int msgseq) {
	ptp3_header_t pheader = &(pmsg->header);

	strncpy ((char *)pheader->ifacetype, (const char *)_TP3_INTERFACETYPE, _TP3_HEADER_INTERFACE_LEN);
	pheader->msgtype = msgtype;
	pheader->transtype = transtype;
	pheader->gcgid = gcgid;
	pheader->gosid = gosid;
	pheader->msgseq = msgseq % _TP3_MSGSEQ_MAX;

	return TP3_SC_NOERROR;
}

void
tp3_initmsg (ptp3_msg_t pmsg) {
	memset ((void *)pmsg, 0x00, sizeof (tp3_msg_t));
}

void
tp3_initframe (ptp3_frame_t pframe) {
	memset ((void *)pframe, 0x00, sizeof (tp3_frame_t));
}

int
tp3_readfieldlen (ptp3_msg_t pmsg, tp3_fieldcode_t code) {
	int i;
	for (i = 0; i < pmsg->numoffields; i++) {
		if ((pmsg->fields)[i].code == code)
			return (pmsg->fields)[i].length;
	}
	return -1;
}

int
tp3_isnumberinfield (ptp3_msg_t pmsg, tp3_fieldcode_t code) {
	int i;
	for (i = 0; i < pmsg->numoffields; i++) {
		if ((pmsg->fields)[i].code == code) {
			if ((pmsg->fields)[i].length > _TP3_FDATA_LIMIT) {
				return 0;
			} else {
				return 1;
			}
		}
	}
	return -1;
}

int
tp3_readfield (ptp3_msg_t pmsg, tp3_fieldcode_t code, ptp3_fdata_t pdata) {
	int i;
	ptp3_field_t pfd;

	for (i = 0; i < pmsg->numoffields; i++) {
		pfd = pmsg->fields + i;
		if (pfd->code == code) {
			memcpy ((void *)pdata, (void *)(&(pfd->data)), sizeof (tp3_fdata_t));
			return pfd->length;
		}
	}
	TP3_EXP_RETURN (1, -1, TP3_SC_NOREQFIELD_ERR);
}

int
tp3_readfield_num (ptp3_msg_t pmsg, tp3_fieldcode_t code, int *pdata) {
	tp3_fdata_t fdata;
	int ret;

	ret = tp3_readfield (pmsg, code, &fdata);
	if (ret <= 0)
		return ret;
	if (ret > 4)
		return -2;
	*pdata = fdata.data;
	return ret;
}

void
tp3_printmsg (ptp3_msg_t pmsg) {
	int i, j;
	ptp3_field_t pfd;
	ptp3_header_t pheader = &(pmsg->header);

	// print header
	printf ("msgtype, transtype, gcgid, gosid : %x, %x, %x, %x \n", 
			pheader->msgtype, pheader->transtype, pheader->gcgid, pheader->gosid);
	printf ("tdate, ttime, msgseq : %s, %s, %d\n", 
			pheader->tdate, pheader->ttime, pheader->msgseq);

	// print fields
	for (i = 0; i < pmsg->numoffields; i++) {
		pfd = pmsg->fields + i;
		printf ("fieldcode : %x, fieldlength : %d, data :", pfd->code, pfd->length);
		switch (pfd->length) {
			case 1: case 2: case 3: case 4:
				printf (" %d", (pfd->data).data);
				break;
			default:
				for (j = 0; j < pfd->length; j++) {
					if (j % 4 == 0)
						printf (" ");
					printf ("%02x", ((pfd->data).pbyte)[j]);
				}
		}
		printf ("\n");
	}
	printf ("\n");
}

void
tp3_printframe (ptp3_frame_t pframe) {
	int length, j;
	byte *pbyte;
	ptp3_framefield_t pffd;

	length = 0;
	pbyte = (byte *)(pframe->data);

	printf ("frame length : %d, checksum :[%c%c]\n",
			pframe->length, pbyte[pframe->length - 2], pbyte[pframe->length - 1]);

	while (length < (int)pframe->length - 2) {
		pffd = (ptp3_framefield_t)((byte *)(pframe->data) + length);
		pbyte = &(pffd->data);
		printf ("fieldcode : %x, fieldlength : %d, data :", pffd->code, pffd->length);
		for (j = 0; j < pffd->length; j++, pbyte++) {
			if (j % 4 == 0)
				printf (" ");
			printf ("%02x", *pbyte);
		}
		printf ("\n");

		length += pffd->length + _TP3_FIELDINFO_LENGTH;
	}
	printf ("\n");
}

void
tp3_printerror (tp3_stat_t stat) {
	char *msg = tp3_geterrormsg (stat);

	if (msg == NULL) {
		printf ("%x는 %s\n", stat, "알수없는 에러입니다.");
	} else {
		printf ("%s\n", msg);
	}
}

char *
tp3_geterrormsg (tp3_stat_t stat) {
	static char *nostat = "존재하지 않는 에러입니다.";
	static char *msgs[] = {
		"에러가 없습니다.",
		"존재하지 않는 메세지 타입입니다.",
		"메세지 전송타입이 올바르지 않습니다.",
		"통합제어기 식별자가 맞지 않습니다.",
		"운영시스템 식별자가 맞지 않습니다.",
		"전송일자 및 시각에 오류가 있습니다.",
		"메세지 일련번호가 중복됩니다.",
		"필수 필드가 존재하지 않습니다.",
		"프레임 형식이 맞지 않습니다."
	};
	static char *exmsgs[] = {
		"내부 에러 입니다.",
		"메모리할당을 할 수 없습니다.",
		"인터페이스 문자열이 맞지 않습니다.",
		"체크섬이 맞지 않습니다.",
		"요청 메세지가 아닙니다.",
		"노드아이디가 이미 존재합니다.",
		"노드아이디를 찾을 수 없습니다.",
		"적절하지 않은 디바이스 타입입니다.",
		"디바이스 아이디를 찾을 수 없습니다.",
		"할당된 공간이 충분하지 않습니다."
	};

	if (stat <= TP3_SC_NOREQFIELD_ERR)
		return msgs[stat];
	if (stat == TP3_SC_FRAMEFMT_ERR)
		return msgs[8];
	if (stat >= TP3_SC_INTERNAL_ERR && stat < TP3_SC_MAXSIZE)
		return exmsgs[stat - TP3_SC_INTERNAL_ERR];
	return nostat;
}

tp3_stat_t
tp3_setrescode (ptp3_msg_t pres, tp3_rescode_t rescode) {
	tp3_fdata_t data;

	data.data = (int) rescode;
	return tp3_addfield (pres, TP3_FC_RESCODE, 1, &data);
}


tp3_stat_t
tp3_setresponse (ptp3_msg_t preq, ptp3_msg_t pres, tp3_rescode_t rescode) {
	ptp3_header_t pheader = &(preq->header);
	tp3_transtype_t transtype = pheader->transtype + 1;
	tp3_stat_t stat = TP3_SC_NOERROR;

	if (transtype > TP3_TT_OS_CG_ACKA || transtype < TP3_TT_CG_OS_REQ) {
		return TP3_RC_MSGTYPE_ERR;
	}

	switch (pheader->transtype) {
		case TP3_TT_CG_OS_RES:
		case TP3_TT_OS_CG_RES:
		case TP3_TT_CG_OS_NOTI:
		case TP3_TT_OS_CG_ACKA:
			return TP3_SC_IS_NOT_REQUEST;
		default:
			break;
	}

	tp3_initmsg (pres);

	stat = tp3_setheader (pres, pheader->msgtype, transtype, pheader->gcgid, pheader->gosid, pheader->msgseq);
	if (stat) {
		tp3_printerror (stat);
		return stat;
	}

	stat = tp3_setrescode (pres, rescode);
	if (stat) {
		tp3_printerror (stat);
		return stat;
	}

	return stat;
}

