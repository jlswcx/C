
#ifndef __tlv_c_bitmap_h__
#define __tlv_c_bitmap_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Windows 3.0以后的BMP文件都是指设备无关位图(DIB，device-independent bitmap)
// BMP位图文件默认的文件扩展名是.BMP，有时它也会以.DIB或.RLE作扩展名
// RGB数据也是倒着念的，原始数据是按B、G、R的顺序排列的。


#define c_bitmap_widthbytes(x)	((x + 31) / 32 * 4)		// x = width * biBitCount 返回对齐后的宽度字节数   x 的单位是 位数
														// 一定要加 （） 不然可能于外面产生优先级顺序

#pragma pack(push, 2)

// 位图文件头	14个字节			// 因为涉及到结构体内存对齐方式 所以该结构体大小为 16 是4的背熟， 设置对齐方式 使用 #pragma pack(2) 取消用 #pragma pack() 

typedef struct {
	unsigned short  bftype;			// 位图文件的类型，必须为BM  *(word*)"BM"  2个字节
	unsigned long   bfsize;			// 位图文件的大小，以字节为单位 c_bitmapfileheader + c_bitmapinfoheader + imagesdata	4个字节
	unsigned short  bfreserved1;	// 位图文件保留字，必须为0	2个字节
	unsigned short  bfreserved2;	// 位图文件保留字，必须为0	2个字节
	unsigned long   bfoffbits;		// 位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位	4个字节
}c_bitmap_fileheader;

#pragma pack(pop)


// 位图信息头	40 个字节

typedef struct {
	unsigned long		bisize;				// 本结构所占用字节数 40 个字节
	signed long			biwidth;			// 位图的宽度，以像素为单位
	signed long			biheight;			// 位图的高度，以像素为单位
	unsigned short		biplanes;			// 目标设备的级别，必须为1	
	unsigned short		bibitcount;			// 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一
	unsigned long		bicompression;		// 位图压缩类型，必须是 0(BI_RGB为不压缩), 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
	unsigned long		bisizeimage;		// 位图的大小，以字节为单位
	signed long			bixpelspermeter;	// 位图水平分辨率，每米像素数
	signed long			biypelspermeter;	// 位图垂直分辨率，每米像素数
	unsigned long		biclrused;			// 位图实际使用的颜色表中的颜色数
	unsigned long		biclrimportant;		// 位图显示过程中重要的颜色数
}c_bitmap_infoheader;


// 颜色表	
typedef struct {
	unsigned char		rgbblue;			// 蓝色的亮度(值范围为0-255)
	unsigned char		rgbgreen;			// 绿色的亮度(值范围为0-255)
	unsigned char		rgbred;				// 红色的亮度(值范围为0-255)
	unsigned char		rgbreserved;		// 保留，必须为0
}c_bitmap_rgbquad;


typedef struct {
	c_bitmap_infoheader	bmiheader;
	c_bitmap_rgbquad	bmicolors[1];
}c_bitmap_info;

/*

*/
void c_bitmap_get_fileheader(char* _in_filestream, c_bitmap_fileheader* _out_bmpfileheader);
void c_bitmap_get_infoheader(char* _in_filestream, c_bitmap_infoheader* _out_bmpinfoheader);
/*
  返回值数据索引数组开头指针，用的是_filestream参数，不需要手动释放。_quadnum为数组数量
*/
c_bitmap_rgbquad* c_get_bitmap_rgbq(char* _in_filestream, int* _in_out_quadnum);

/*
  获取图片颜色数据尺寸
*/
void c_bitmap_get_whsize(char* _in_filestream, long* _out_width, long* _out_height);
unsigned long c_bitmap_get_imagesize(char* _filestream);


void c_bitmap_create_fileheader(c_bitmap_fileheader* _in_out_bmpfileheader, unsigned long _in_sizeimage);
void c_bitmap_create_infoheader(c_bitmap_infoheader* _in_out_bmpinfoheader, long _in_width, long _in_height, unsigned short _in_bitcount);

/*
  返回字符内存需要手动释放
*/
char* c_bitmap_create(long _in_width, long _in_height, unsigned short _in_bitcount, void* _in_imagedata, unsigned long _in_imagesize);

#ifdef __cplusplus
}
#endif

#endif	// !__tlv_c_bitmap_h__

