# Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (libtp3)

SET (LIBRARY_OUTPUT_PATH ../lib)

INCLUDE_DIRECTORIES (../include ../util)

SET (LIBSRCS ttap3_base.c  ttap3_gcg.c  ttap3_gos.c  ttap3_process.c  ttap3_util.c ../util/mem.c)

ADD_LIBRARY (tp3 SHARED ${LIBSRCS})

ADD_LIBRARY (tp3_static STATIC ${LIBSRCS})
