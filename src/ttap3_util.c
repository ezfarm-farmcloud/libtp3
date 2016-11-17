/** 
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
   \file ttap3_util.c
   \brief TTAK.KO-06.0288 Part 3 구현에 사용되는 유틸리티 함수 및 데이터타입
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ttap3_define.h>
#include <ttap3_config.h>
#include <ttap3_base.h>
#include <ttap3_util.h>

#include <../util/mem.h>

#ifdef _MSC_VER
#define localtime_r	localtime_s
#endif

void
tp3_setgcgprof (ptp3_gcgprof_t pgcgprof, byte *serial, byte *model, byte *ipv4, byte *ipv6, int installsite, byte mcutype, byte ostype, int osver, byte ramsize, byte powertype, int swver) {
	strcpy ((char *)pgcgprof->serial, (const char *)serial);
	strcpy ((char *)pgcgprof->model, (const char *)model);
	memcpy (pgcgprof->ipv4, ipv4, 4);
	memcpy (pgcgprof->ipv6, ipv6, 6);
	pgcgprof->installsite = installsite;
	pgcgprof->mcutype = mcutype;
	pgcgprof->ostype = ostype;
	pgcgprof->osver = osver;
	pgcgprof->ramsize = ramsize;
	pgcgprof->powertype = powertype;
	pgcgprof->swver = swver;
}

tp3_stat_t
tp3_readfields_gcgprof (ptp3_msg_t pmsg, ptp3_gcgprof_t pgcgprof) {
	tp3_fdata_t data;
	int len;

	len = tp3_readfield (pmsg, TP3_FC_SERIALNUM, &data);
	strncpy ((char *)pgcgprof->serial, (const char *)data.pbyte, len);
	(pgcgprof->serial)[len] = '\0';
	len = tp3_readfield (pmsg, TP3_FC_MODELNUM, &data);
	strncpy ((char *)pgcgprof->model, (const char *)data.pbyte, len);
	(pgcgprof->model)[len] = '\0';
	len = tp3_readfield (pmsg, TP3_FC_IPV4ADDR, &data);
	memcpy (pgcgprof->ipv4, &(data.data), 4);
	len = tp3_readfield (pmsg, TP3_FC_IPV6ADDR, &data);
	memcpy (pgcgprof->ipv6, data.pbyte, 6);
	len = tp3_readfield (pmsg, TP3_FC_INSTSITE, &data);
	pgcgprof->installsite = data.data;
	len = tp3_readfield (pmsg, TP3_FC_MCUTYPE, &data);
	pgcgprof->mcutype = data.data;
	len = tp3_readfield (pmsg, TP3_FC_OSTYPE, &data);
	pgcgprof->ostype = data.data;
	len = tp3_readfield (pmsg, TP3_FC_OSVER, &data);
	pgcgprof->osver = data.data;
	len = tp3_readfield (pmsg, TP3_FC_RAMSIZE, &data);
	pgcgprof->ramsize = data.data;
	len = tp3_readfield (pmsg, TP3_FC_POWTYPE, &data);
	pgcgprof->powertype = data.data;
	len = tp3_readfield (pmsg, TP3_FC_SWVER, &data);
	pgcgprof->swver = data.data;

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_addfields_gcgprof (ptp3_msg_t pmsg, ptp3_gcgprof_t pgcgprof) {
	TP3_STAT_NOERROR (
			tp3_addfield_nbytes (pmsg, TP3_FC_SERIALNUM, 
				strlen ((const char *)pgcgprof->serial), pgcgprof->serial));
	TP3_STAT_NOERROR (
			tp3_addfield_nbytes (pmsg, TP3_FC_MODELNUM, 
				strlen ((const char *)pgcgprof->model), pgcgprof->model));
	TP3_STAT_NOERROR (
			tp3_addfield_nbytes (pmsg, TP3_FC_IPV4ADDR, 4, pgcgprof->ipv4));
	TP3_STAT_NOERROR (
			tp3_addfield_nbytes (pmsg, TP3_FC_IPV6ADDR, 6, pgcgprof->ipv6));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_INSTSITE, pgcgprof->installsite));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_MCUTYPE, pgcgprof->mcutype));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_OSTYPE, pgcgprof->ostype));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_OSVER, pgcgprof->osver));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_RAMSIZE, pgcgprof->ramsize));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_POWTYPE, pgcgprof->powertype));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_SWVER, pgcgprof->swver));

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_readfields_senattr (ptp3_msg_t pmsg, ptp3_senattr_t psenattr) {
	tp3_fdata_t data;
	int tmp;

	TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_SNID, &data), TP3_SC_NOREQFIELD_ERR);
	tmp = data.data;
	TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_SID, &data), TP3_SC_NOREQFIELD_ERR);
	psenattr->gsid = tp3_getgid (tmp, data.data);
	TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_SENTYPE, &data), TP3_SC_NOREQFIELD_ERR);
	psenattr->sentype = data.data;
	TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_SENDATATYPE, &data), TP3_SC_NOREQFIELD_ERR);
	psenattr->datatype = data.data;
	TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_SENGETTYPE, &data), TP3_SC_NOREQFIELD_ERR);
	psenattr->gathertype = data.data;
	if (psenattr->gathertype == TP3_SENGT_PERIOD) {
		TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_SENGETINTV, &data), TP3_SC_NOREQFIELD_ERR);
		psenattr->gatherinterval = tp3_getinterval (data.pbyte);
	}
	if (tp3_readfield (pmsg, TP3_FC_MAXVALUE, &data) > 0)
		psenattr->maxval = data.data;
	if (tp3_readfield (pmsg, TP3_FC_MINVALUE, &data) > 0)
		psenattr->minval = data.data;
	if (tp3_readfield (pmsg, TP3_FC_PRECISION, &data) > 0)
		psenattr->precision = data.data;
	if (tp3_readfield (pmsg, TP3_FC_ERRRANGE, &data) > 0)
		psenattr->errrange = data.data;

	if (psenattr->gathertype == TP3_SENGT_EVENT) {
		TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_THRESHOLDTYPE, &data), TP3_SC_NOREQFIELD_ERR);
		psenattr->thresholdtype = data.data;
		TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_THRESHOLD, &data), TP3_SC_NOREQFIELD_ERR);
		psenattr->threshold = data.data;
	}

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_addfields_senattr (ptp3_msg_t pmsg, ptp3_senattr_t psenattr) {
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_SNID, tp3_getnid(psenattr->gsid)));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_SID, tp3_getdevid(psenattr->gsid)));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_SENTYPE, psenattr->sentype));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_SENDATATYPE, psenattr->datatype));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_SENGETTYPE, psenattr->gathertype));
	if (psenattr->gathertype == TP3_SENGT_PERIOD) {
		TP3_STAT_NOERROR (
			tp3_addfield_interval (pmsg, TP3_FC_SENGETINTV, psenattr->gatherinterval));
	}
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_MAXVALUE, psenattr->maxval));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_MINVALUE, psenattr->minval));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_PRECISION, psenattr->precision));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_ERRRANGE, psenattr->errrange));
	if (psenattr->gathertype == TP3_SENGT_EVENT) {
		TP3_STAT_NOERROR (
			tp3_addfield_interval (pmsg, TP3_FC_THRESHOLDTYPE, psenattr->thresholdtype));
		TP3_STAT_NOERROR (
			tp3_addfield_interval (pmsg, TP3_FC_THRESHOLD, psenattr->threshold));
	}
	
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_readfields_actattr (ptp3_msg_t pmsg, ptp3_actattr_t pactattr) {
	tp3_fdata_t data;
	int tmp;

	TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_ANID, &data), TP3_SC_NOREQFIELD_ERR);
	tmp = data.data;
	TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_AID, &data), TP3_SC_NOREQFIELD_ERR);
	pactattr->gaid = tp3_getgid (tmp, data.data);
	TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_ACTTYPE, &data), TP3_SC_NOREQFIELD_ERR);
	pactattr->acttype = data.data;
	TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_ACTDATATYPE, &data), TP3_SC_NOREQFIELD_ERR);
	pactattr->datatype = data.data;
	TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_ACTGETTYPE, &data), TP3_SC_NOREQFIELD_ERR);
	pactattr->gathertype = data.data;
	if (pactattr->gathertype == TP3_ACTGT_PERIOD) {
		TP3_STAT_RETURN (0 > tp3_readfield (pmsg, TP3_FC_ACTGETINTV, &data), TP3_SC_NOREQFIELD_ERR);
		pactattr->gatherinterval = tp3_getinterval (data.pbyte);
	}
			
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_addfields_actattr (ptp3_msg_t pmsg, ptp3_actattr_t pactattr) {
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_ANID, tp3_getnid(pactattr->gaid)));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_AID, tp3_getdevid(pactattr->gaid)));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_ACTTYPE, pactattr->acttype));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_ACTDATATYPE, pactattr->datatype));
	TP3_STAT_NOERROR (
			tp3_addfield_num (pmsg, TP3_FC_ACTGETTYPE, pactattr->gathertype));
	if (pactattr->gathertype == TP3_ACTGT_PERIOD) {
		TP3_STAT_NOERROR (
			tp3_addfield_interval (pmsg, TP3_FC_SENGETINTV, pactattr->gatherinterval));
	}
			
	return TP3_SC_NOERROR;
}

int
tp3_getgid (int nid, int devid) {
	return (nid << 8) + devid;
}

int
tp3_getdevid (int gid) {
	return gid & 0xFF;
}

int
tp3_getnid (int gid) {
	return gid >> 8;
}


tp3_stat_t
tp3_initnodedata (ptp3_nodedata_t pnode, int numofdev, int *gids, tp3_dev_t devtype) {
	TP3_STAT_RETURN ((devtype != TP3_DEV_SNODE) && (devtype != TP3_DEV_ANODE), 
			TP3_SC_NOT_PROPER_DEVTYPE);

	memset ((void *)pnode, 0x00, sizeof (tp3_nodedata_t));

	pnode->numofdev = numofdev;
	pnode->devtype = devtype;

	if (numofdev > 0) {
		pnode->gids = (int *)MALLOC(sizeof(int) * numofdev);
		pnode->stats = (tp3_devstat_t *)MALLOC(sizeof(tp3_devstat_t) * numofdev);
		pnode->envs = (int *)MALLOC(sizeof(int) * numofdev);
		if (devtype == TP3_DEV_SENSOR) {
			pnode->attrs = (void *)MALLOC(sizeof(tp3_senattr_t) * numofdev);
		} else {
			pnode->attrs = (void *)MALLOC(sizeof(tp3_actattr_t) * numofdev);
		}

		if (pnode->gids == NULL || pnode->stats == NULL 
				|| pnode->envs == NULL || pnode->attrs == NULL) {
			FREE (pnode->gids);
			FREE (pnode->stats);
			FREE (pnode->envs);
			FREE (pnode->attrs);

			TP3_STAT_PRETURN ("tp3_initnodedata : memory allocation for ptp3_nodedata_t",
					TP3_SC_MEMORY_ALLOCATION_FAIL);
		}

		memcpy ((void *)(pnode->gids), (void *)gids, sizeof(int) * numofdev);
		memset ((void *)(pnode->stats), 0x00, sizeof(tp3_nodestat_t) * numofdev);
		memset ((void *)(pnode->envs), 0x00, sizeof(int) * numofdev);
		if (devtype == TP3_DEV_SENSOR) {
			memset ((void *)(pnode->attrs), 0x00, sizeof(tp3_senattr_t) * numofdev);
		} else {
			memset ((void *)(pnode->attrs), 0x00, sizeof(tp3_actattr_t) * numofdev);
		}
	}

	return TP3_SC_NOERROR;
}

int
tp3_getidxdev (ptp3_nodedata_t pnode, int gid) {
	int i;
	for (i = 0; i < pnode->numofdev; i++) {
		if ((pnode->gids)[i] == gid) {
			return i;
		}
	}
	return -1;
}

tp3_stat_t
tp3_setdevenv (ptp3_nodedata_t pnode, int gid, int env) {
	int idx = tp3_getidxdev(pnode, gid);
	TP3_STAT_RETURN (idx < 0, TP3_SC_NOT_FOUND_DEVID);
	(pnode->envs)[idx] = env;
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_setdevstat (ptp3_nodedata_t pnode, int gid, tp3_devstat_t stat) {
	int idx = tp3_getidxdev(pnode, gid);
	TP3_STAT_RETURN (idx < 0, TP3_SC_NOT_FOUND_DEVID);
	(pnode->stats)[idx] = stat;
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_setdevattr (ptp3_nodedata_t pnode, int gid, void *attr) {
	int idx = tp3_getidxdev(pnode, gid);
	TP3_STAT_RETURN (idx < 0, TP3_SC_NOT_FOUND_DEVID);
	if (pnode->devtype == TP3_DEV_SENSOR) {
		ptp3_senattr_t pattr = (ptp3_senattr_t)(pnode->attrs);
		memcpy ((void *)(pattr + idx), attr, sizeof(tp3_senattr_t));
	} else {
		ptp3_actattr_t pattr = (ptp3_actattr_t)(pnode->attrs);
		memcpy ((void *)(pattr + idx), attr, sizeof(tp3_actattr_t));
	}
	return TP3_SC_NOERROR;
}

void
tp3_releasenodedata (ptp3_nodedata_t pnode) {
	FREE (pnode->gids);
	FREE (pnode->envs);
	FREE (pnode->stats);
	FREE (pnode->attrs);
}

void
tp3_setsenattr (ptp3_senattr_t psenattr, int gsid, tp3_sentype_t sentype, tp3_sendatatype_t datatype, tp3_sengathertype_t gathertype, int gatherinterval, int maxval, int minval, int precision, int errrange, tp3_senthrsdtype_t thresholdtype, int threshold) {
	psenattr->gsid = gsid;
	psenattr->sentype = sentype;
	psenattr->datatype = datatype;
	psenattr->gathertype = gathertype;
	psenattr->gatherinterval = gatherinterval;
	psenattr->maxval = maxval;
	psenattr->minval = minval;
	psenattr->precision = precision;
	psenattr->errrange = errrange;
	psenattr->thresholdtype = thresholdtype;
	psenattr->threshold = threshold;
}

void
tp3_setactattr (ptp3_actattr_t pactattr, int gaid, tp3_acttype_t acttype, tp3_actdatatype_t datatype, tp3_actgathertype_t gathertype, int gatherinterval) {
	pactattr->gaid = gaid;
	pactattr->acttype = acttype;
	pactattr->datatype = datatype;
	pactattr->gathertype = gathertype;
	pactattr->gatherinterval = gatherinterval;
}

void
tp3_initnset (ptp3_nsetdata_t pnset) {
	memset ((void *)pnset, 0x00, sizeof (tp3_nsetdata_t));
}

int
tp3_getidxnode (ptp3_nsetdata_t pnset, int nodeid) {
	int i;
	for (i = 0; i < pnset->numofnode; i++) {
		if ((pnset->nids)[i] == nodeid) {
			return i;
		}
	}
	return -1;
}

tp3_stat_t
tp3_updatenset (ptp3_nsetdata_t pnset, int nodeid, ptp3_nodedata_t pnode) {
	int idx = tp3_getidxnode(pnset, nodeid);

	if (pnode == NULL) {
		TP3_STAT_RETURN (idx < 0, TP3_SC_NOT_FOUND_NODEID);

		return tp3_delnset (pnset, nodeid);
	} else {
		if (idx < 0) {
			return tp3_addnset (pnset, nodeid, pnode);
		} else {
			pnset->numofdev += (pnode->numofdev - (pnset->nodedata)[idx].numofdev);
			tp3_releasenodedata ((pnset->nodedata) + idx);
			memcpy ((void *)((pnset->nodedata) + idx), (void *)pnode, sizeof(tp3_nodedata_t));
			return TP3_SC_NOERROR;
		}
	}
}

tp3_stat_t
tp3_addnset (ptp3_nsetdata_t pnset, int nodeid, ptp3_nodedata_t pnode) {
	int idx = tp3_getidxnode(pnset, nodeid);
	void *ptr1, *ptr2, *ptr3;

	TP3_STAT_RETURN (idx >= 0, TP3_SC_EXIST_NODEID);

	(pnset->numofnode)++;

	ptr1 = REALLOC(pnset->nids, sizeof(int) * pnset->numofnode);
	ptr2 = REALLOC(pnset->stats, sizeof(tp3_nodestat_t) * pnset->numofnode);
	ptr3 = REALLOC(pnset->nodedata, sizeof(tp3_nodedata_t) * pnset->numofnode);

	if (ptr1 == NULL || ptr2 == NULL || ptr3 == NULL) {
		if (ptr1 == NULL) {
			FREE (pnset->nids);
		} else if (ptr2 == NULL) {
			FREE (pnset->stats);
		} else {
			FREE (pnset->nodedata);
		}

		FREE (ptr1);
		FREE (ptr2);
		FREE (ptr3);

		TP3_STAT_PRETURN ("tp3_initnset : memory reallocation for ptp3_nsetdata_t",
				TP3_SC_MEMORY_ALLOCATION_FAIL);
	}

	pnset->nids = (int *)ptr1;
	pnset->stats = (tp3_nodestat_t *)ptr2;
	pnset->nodedata = (tp3_nodedata_t *)ptr3;

	(pnset->nids)[pnset->numofnode - 1] = nodeid;
	(pnset->stats)[pnset->numofnode - 1] = TP3_NODEST_NORMAL;
	memcpy ((void *)((pnset->nodedata) + (pnset->numofnode - 1)), (void *)pnode, sizeof(tp3_nodedata_t));
	pnset->numofdev += pnode->numofdev;
	
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_delnset (ptp3_nsetdata_t pnset, int nodeid) {
	int idx = tp3_getidxnode(pnset, nodeid);

	TP3_STAT_RETURN (idx < 0, TP3_SC_NOT_FOUND_NODEID);

	(pnset->numofnode)--;
	pnset->numofdev -= (pnset->nodedata + idx)->numofdev;
	tp3_releasenodedata (pnset->nodedata + idx);

	(pnset->nids)[idx] = (pnset->nids)[pnset->numofnode];
	(pnset->stats)[idx] = (pnset->stats)[pnset->numofnode];
	(pnset->nodedata)[idx] = (pnset->nodedata)[pnset->numofnode];
	
	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_setnodestat (ptp3_nsetdata_t pnset, int nodeid, tp3_nodestat_t nstat) {
	int idx = tp3_getidxnode(pnset, nodeid);

	TP3_STAT_RETURN (idx < 0, TP3_SC_NOT_FOUND_NODEID);

	(pnset->stats)[idx] = nstat;

	return TP3_SC_NOERROR;
}

void
tp3_releasenset (ptp3_nsetdata_t pnset) {
	int i;
	for (i = 0; i < pnset->numofnode; i++) {
		tp3_releasenodedata (pnset->nodedata + i);
	}

	FREE (pnset->nids);
	FREE (pnset->stats);
	FREE (pnset->nodedata);
}

int
tp3_readfield_array (ptp3_msg_t pmsg, tp3_fieldcode_t fc, int *arr, int maxsize) {
	tp3_fdata_t data;
	//int i, size, len;
	int len;
	byte *ptr;

	len = tp3_readfield (pmsg, fc, &data);
	TP3_EXP_RETURN (len < 0, -1, TP3_SC_NOREQFIELD_ERR);

	/*
	switch (fc) {
		case TP3_FC_ARROFSNID:
		case TP3_FC_ARROFANID:
			//size = _TP3_NODEID_SIZE;
			//break;
		case TP3_FC_ARROFGSID:
		case TP3_FC_ARROFGAID:
			//size = _TP3_GID_SIZE;
			//break;
		case TP3_FC_ARROFSNSTAT:
		case TP3_FC_ARROFANSTAT:
		case TP3_FC_ARROFSENSSTAT:
		case TP3_FC_ARROFACTSSTAT:
			//size = _TP3_STAT_SIZE;
			//break;
		case TP3_FC_ARROFSENSVAL:
			//size = _TP3_SENDATA_SIZE;
			//break;
		case TP3_FC_ARROFACTSVAL:
			//size = _TP3_ACTDATA_SIZE;
			break;
		default:
			TP3_EXP_RETURN (1, -1, TP3_SC_NOREQFIELD_ERR);
	}
	*/
	if (len > _TP3_FDATA_LIMIT)
		ptr = data.pbyte;
	else
		ptr = (byte *)&(data.data);

	if (len > maxsize * sizeof(int))
		len = maxsize * sizeof(int);
	memcpy (arr, ptr, len);
	return len / sizeof(int);
	/*
	len = len / size;
	for (i = 0; i < len && i < maxsize; i++) {
		arr[i] = tp3_readnum (ptr + i * size, size);
	}
	return i;
	*/
}

