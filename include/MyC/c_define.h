/*
	自定义 宏变量 简化数据类型 函数 调试生成文件
*/
#ifndef __tlv_c_define_h__
#define __tlv_c_define_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#define _tlv_in_
#define _tlv_out_
#define _tlv_in_out_


typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned long	dword;

typedef char *pstr, *lpstr;;
typedef const char *pcstr, *lpcstr;
typedef void *pvoid, *lpvoid;

typedef long long llong;
typedef unsigned long long ullong;

#undef bool
#define bool int

#undef null
#define null ((void *)0)			// 标准C 定义方法


#define c_round(x, y)	(((x)+(y-1))&~(y-1))	
#define c_round4(x)		c_round(x, 4)			/* Multiple of Four up */
#define	c_lround4(x)	(c_round4(x) - 4)		/* Multiple of Four lower */

#ifndef c_makeword	
#define c_makeword(a, b)    ((word)(((byte)(a)) | ((word)((byte)(b))) << 8))
#endif	
#ifndef c_makelong
#define c_makelong(a, b)    ((long)(((word)(a)) | ((dword)((word)(b))) << 16))
#endif	

#define c_max_str_lenght	(0x3FFF - 1)	/* 预定义字符串 支持的最大字符数量 */



#ifndef _trace_on_
#define _trace_on_
#endif

void c_debugtrace(const char *szformat, ...);

#ifdef _trace_on_
	#define c_trace0(sz)						c_debugtrace("%s", sz)
	#define c_trace1(sz, p1)					c_debugtrace(sz, p1)
	#define c_trace2(sz, p1, p2)				c_debugtrace(sz, p1, p2)
	#define c_trace3(sz, p1, p2, p3)			c_debugtrace(sz, p1, p2, p3)
	#define c_trace4(sz, p1, p2, p3, p4)		c_debugtrace(sz, p1, p2, p3, p4)
	#define c_trace5(sz, p1, p2, p3, p4, p5)	c_debugtrace(sz, p1, p2, p3, p4, p5)
#else
	#define c_trace0(sz)
	#define c_trace1(sz, p1)
	#define c_trace2(sz, p1, p2)
	#define c_trace3(sz, p1, p2, p3)
	#define c_trace4(sz, p1, p2, p3, p4)	
	#define c_trace5(sz, p1, p2, p3, p4, p5)
#endif	// _TRACE_ON

#define c_tracelog	c_debugtrace

#define c_assert	assert

#ifdef __cplusplus
}
#endif


#endif	// __tlv_c_define_h__
