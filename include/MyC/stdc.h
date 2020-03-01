/**********************
	All Header File
**********************/

#ifndef __tlv_main_h__
#define __tlv_main_h__

#pragma once


/* 
__GNUC__	GCC������
_MSC_VER	VC������
__LCC__		LCC������
*/


// ȷ��_DEBUG��DEBUG������
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


#ifdef _MSC_VER										// ʹ��VC������ʱ�Ż����ô���
#if _MSC_VER > 1200									// �߰汾MSC����������һЩ�Ͱ汾����ȫ����	
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS				// ʹ�� sprintf ���Ϻ���ʱ��ֹ��������
	#endif
	#ifndef _CRT_OBSOLETE_NO_WARNINGS
		#define _CRT_OBSOLETE_NO_WARNINGS			// ʹ�� _sleep ��ʱ����ʱ��ֹ��������
	#endif
	#ifndef _CRT_NON_CONFORMING_SWPRINTFS
		#define _CRT_NON_CONFORMING_SWPRINTFS		// �����°汾���� ��Ϊ���ϰ汾���������м�������
	#endif
#endif
#endif


/*
// �˺궨��֮ǰtime_tռ��8�ֽڣ�����֮��ռ��4�ֽ�
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