tp3_stat_t
tp3_nsettoarray (ptp3_nsetdata_t pnset, tp3_fieldcode_t fc, int *length, byte **pbyte) {
	int len, i, j;
	ptp3_nodedata_t pdev;
	int *ptr; 

	switch (fc) {
		case TP3_FC_ARROFSNID:
		case TP3_FC_ARROFANID:
			len = pnset->numofnode;
			ptr = (int *) MALLOC (len * sizeof(int));
			TP3_STAT_RETURN ((ptr == NULL), TP3_SC_MEMORY_ALLOCATION_FAIL);
			for (i = 0; i < len; i ++) {
				ptr[i] = pnset->nids[i];
			}
			break;

		case TP3_FC_ARROFGSID:
		case TP3_FC_ARROFGAID:
			len = pnset->numofdev;
			ptr = (int *) MALLOC (len * sizeof(int));
			TP3_STAT_RETURN ((ptr == NULL), TP3_SC_MEMORY_ALLOCATION_FAIL);
			pdev = pnset->nodedata;
			j = 0;
			if ( pdev->gids != NULL ) {
				for (i = 0; i < len; i++) {
					ptr[i] = pdev->gids[j++];
					if (j == pdev->numofdev) {
						pdev++;
						j = 0;
					}
				}
			}
			break;

		case TP3_FC_ARROFSNSTAT:
		case TP3_FC_ARROFANSTAT:
			len = pnset->numofnode;
			ptr = (int *) MALLOC (len * sizeof(int));
			TP3_STAT_RETURN ((ptr == NULL), TP3_SC_MEMORY_ALLOCATION_FAIL);
			for (i = 0; i < len; i++) {
				ptr[i] = pnset->stats[i];
			}
			break;

		case TP3_FC_ARROFSENSSTAT:
		case TP3_FC_ARROFACTSSTAT:
			len = pnset->numofdev;
			ptr = (int *) MALLOC (len * sizeof(int));
			TP3_STAT_RETURN ((ptr == NULL), TP3_SC_MEMORY_ALLOCATION_FAIL);
			pdev = pnset->nodedata;
			j = 0;
			if ( pdev->stats != NULL ) {
				for (i = 0; i < len; i++) {
					ptr[i] = pdev->stats[j++];
					if (j == pdev->numofdev) {
						pdev++;
						j = 0;
					}
				}
			}
			break;

		case TP3_FC_ARROFSENSVAL:
		case TP3_FC_ARROFACTSVAL:
			len = pnset->numofdev;
			ptr = (int *) MALLOC (len * sizeof(int));
			TP3_STAT_RETURN ((ptr == NULL), TP3_SC_MEMORY_ALLOCATION_FAIL);
			pdev = pnset->nodedata;
			j = 0;
			if ( pdev->envs != NULL ){
				for (i = 0; i < len; i++) {
					ptr[i] = pdev->envs[j++];
					if (j == pdev->numofdev) {
						pdev++;
						j = 0;
					}
				}
			}
			break;

		default:
			return TP3_SC_MSGTYPE_ERR;
	}

	*length = len * sizeof(int);
	*pbyte = (byte *)ptr;

	return TP3_SC_NOERROR;
}

