# Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.

ENABLE_TESTING ()

INCLUDE_DIRECTORIES (../include ../util)

FIND_LIBRARY (LIBS NAMES tp3 PATHS ../lib)
FIND_LIBRARY (UVLIB NAMES uv PATHS /usr/local/lib)

ADD_EXECUTABLE (test_base test_base.c)
TARGET_LINK_LIBRARIES (test_base ${LIBS})

ADD_EXECUTABLE (test_util test_util.c)
TARGET_LINK_LIBRARIES (test_util ${LIBS})

ADD_EXECUTABLE (test_gcgngos test_gcgngos.c)
TARGET_LINK_LIBRARIES (test_gcgngos ${LIBS})

ADD_EXECUTABLE (test_process test_process.c)
TARGET_LINK_LIBRARIES (test_process ${LIBS})

ADD_EXECUTABLE (test_tcp test_tcp.c)
TARGET_LINK_LIBRARIES (test_tcp ${LIBS})
TARGET_LINK_LIBRARIES (test_tcp ${UVLIB})

ADD_TEST (unittest1 test_base)
ADD_TEST (unittest2 test_util)
ADD_TEST (unittest3 test_gcgngos)
ADD_TEST (unittest4 test_process)
ADD_TEST (unittest5 test_tcp)
