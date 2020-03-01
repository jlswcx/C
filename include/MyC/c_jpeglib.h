
#ifndef __tlv_c_jpeg_h__
#define __tlv_c_jpeg_h__

#pragma once


void c_jpeg_bmp2jpg_from_file(const char* _from_filename, const char* _to_filename);
void c_jpeg_bmp2jpg_from_buffer(long _width, long _height, unsigned short _bitcount, char* _bmpimagedata, const char* _to_filename, int _quality);




#endif	// !__tlv_c_jpeg_h__