tp3_stat_t
tp3_addfield_nsetdata (ptp3_msg_t pmsg, tp3_fieldcode_t fc, ptp3_nsetdata_t pnset) {
	int length;
	byte *pbyte;

	TP3_STAT_NOERROR (tp3_nsettoarray (pnset, fc, &length, &pbyte));
	TP3_STAT_NOERROR (tp3_addfield_nbytes (pmsg, fc, length, pbyte));
	// by chj 2015.9.30 
	// memory leak
	FREE(pbyte) ;
	return TP3_SC_NOERROR;
}

void
tp3_settimestamp (byte *ptr, time_t epoch) {
	struct tm t;
	byte tmp;

	localtime_r (&epoch, &t);
	tmp = (t.tm_year + 1900) / 100;
	ptr[0] = ((tmp / 10) << 4) | tmp % 10;
	tmp = (t.tm_year + 1900) % 100;
	ptr[1] = ((tmp / 10) << 4) | tmp % 10;
	tmp = (t.tm_mon + 1);
	ptr[2] = ((tmp / 10) << 4) | tmp % 10;
	tmp = t.tm_mday;
	ptr[3] = ((tmp / 10) << 4) | tmp % 10;
	tmp = t.tm_hour;
	ptr[4] = ((tmp / 10) << 4) | tmp % 10;
	tmp = t.tm_min;
	ptr[5] = ((tmp / 10) << 4) | tmp % 10;
	tmp = t.tm_sec;
	ptr[6] = ((tmp / 10) << 4) | tmp % 10;
}

