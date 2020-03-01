
#ifndef __tlv_c_string_h__
#define __tlv_c_string_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
  如果需要 将字符串进行网络传送时先转义将+转_等操作
*/
char* c_string_en_urlsafe(char* string);
char* c_string_de_urlsafe(char* string);

/*
  删除首尾空白符 */ 
char* c_ltrim(char *_string);
char* c_rtrim(char *_string);
char* c_trim(char *_string); 


#ifdef __cplusplus
}
#endif

#endif	// __tlv_c_string_h__
