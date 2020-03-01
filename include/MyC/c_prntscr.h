
#ifndef __tlv_c_prntscr_h__
#define __tlv_c_prntscr_h__

#pragma once


/*
  截图屏幕图片或指定窗口HWND图片
  返回字符串需要手动释放
*/
char* c_prntscr(int _in_xpos, int _in_ypos, int _in_width , int _in_height, int _in_hwnd, unsigned long* _in_out_imagesize);



#endif	// !__tlv_c_prntscr_h__