time_t
tp3_getepoch (byte *ptr) {
	struct tm t;

	t.tm_year = ((ptr[0] / 16) * 10 + (ptr[0] & 0xF)) * 100;
	t.tm_year += (ptr[1] / 16) * 10 + (ptr[1] & 0xF) - 1900;
	t.tm_mon = (ptr[2] / 16) * 10 + (ptr[2] & 0xF) - 1;
	t.tm_mday = (ptr[3] / 16) * 10 + (ptr[3] & 0xF);
	t.tm_hour = (ptr[4] / 16) * 10 + (ptr[4] & 0xF);
	t.tm_min = (ptr[5] / 16) * 10 + (ptr[5] & 0xF);
	t.tm_sec = (ptr[6] / 16) * 10 + (ptr[6] & 0xF);

	return mktime (&t);
}

tp3_stat_t
tp3_addfield_timestamp (ptp3_msg_t pmsg, tp3_fieldcode_t fc, time_t stamp) {
	byte ptr[7];
	tp3_settimestamp (ptr, stamp);
	return tp3_addfield_bytes (pmsg, fc, ptr);
}

int
tp3_getinterval (byte *ptr) {
	return (ptr[0] - '0') * 36000 + (ptr[1] - '0') * 3600
		+ (ptr[2] - '0') * 600 + (ptr[3] - '0') * 60
		+ (ptr[4] - '0') * 10 + (ptr[5] - '0');
}

