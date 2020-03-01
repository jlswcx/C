/**********************
	All Header File
**********************/

#ifndef __tlv_main_h__
#define __tlv_main_h__

#pragma once


/* 
__GNUC__	GCC编译器
_MSC_VER	VC编译器
__LCC__		LCC编译器
*/


// 确保_DEBUG和DEBUG都能用
#ifdef _DEBUG
	#ifndef DEBUG
		#define DEBUG _DEBUG
	#endif
#endif
#ifdef DEBUG
	#ifndef _DEBUG
		#define _DEBUG DEBUG
	#endif
#endif


#ifdef _MSC_VER										// 使用VC编译器时才会设置此项
#if _MSC_VER > 1200									// 高版本MSC编译器禁用一些低版本不安全函数	
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS				// 使用 sprintf 等老函数时防止警告提醒
	#endif
	#ifndef _CRT_OBSOLETE_NO_WARNINGS
		#define _CRT_OBSOLETE_NO_WARNINGS			// 使用 _sleep 过时函数时防止警告提醒
	#endif
	#ifndef _CRT_NON_CONFORMING_SWPRINTFS
		#define _CRT_NON_CONFORMING_SWPRINTFS		// 禁用新版本函数 因为与老版本函数参数有兼容问题
	#endif
#endif
#endif


/*
// 此宏定义之前time_t占用8字节，定义之后占用4字节
#ifndef _USE_32BIT_TIME_T
	#define _USE_32BIT_TIME_T
#endif
*/

#include "c_header.h"
#include "c_define.h"
#include "c_malloc.h"
#include "c_file.h"
#include "c_md5.h"

#endif	// !__tlv_main_h__
