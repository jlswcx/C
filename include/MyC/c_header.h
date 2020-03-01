/**********************
   C Standard Libray
**********************/

#ifndef __tlv_c_standard_header_h__
#define __tlv_c_standard_header_h__

#pragma once


/*
不同编译器对C95以后的支持参差不齐，这里只包含C95以前的C库作统一使用
VC编译器更惨，变量只能定义在函数体或语句开头，数组无法使用动态定义。
*/

/* Bengin C Standard Libray */

/* #C89(15) */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* #C95(+3) */
#include <iso646.h>
#include <wchar.h>
#include <wctype.h>

/*
// #C99(+6) 
#include <complex.h>
#include <fenv.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <tgmath.h>	

// #C11(+5)
#include <stdalign.h>
#include <stdnoreturn.h>
#include <uchar.h>
#include <stdatomic.h>	
#include <threads.h>
*/

/* End C Standard Libray */


#include <sys/types.h>
#include <sys/stat.h>			/* fileexist use */


#endif	// __tlv_c_standard_header_h__