tp3_stat_t
tp3_addfield_interval (ptp3_msg_t pmsg, tp3_fieldcode_t fc, int interval) {
	int tmp;
	byte ptr[6];

	tmp = interval / 3600;
	ptr[0] = tmp / 10 + '0'; 
	ptr[1] = tmp % 10 + '0'; 
	tmp = (interval - tmp * 3600) / 60;
	ptr[2] = tmp / 10 + '0';
	ptr[3] = tmp % 10 + '0';
	ptr[4] = (interval % 60) / 10 + '0';
	ptr[5] = (interval % 10) + '0';

	return tp3_addfield_bytes (pmsg, fc, ptr);
}

tp3_stat_t
tp3_initblock (ptp3_block_t pblock, int rows, int numofsn, int numofsens, int numofan, int numofacts) {
	pblock->pitems = (tp3_blockitem_t *)MALLOC(sizeof(tp3_blockitem_t) * rows);
	TP3_STAT_RETURN (pblock->pitems == NULL, TP3_SC_MEMORY_ALLOCATION_FAIL);
	memset ((void *)(pblock->pitems), 0x00, sizeof(tp3_blockitem_t) * rows);

	pblock->rows = rows;
	pblock->numofdev[TP3_DEV_SNODE] = numofsn;
	pblock->numofdev[TP3_DEV_ANODE] = numofan;
	pblock->numofdev[TP3_DEV_SENSOR] = numofsens;
	pblock->numofdev[TP3_DEV_ACTUATOR] = numofacts;
	pblock->pcuritem = pblock->pitems;
	
	return TP3_SC_NOERROR;
}

