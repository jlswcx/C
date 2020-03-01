
#ifndef __tlv_c_bitmap_h__
#define __tlv_c_bitmap_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Windows 3.0�Ժ��BMP�ļ�����ָ�豸�޹�λͼ(DIB��device-independent bitmap)
// BMPλͼ�ļ�Ĭ�ϵ��ļ���չ����.BMP����ʱ��Ҳ����.DIB��.RLE����չ��
// RGB����Ҳ�ǵ�����ģ�ԭʼ�����ǰ�B��G��R��˳�����еġ�


#define c_bitmap_widthbytes(x)	((x + 31) / 32 * 4)		// x = width * biBitCount ���ض����Ŀ���ֽ���   x �ĵ�λ�� λ��
														// һ��Ҫ�� ���� ��Ȼ����������������ȼ�˳��

#pragma pack(push, 2)

// λͼ�ļ�ͷ	14���ֽ�			// ��Ϊ�漰���ṹ���ڴ���뷽ʽ ���Ըýṹ���СΪ 16 ��4�ı��죬 ���ö��뷽ʽ ʹ�� #pragma pack(2) ȡ���� #pragma pack() 

typedef struct {
	unsigned short  bftype;			// λͼ�ļ������ͣ�����ΪBM  *(word*)"BM"  2���ֽ�
	unsigned long   bfsize;			// λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ c_bitmapfileheader + c_bitmapinfoheader + imagesdata	4���ֽ�
	unsigned short  bfreserved1;	// λͼ�ļ������֣�����Ϊ0	2���ֽ�
	unsigned short  bfreserved2;	// λͼ�ļ������֣�����Ϊ0	2���ֽ�
	unsigned long   bfoffbits;		// λͼ���ݵ���ʼλ�ã��������λͼ�ļ�ͷ��ƫ������ʾ�����ֽ�Ϊ��λ	4���ֽ�
}c_bitmap_fileheader;

#pragma pack(pop)


// λͼ��Ϣͷ	40 ���ֽ�

typedef struct {
	unsigned long		bisize;				// ���ṹ��ռ���ֽ��� 40 ���ֽ�
	signed long			biwidth;			// λͼ�Ŀ�ȣ�������Ϊ��λ
	signed long			biheight;			// λͼ�ĸ߶ȣ�������Ϊ��λ
	unsigned short		biplanes;			// Ŀ���豸�ļ��𣬱���Ϊ1	
	unsigned short		bibitcount;			// ÿ�����������λ����������1(˫ɫ), 4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ
	unsigned long		bicompression;		// λͼѹ�����ͣ������� 0(BI_RGBΪ��ѹ��), 1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ
	unsigned long		bisizeimage;		// λͼ�Ĵ�С�����ֽ�Ϊ��λ
	signed long			bixpelspermeter;	// λͼˮƽ�ֱ��ʣ�ÿ��������
	signed long			biypelspermeter;	// λͼ��ֱ�ֱ��ʣ�ÿ��������
	unsigned long		biclrused;			// λͼʵ��ʹ�õ���ɫ���е���ɫ��
	unsigned long		biclrimportant;		// λͼ��ʾ��������Ҫ����ɫ��
}c_bitmap_infoheader;


// ��ɫ��	
typedef struct {
	unsigned char		rgbblue;			// ��ɫ������(ֵ��ΧΪ0-255)
	unsigned char		rgbgreen;			// ��ɫ������(ֵ��ΧΪ0-255)
	unsigned char		rgbred;				// ��ɫ������(ֵ��ΧΪ0-255)
	unsigned char		rgbreserved;		// ����������Ϊ0
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
  ����ֵ�����������鿪ͷָ�룬�õ���_filestream����������Ҫ�ֶ��ͷš�_quadnumΪ��������
*/
c_bitmap_rgbquad* c_get_bitmap_rgbq(char* _in_filestream, int* _in_out_quadnum);

/*
  ��ȡͼƬ��ɫ���ݳߴ�
*/
void c_bitmap_get_whsize(char* _in_filestream, long* _out_width, long* _out_height);
unsigned long c_bitmap_get_imagesize(char* _filestream);


void c_bitmap_create_fileheader(c_bitmap_fileheader* _in_out_bmpfileheader, unsigned long _in_sizeimage);
void c_bitmap_create_infoheader(c_bitmap_infoheader* _in_out_bmpinfoheader, long _in_width, long _in_height, unsigned short _in_bitcount);

/*
  �����ַ��ڴ���Ҫ�ֶ��ͷ�
*/
char* c_bitmap_create(long _in_width, long _in_height, unsigned short _in_bitcount, void* _in_imagedata, unsigned long _in_imagesize);

#ifdef __cplusplus
}
#endif

#endif	// !__tlv_c_bitmap_h__

