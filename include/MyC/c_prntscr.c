
#include "c_prntscr.h"
#include "c_bitmap.h"
#include "c_malloc.h"

#include <windows.h>


char* c_prntscr(int _xpos, int _ypos, int _width , int _height, int _hwnd, unsigned long* _imagesize)
{
	HDC hdc, jdc;
	HBITMAP jhbmp, oldhbmp;
	BITMAPINFO bmpinfo = {0};

	char* imagedata = NULL;

	//截屏
	
	hdc = GetDC((HWND)_hwnd);							//屏幕DC
	jdc = CreateCompatibleDC(hdc);						//兼容DC
	jhbmp = CreateCompatibleBitmap(hdc,_width,_height);	//兼容位图
	oldhbmp = (HBITMAP)SelectObject(jdc, jhbmp);
	BitBlt(jdc, 0, 0, _width, _height, hdc, _xpos, _ypos, SRCCOPY);

	//
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);		//结构体长度
	bmpinfo.bmiHeader.biWidth = _width;							//指定位图的宽度
	bmpinfo.bmiHeader.biHeight = -_height;						//指定位图的高度
	bmpinfo.bmiHeader.biBitCount = 24;							//每一像素的位数
	bmpinfo.bmiHeader.biSizeImage = c_bitmap_widthbytes(_width*24) * _height;			//指定图象的大小，以字节为单位
	bmpinfo.bmiHeader.biCompression = BI_RGB;					//指定压缩类型(BI_RGB为不压缩)
	bmpinfo.bmiHeader.biPlanes = 1;								//指定目标设备的级数(必须为 1 ) 
	bmpinfo.bmiHeader.biClrImportant = 0;						//指定认为重要的颜色索引个数，用 0 表示所有颜色均重要	
	bmpinfo.bmiHeader.biClrUsed = 0;							//在颜色表中实际使用的色彩索引的个数，用O表示全要使用
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;						//指定设备水平分辨率，以每米的像素为单位
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;						//指定设备垂直分辨率，以每米的像素为单位
//	jbmpo.bmiColors[0].rgbBlue=0;
//	jbmpo.bmiColors[0].rgbGreen=0;
//	jbmpo.bmiColors[0].rgbRed=0;
//	jbmpo.bmiColors[0].rgbReserved=0;
	
	imagedata = c_alloc(bmpinfo.bmiHeader.biSizeImage);

	if(!GetDIBits(jdc, jhbmp, 0, _height, imagedata, &bmpinfo, DIB_RGB_COLORS))		//DIB_PAL_COLORS/DIB_RGB_COLORS
	{
		imagedata = NULL;
		bmpinfo.bmiHeader.biSizeImage = 0;
	}

	SelectObject(jdc,oldhbmp);
	DeleteObject(jhbmp);
	DeleteDC(jdc);
	ReleaseDC((HWND)_hwnd, hdc);

	if(_imagesize)
		*_imagesize = bmpinfo.bmiHeader.biSizeImage;

	return imagedata;
}