void
tp3_nextemptyblockitem (ptp3_block_t pblock, time_t timestamp, tp3_gcgstat_t gcgstat) {
	while ((pblock->pcuritem)->timestamp != 0) {
		//ASSERT (pblock->pcuritem < pblock->pitems + pblock->rows);
		(pblock->pcuritem)++;
	}

	(pblock->pcuritem)->timestamp = timestamp;
	(pblock->pcuritem)->gcgstat = gcgstat;
	(pblock->itemidx)[TP3_DEV_SNODE] = (pblock->itemidx)[TP3_DEV_ANODE] 
		= (pblock->itemidx)[TP3_DEV_SENSOR] = (pblock->itemidx)[TP3_DEV_ACTUATOR] = 0;
}

void
tp3_setblockitem (ptp3_block_t pblock, int id, int value, tp3_dev_t devtype) {
	ptp3_blockitem_t pitem = pblock->pcuritem;
	int *idx = pblock->itemidx;

	pitem->ids[devtype][idx[devtype]] = id;
	pitem->vals[devtype][idx[devtype]] = value;
	idx[devtype] ++;
}

byte *
tp3_setstatblockbytes (byte *ptr, int numofdev, tp3_dev_t devtype, ptp3_blockitem_t pitem) {
	int i;

	*ptr = (byte)numofdev;
	ptr++;

	if (devtype == TP3_DEV_SNODE || devtype == TP3_DEV_ANODE) {
		for (i = 0; i < numofdev; i++) {
			tp3_set3bytes (ptr, (pitem->ids)[devtype][i]);
			ptr += _TP3_NODEID_SIZE;
		}
	} else {
		for (i = 0; i < numofdev; i++) {
			tp3_set4bytes (ptr, (pitem->ids)[devtype][i]);
			ptr += _TP3_GID_SIZE;
		}
	}

	for (i = 0; i < numofdev; i++) {
		*ptr = (pitem->vals)[devtype][i];
		ptr++;
	}

	return ptr;
}

