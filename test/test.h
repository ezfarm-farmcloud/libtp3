/**
 * Copyright © 2015-2016 ezfarm Co., Ltd. All Rights Reserved.
   \file test.h
   \brief libtp3 를 테스트하기 위해 사용되는 헤더파일
 */
#ifndef _TP3_TEST_H_
#define _TP3_TEST_H_

/* Die with fatal error. */
#define FATAL(msg)                                        		\
	do {                                                    	\
		fprintf(stderr,                                       	\
			"Fatal error in %s on line %d: %s\n",         		\
			__FILE__,                                     		\
			__LINE__,                                     		\
			msg);                                         		\
		fflush(stderr);                                       	\
		abort();                                              	\
	} while (0)

/* No error Assertion Test */
#define ASSERT_NOERROR(expr)											\
	do {                                                     	\
		tp3_stat_t __stat;  \
		if (TP3_SC_NOERROR != (__stat = expr)) {                                          \
			fprintf(stderr,                                     \
				"Assertion failed in %s on line %d: %s\n\t%s\n",   	\
				__FILE__,                                     	\
				__LINE__,                                     	\
				#expr,                                       	\
				tp3_geterrormsg (__stat));                                       	\
			abort();                                            \
		}                                                       \
	} while (0)

/* Assertion Test */
#define ASSERT(expr)											\
	do {                                                     	\
		if (!(expr)) {                                          \
			fprintf(stderr,                                     \
				"Assertion failed in %s on line %d: %s\n",    	\
				__FILE__,                                     	\
				__LINE__,                                     	\
				#expr);                                       	\
			abort();                                            \
		}                                                       \
	} while (0)

/* Test Run */
#define TEST(expr)											\
	do {                                                     	\
		if (TP3_SC_NOERROR == (expr)) {                                          \
			fprintf(stderr,                                     \
				"%s test pass in %s on line %d\n",    	\
				#expr,                                       	\
				__FILE__,                                     	\
				__LINE__);                                     	\
		}                                                       \
	} while (0)

#endif //_TP3_TEST_H_
