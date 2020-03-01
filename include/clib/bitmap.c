
#include "config.h"

#include <string.h>

#include "bitmap.h"
#include "malloc.h"


void c_bitmap_get_fileheader(char* _filestream, c_bitmap_fileheader* _bmpfileheader)
{
	if(_filestream == NULL) return;
	if(*(unsigned short*)_filestream != 19778) return;				// 是否为位图标志 

	memcpy(_bmpfileheader, _filestream, sizeof(c_bitmap_fileheader));
}

void c_bitmap_get_infoheader(char* _filestream, c_bitmap_infoheader* _bmpinfoheader)
{
	if(_filestream == NULL) return;
	if(*(unsigned short*)_filestream != 19778) return;

	memcpy(_bmpinfoheader, _filestream + 14, sizeof(c_bitmap_infoheader));
}

c_bitmap_rgbquad* c_bitmap_get_rgbq(char* _filestream, int* _quadnum)
{
	/*
	  rgbq数据位于 文件信息头之后 数据信息之前
	*/
	int rgbqsize = 0;

	c_bitmap_fileheader bmpfilehead = {0};
	c_bitmap_get_fileheader(_filestream, &bmpfilehead);

	rgbqsize = bmpfilehead.bfoffbits - 54;
	if(rgbqsize < sizeof(c_bitmap_rgbquad))				// 无法填充一个结构体 返回
		return NULL;
	else
		if(_quadnum)
		{
			*_quadnum = rgbqsize / sizeof(c_bitmap_rgbquad);
		}
	return (c_bitmap_rgbquad *)(_filestream+54);
}

void c_bitmap_get_whsize(char* _filestream, long* _width, long* _height)
{
	c_bitmap_infoheader bmpinfoheader = {0};
	c_bitmap_get_infoheader(_filestream, &bmpinfoheader);

	if(_width)
	*_width = bmpinfoheader.biwidth;
	if(_height)
	*_height = bmpinfoheader.biheight;
}

unsigned long c_bitmap_get_imagesize(char* _filestream)
{
	c_bitmap_infoheader bmpinfoheader = {0};
	c_bitmap_get_infoheader(_filestream, &bmpinfoheader);

	return bmpinfoheader.bisizeimage;
}

void c_bitmap_create_fileheader(c_bitmap_fileheader* _bmpfileheader, unsigned long _sizeimage)
{
	_bmpfileheader->bftype = 19778;			// "BM"
	_bmpfileheader->bfsize = 54 + _sizeimage;
	_bmpfileheader->bfreserved1 = 0;
	_bmpfileheader->bfreserved2 = 0;
	_bmpfileheader->bfoffbits = 54;
}

void c_bitmap_create_infoheader(c_bitmap_infoheader* _bmpinfoheader, long _width, long _height, unsigned short _bitcount)
{
	_bmpinfoheader->bisize = sizeof(c_bitmap_infoheader);
	_bmpinfoheader->biwidth = _width;
	_bmpinfoheader->biheight = _height;
	_bmpinfoheader->biplanes = 1;
	_bmpinfoheader->bibitcount = _bitcount;
	_bmpinfoheader->bicompression = 0;
	_bmpinfoheader->bisizeimage = c_bitmap_widthbytes(_width * _bitcount) * _height;
	_bmpinfoheader->bixpelspermeter = 0;
	_bmpinfoheader->biypelspermeter = 0;
	_bmpinfoheader->biclrused = 0;
	_bmpinfoheader->biclrimportant = 0;
}

char* c_bitmap_create(long _width, long _height, unsigned short _bitcount, void* _imagedata, unsigned long _imagesize)
{
	c_bitmap_fileheader _bmpfileheader;
	c_bitmap_infoheader _bmpinfoheader;
	char* filestream = NULL;

	c_bitmap_create_fileheader(&_bmpfileheader, _imagesize);
	c_bitmap_create_infoheader(&_bmpinfoheader, _width, _height, _bitcount);

	filestream = c_alloc(54 + _imagesize);
	memcpy(filestream, (void*)&_bmpfileheader, 14);
	memcpy(filestream+14, (void*)&_bmpinfoheader, 40);
	memcpy(filestream+54, _imagedata, _imagesize);

	return filestream;
}



