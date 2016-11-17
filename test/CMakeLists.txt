# Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.

ENABLE_TESTING ()

INCLUDE_DIRECTORIES (../include ../util)

SET (LIBSRCS ../src/ttap3_base.c  ../src/ttap3_gcg.c  ../src/ttap3_gos.c  ../src/ttap3_process.c  ../src/ttap3_util.c ../util/mem.c)

ADD_EXECUTABLE (test_base test_base.c ${LIBSRCS})

ADD_EXECUTABLE (test_util test_util.c ${LIBSRCS})

ADD_EXECUTABLE (test_gcgngos test_gcgngos.c ${LIBSRCS})

ADD_EXECUTABLE (test_process test_process.c ${LIBSRCS})

ADD_TEST (unittest1 test_base)
ADD_TEST (unittest2 test_util)
ADD_TEST (unittest3 test_gcgngos)
ADD_TEST (unittest4 test_process)

