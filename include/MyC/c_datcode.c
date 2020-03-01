
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "c_datcode.h"
#include "c_base64.h"
#include "c_authcode.h"
#include "c_malloc.h"
#include "c_bitmap.h"
#include "c_prntscr.h"
#include "c_libcurl.h"
#include "c_jpeglib.h"
#include "c_file.h"


static const char* c_dati_null_string = "";


char* c_dati_post(const char* _urlsuf, unsigned int _urlsuf_length, char* _imagedata, unsigned int _imagelength, int _timeout)
{
	const char* urlsuf = _urlsuf;
	char* imagedata = _imagedata;
	unsigned int urlsuf_length = _urlsuf_length;
	unsigned int imagelength = _imagelength;

	char* urlpre = "http://www.tianlunvip.com/commun/receive.php?s=";
	int urlprelen = 0;
	int nport = 80;
	char* key = "e3bbd444fac224155a47774acaa8c691";
	int expiry = 480;
	int rkeylen = 5;
	int timeout = _timeout;

	char* enurl = NULL;
	unsigned int enurllen = 0;

	char* szurl = NULL;

	urlprelen = strlen(urlpre);

	if(urlsuf == NULL) {
		urlsuf = ""; urlsuf_length = 0;
	}
	if(imagedata == NULL) {
		imagedata = ""; imagelength = 0;
	}

	if(urlsuf != NULL && urlsuf_length > 0)
	{
		enurl = c_authcode(urlsuf, urlsuf_length ,"ENCODE", key, strlen(key), expiry, rkeylen, &enurllen);
		enurl = c_authcode_encode(enurl);
		szurl = c_alloc(urlprelen + enurllen);
		memcpy(szurl, urlpre, urlprelen);
		memcpy(szurl + urlprelen, enurl, enurllen);

		c_free(enurl);
	}
	else
	{
		szurl = urlpre;
	}

	return c_curl_http_post(szurl, nport, imagedata, imagelength, timeout);
}


// &s=AUTHCODE  "SM" "T=R" username "T=R" password "T=R" imagetype "T=R" worker "T=R" timeout

int c_sendimage(const char* _username, const char* _password, 
				  int _itemcode,
				  int _x, int _y, int _w, int _h, 
				  int _hwnd,
				  int _timeout, const char* _worker, char* _result)
{
	const char* username = _username;
	const char* password = _password;
	const char* worker = _worker;
	int itemcode = _itemcode;
	int timeout	= _timeout;

	char buffer[512] = {0};			// 存储数据	

	char* szfunc = "SM";			// 功能
	char* szexplode = "F=T";		// 分隔符
	int ulen = 0, plen = 0;

	char* imagedata = NULL;			// 截图数据
	unsigned long imagesize = 0;
	long x, y;

	char* enimagedata = NULL;		// 截图编码后数据
	long enimagesize = 0;

	char tmpbuf[3] = {0};
	char* result = NULL;

	ulen = strlen(username);
	plen = strlen(password);
	if(username == NULL || ulen <= 0 || ulen > 26)
	{
		return C_DATI_ERR01;
	}
	if(password == NULL || plen <= 0 || plen > 26)
	{
		return C_DATI_ERR02;
	}
	if(itemcode < 1000 || itemcode > 1200)
	{
		return C_DATI_ERR03;
	}
	if(worker == NULL)
	{
		worker = "";
	}
	if(timeout > 600)
	{
		timeout = 600;
	}

	sprintf(buffer, "%s%s%s%s%s%s%d%s%d%s%s", szfunc, szexplode, username, szexplode, password, \
		szexplode, itemcode, szexplode, timeout, szexplode, worker);

	imagedata = c_prntscr(_x, _y, _w, _h, _hwnd, &imagesize);
	if(imagedata == NULL || imagesize <= 0)
	{	
		return C_DATI_ERR04;
	}
	for(y = 0; y < _h; y++)
	{
		int y_widthbytes = c_bitmap_widthbytes(_w * 24) * y;
		for( x = 0;x < _w; x++)
		{	
			int demo = y_widthbytes + x*3;

			tmpbuf[0] = imagedata[demo];
		//	tmpbuf[1] = imagedata[demo+1];
			tmpbuf[2] = imagedata[demo+2];

			imagedata[demo] = tmpbuf[2];
			imagedata[demo+2] = tmpbuf[0];
		}
	}
	c_jpeg_bmp2jpg_from_buffer(_w, _h, 24, imagedata, "temp.jpg", 50);
	c_free(imagedata);

	/*
	  读取图片数据编码
	*/
	enimagedata = c_dati_enimage("temp.jpg", &enimagesize);
	result = c_dati_post(buffer, strlen(buffer), enimagedata, enimagesize, timeout);
	printf("%s\n", result);
	if(_result && result)
		memcpy(_result, result, 32);

	c_free(enimagedata);

	return 0;
}



int
c_sendimage2(const char* _datkey,
	int _itemcode,
	int _x, int _y, int _w, int _h,
	int _hwnd,
	int _timeout, const char* _worker, char* _result);





char* c_dati_enimage(const char* _filename, long* _retlength)
{
	const char* filename = _filename;
	FILE* fp = NULL;
	long filesize = 0;
	char* filestream = NULL;
	char* imagedata = NULL;
	long imagesize = 0;

	fp = fopen(filename, "rb");
	if(fp)
	{
		filesize = c_file_getsize(fp);
		filestream = c_alloc(filesize);
		fread(filestream, filesize, 1, fp);
		fclose(fp);
	}

	if(filestream != NULL && filesize > 0)
	{
		imagedata = c_alloc(2 + C_BASE64_ENLEN(filesize));
		imagesize = c_base64_encode(filestream, filesize, imagedata + 2);
		c_free(filestream);

		memcpy(imagedata, "m=", 2);
	}

	if(_retlength)
		*_retlength = imagesize;

	return imagedata;
}


void c_dati_test()
{
	char result[33] = {0};
	int k = c_sendimage("tianlunvip", "97691badcbc1e6bc7bb80e77ddcaf2d1", 1000,0,0,200,100,0,60,"worker", result);
//	printf("%s\n",result);
}