tp3_stat_t
tp3_addfield_statblock (ptp3_msg_t pmsg, ptp3_block_t pblock) {
	int i;
	byte *pblk, *ptr;
	int step = 1 /* 1: gcgstat */
			+ 1 + 4 * pblock->numofdev[TP3_DEV_SNODE] /* 3 : node id, 1 : status */
		 	+ 1 + 4 * pblock->numofdev[TP3_DEV_ANODE] /* 3 : node id, 1 : status */
		 	+ 1 + 5 * pblock->numofdev[TP3_DEV_SENSOR] /* 4 : gid, 1 : status */
		 	+ 1 + 5 * pblock->numofdev[TP3_DEV_ACTUATOR] /* 4 : gid, 1 : status */
		 	+ 7; /* 7 : timestamp */
	int size = pblock->rows * step;

	pblk = (byte *)MALLOC (sizeof(byte) * size);
	TP3_STAT_RETURN (pblk == NULL, TP3_SC_MEMORY_ALLOCATION_FAIL);

	for (i = 0; i < pblock->rows; i++) {
		ptr = pblk + step * i;
		*ptr = (byte)((pblock->pitems + i)->gcgstat);
		ptr++;
		ptr = tp3_setstatblockbytes (ptr, pblock->numofdev[TP3_DEV_SNODE], TP3_DEV_SNODE, pblock->pitems + i);
		ptr = tp3_setstatblockbytes (ptr, pblock->numofdev[TP3_DEV_ANODE], TP3_DEV_ANODE, pblock->pitems + i);
		ptr = tp3_setstatblockbytes (ptr, pblock->numofdev[TP3_DEV_SENSOR], TP3_DEV_SENSOR, pblock->pitems + i);
		ptr = tp3_setstatblockbytes (ptr, pblock->numofdev[TP3_DEV_ACTUATOR], TP3_DEV_ACTUATOR, pblock->pitems + i);
		tp3_settimestamp (ptr, ((pblock->pitems + i)->timestamp));
	}

	TP3_STAT_NOERROR (tp3_addfield_nbytes (pmsg, TP3_FC_BLKOFSTAT, size, pblk));
	return TP3_SC_NOERROR;
}

byte *
tp3_getstatblockbytes (byte *ptr, int numofdev, tp3_dev_t devtype, ptp3_blockitem_t pitem) {
	int j;

	if (devtype == TP3_DEV_SNODE || devtype == TP3_DEV_ANODE) {
		for (j = 0; j < numofdev; j++) {
			(pitem->ids)[devtype][j] = tp3_readnum (ptr, _TP3_NODEID_SIZE);
			ptr += _TP3_NODEID_SIZE;
		}
	} else {
		for (j = 0; j < numofdev; j++) {
			(pitem->ids)[devtype][j] = tp3_readnum (ptr, _TP3_GID_SIZE);
			ptr += _TP3_GID_SIZE;
		}
	}
	for (j = 0; j < numofdev; j++) {
		(pitem->vals)[devtype][j] = *ptr;
		ptr++;
	}

	return ptr;
}

tp3_stat_t
tp3_parsestatblock (ptp3_block_t pblock, int rows, byte *ptr, int len) {
	int numofsn, numofsens, numofan, numofacts;
	int i, step;
	byte *pblk;
	ptp3_blockitem_t pitem;

	numofsn = ptr[1];
	numofan = ptr[1 + 1 + 4 * numofsn];
	numofsens = ptr[1 + 1 + 4 * numofsn + 1 + 4 * numofan];
	numofacts = ptr[1 + 1 + 4 * numofsn + 1 + 4 * numofan + 1 + 5 * numofsens];

	tp3_initblock (pblock, rows, numofsn, numofsens, numofan, numofacts);
	step = 1 /* 1: gcgstat */
			+ 1 + 4 * pblock->numofdev[TP3_DEV_SNODE] /* 3 : node id, 1 : status */
		 	+ 1 + 4 * pblock->numofdev[TP3_DEV_ANODE] /* 3 : node id, 1 : status */
		 	+ 1 + 5 * pblock->numofdev[TP3_DEV_SENSOR] /* 4 : gid, 1 : status */
		 	+ 1 + 5 * pblock->numofdev[TP3_DEV_ACTUATOR] /* 4 : gid, 1 : status */
		 	+ 7; /* 7 : timestamp */

	for (i = 0; i < rows; i++) {
		pblk = ptr + i * step;
		pitem = pblock->pitems + i;

		pitem->gcgstat = *pblk;
		pblk += 2;

		pblk = tp3_getstatblockbytes (pblk, numofsn, TP3_DEV_SNODE, pitem);
		pblk++;
		pblk = tp3_getstatblockbytes (pblk, numofan, TP3_DEV_ANODE, pitem);
		pblk++;
		pblk = tp3_getstatblockbytes (pblk, numofsens, TP3_DEV_SENSOR, pitem);
		pblk++;
		pblk = tp3_getstatblockbytes (pblk, numofacts, TP3_DEV_ACTUATOR, pitem);
		pitem->timestamp = tp3_getepoch (pblk);
	}

	return TP3_SC_NOERROR;
}

