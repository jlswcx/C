
#ifndef __tlv_c_prntscr_h__
#define __tlv_c_prntscr_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
  ��ͼ��ĻͼƬ��ָ������HWNDͼƬ
  �����ַ�����Ҫ�ֶ��ͷ�
*/
char* c_prntscr(int _in_xpos, int _in_ypos, int _in_width , int _in_height, int _in_hwnd, unsigned long* _in_out_imagesize);


#ifdef __cplusplus
}
#endif

#endif	// !__tlv_c_prntscr_h__
