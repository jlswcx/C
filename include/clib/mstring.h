
#ifndef __tlv_c_string_h__
#define __tlv_c_string_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
  �����Ҫ ���ַ����������紫��ʱ��ת�彫+ת_�Ȳ���
*/
char* c_string_en_urlsafe(char* string);
char* c_string_de_urlsafe(char* string);

/*
  ɾ����β�հ׷� */ 
char* c_ltrim(char *_string);
char* c_rtrim(char *_string);
char* c_trim(char *_string); 


#ifdef __cplusplus
}
#endif

#endif	// __tlv_c_string_h__