byte *
tp3_setenvblockbytes (byte *ptr, int numofdev, tp3_dev_t devtype, ptp3_blockitem_t pitem) {
	int i;

	*ptr = (byte)numofdev;
	ptr++;

	for (i = 0; i < numofdev; i++) {
		tp3_set4bytes (ptr, (pitem->ids)[devtype][i]);
		ptr += _TP3_GID_SIZE;
	}

	if (devtype == TP3_DEV_ACTUATOR) {
		for (i = 0; i < numofdev; i++) {
			tp3_set2bytes (ptr, (pitem->vals)[devtype][i]);
			ptr += _TP3_ACTDATA_SIZE;
		}
	} else {
		for (i = 0; i < numofdev; i++) {
			tp3_set4bytes (ptr, (pitem->vals)[devtype][i]);
			ptr += _TP3_SENDATA_SIZE;
		}
	}

	return ptr;
}

byte *
tp3_getenvblockbytes (byte *ptr, int numofdev, tp3_dev_t devtype, ptp3_blockitem_t pitem) {
	int i;

	for (i = 0; i < numofdev; i++) {
		(pitem->ids)[devtype][i] = tp3_readnum (ptr, _TP3_GID_SIZE);
		ptr += _TP3_GID_SIZE;
	}

	if (devtype == TP3_DEV_ACTUATOR) {
		for (i = 0; i < numofdev; i++) {
			(pitem->vals)[devtype][i] = tp3_readnum (ptr, _TP3_ACTDATA_SIZE);
			ptr += _TP3_ACTDATA_SIZE;
		}
	} else {
		for (i = 0; i < numofdev; i++) {
			(pitem->vals)[devtype][i] = tp3_readnum (ptr, _TP3_SENDATA_SIZE);
			ptr += _TP3_SENDATA_SIZE;
		}
	}
	return ptr;
}

tp3_stat_t
tp3_parseenvblock (ptp3_block_t pblock, int rows, byte *ptr, int len) {
	int numofsens, numofacts;
	int i, step;
	byte *pblk;
	ptp3_blockitem_t pitem;

	numofsens = ptr[0];
	numofacts = ptr[1 + 8 * numofsens];

	tp3_initblock (pblock, rows, 0, numofsens, 0, numofacts);
	step = 1 + 8 * pblock->numofdev[TP3_DEV_SENSOR] /* 4 : gid, 4 : env */
			+ 1 + 6 * pblock->numofdev[TP3_DEV_ACTUATOR] /* 4 : gid, 2 : env */
		 	+ 7; /* 7 : timestamp */

	for (i = 0; i < rows; i++) {
		pblk = ptr + i * step;
		pitem = pblock->pitems + i;

		pblk++;
		pblk = tp3_getenvblockbytes (pblk, numofsens, TP3_DEV_SENSOR, pitem);
		pblk++;
		pblk = tp3_getenvblockbytes (pblk, numofacts, TP3_DEV_ACTUATOR, pitem);
		pitem->timestamp = tp3_getepoch (pblk);
	}

	return TP3_SC_NOERROR;
}


tp3_stat_t
tp3_addfield_envblock (ptp3_msg_t pmsg, ptp3_block_t pblock) {
	int i;
	byte *pblk, *ptr;
	int step = 1 + 8 * pblock->numofdev[TP3_DEV_SENSOR] /* 4 : gid, 4 : env */
		 	+ 1 + 6 * pblock->numofdev[TP3_DEV_ACTUATOR] /* 4 : gid, 2 : env */
		 	+ 7; /* 7 : timestamp */
	int size = pblock->rows * step;

	pblk = (byte *)MALLOC (sizeof(byte) * size);
	TP3_STAT_RETURN (pblk == NULL, TP3_SC_MEMORY_ALLOCATION_FAIL);

	for (i = 0; i < pblock->rows; i++) {
		ptr = pblk + step * i;
		ptr = tp3_setenvblockbytes (ptr, pblock->numofdev[TP3_DEV_SENSOR], TP3_DEV_SENSOR, pblock->pitems + i);
		ptr = tp3_setenvblockbytes (ptr, pblock->numofdev[TP3_DEV_ACTUATOR], TP3_DEV_ACTUATOR, pblock->pitems + i);
		tp3_settimestamp (ptr, ((pblock->pitems + i)->timestamp));
	}

	TP3_STAT_NOERROR (tp3_addfield_nbytes (pmsg, TP3_FC_BLKOFENV, size, pblk));
	return TP3_SC_NOERROR;
}

void
tp3_releaseblock (ptp3_block_t pblock) {
	FREE (pblock->pitems);
}

