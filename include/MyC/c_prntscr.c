
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

	//����
	
	hdc = GetDC((HWND)_hwnd);							//��ĻDC
	jdc = CreateCompatibleDC(hdc);						//����DC
	jhbmp = CreateCompatibleBitmap(hdc,_width,_height);	//����λͼ
	oldhbmp = (HBITMAP)SelectObject(jdc, jhbmp);
	BitBlt(jdc, 0, 0, _width, _height, hdc, _xpos, _ypos, SRCCOPY);

	//
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);		//�ṹ�峤��
	bmpinfo.bmiHeader.biWidth = _width;							//ָ��λͼ�Ŀ��
	bmpinfo.bmiHeader.biHeight = -_height;						//ָ��λͼ�ĸ߶�
	bmpinfo.bmiHeader.biBitCount = 24;							//ÿһ���ص�λ��
	bmpinfo.bmiHeader.biSizeImage = c_bitmap_widthbytes(_width*24) * _height;			//ָ��ͼ��Ĵ�С�����ֽ�Ϊ��λ
	bmpinfo.bmiHeader.biCompression = BI_RGB;					//ָ��ѹ������(BI_RGBΪ��ѹ��)
	bmpinfo.bmiHeader.biPlanes = 1;								//ָ��Ŀ���豸�ļ���(����Ϊ 1 ) 
	bmpinfo.bmiHeader.biClrImportant = 0;						//ָ����Ϊ��Ҫ����ɫ������������ 0 ��ʾ������ɫ����Ҫ	
	bmpinfo.bmiHeader.biClrUsed = 0;							//����ɫ����ʵ��ʹ�õ�ɫ�������ĸ�������O��ʾȫҪʹ��
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;						//ָ���豸ˮƽ�ֱ��ʣ���ÿ�׵�����Ϊ��λ
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;						//ָ���豸��ֱ�ֱ��ʣ���ÿ�׵�����Ϊ��λ
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
