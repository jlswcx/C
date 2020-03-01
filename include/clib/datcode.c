

#include <clib.h>
#pragma comment(lib, "clib.lib")

#include "datcode.h"


char* c_dati_post(const char* _urlsuf, unsigned int _urlsuf_length, char* _imagedata, unsigned int _imagelength, int _timeout)
{
	const char* urlsuf = _urlsuf;
	char* imagedata = _imagedata;
	unsigned int urlsuf_length = _urlsuf_length;
	unsigned int imagelength = _imagelength;

	char* urlpre = "http://dat.tianlunvip.com/index.php/home/accex/index/s/";
	int urlprelen = 0;
	int nport = 80;
	char* key = "e3bbd444fac224155a47774acaa8c691";
	int expiry = 480;
	int rkeylen = 5;
	int timeout = _timeout;

	char* enurl = NULL;
	int enurllen = 0;

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
		szurl = (char*)c_alloc(urlprelen + enurllen);
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


DATI_API int __stdcall
SendImage(const char* _username, 
		  const char* _password,
		  int _itemcode,
		  int _x, int _y, int _w, int _h,
		  int _hwnd,
		  int _timeout, 
		  const char* _worker,
		  char* _result)
{
	char* imagedata = NULL;			// 截图数据
	size_t imagesize = 0;
	long x, y;

	char tmpbuf[3] = {0};
	char* result = NULL;

	char buffer[512] = {0};			// 存储数据	
	int buflen = 0;					// 实际数据长度
	int ret = c_dati_endata(_username, _password, _itemcode, _timeout, _worker, "SM", buffer, &buflen);
	if(ret) return ret;

	imagedata = c_prntscr(_x, _y, _w, _h, _hwnd, &imagesize);
	if(imagedata == NULL || imagesize <= 0)
	{	
		return C_DATI_ERR_PRNT;
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
	imagedata = c_dati_readfile("temp.jpg", &imagesize);
	result = c_dati_post(buffer, strlen(buffer), imagedata, imagesize, _timeout);
	if(_result && result)
		memcpy(_result, result, 2000);

	c_free(imagedata);

	return 0;
}



DATI_API int __stdcall
SendFile(const char* _username, 
		 const char* _password,
		 int _itemcode,
		 const char* _filename,
		 int _timeout, 
		 const char* _worker,
		 char* _result)
{
	char* imagedata = NULL;			// 截图数据
	size_t imagesize = 0;
	char* result = NULL;

	char buffer[512] = {0};			// 存储数据	
	int buflen = 0;					// 实际数据长度
	int ret = c_dati_endata(_username, _password, _itemcode, _timeout, _worker, "SM", buffer, &buflen);
	if(ret) return ret;

	/*
	  读取图片数据编码
	*/
	imagedata = c_dati_readfile(_filename, &imagesize);
	if(imagedata == NULL || imagesize <= 0)
	{
		return C_DATI_ERR_FILE;
	}
	result = c_dati_post(buffer, buflen, imagedata, imagesize, _timeout);
	if(_result && result) 
	{
		memcpy(_result, result, 32);
	}
	c_free(imagedata);

	return 0;
}


int c_dati_endata(const char* _username, 
				  const char* _password,
				  int _itemcode,
				  int _timeout, 
				  const char* _worker,
				  const char* _dtkind,
				  char* _buffer,
				  int* _length)
{
	const char* username = _username;
	const char* password = _password;
	int itemcode = _itemcode;
	int timeout	= _timeout;
	const char* worker = _worker;
	const char* szsend = _dtkind;	// 功能 发送图片 还是 获取积分等操作

	char* szexplode = "F=T";		// 分隔符

	int ulen = 0, plen = 0, length = 0;

	unsigned int ntime = (unsigned int)time(NULL);
	
	if(username == NULL) 
	{
		return C_DATI_ERR_USER;
	}
	if(password == NULL) 
	{
		return C_DATI_ERR_PWD;
	}

	ulen = strlen(username);
	plen = strlen(password);
	if(ulen < 5 || ulen > 28)
	{
		return C_DATI_ERR_USER;
	}
	if(plen < 8 || plen > 28)
	{
		return C_DATI_ERR_PWD;
	}

	if(itemcode < 1000 || itemcode > 1200)
	{
		return C_DATI_ERR_ITEM;
	}
	if(worker == NULL)
	{
		worker = "";
	}

	// &s=AUTHCODE  "SM" "F=T" username "F=T" password "F=T" itemcode "F=T" timeout "F=T" worker
	length = sprintf(_buffer, "%s%s%010d%s%s%s%s%s%d%s%d%s%s", szsend, szexplode, ntime, szexplode, username, szexplode, password, \
					 szexplode, itemcode, szexplode, timeout, szexplode, worker);

	if(_length) 
	{
		*_length = length;
	}

	return 0;
}


char* c_dati_readfile(const char* _filename, long* _out_length)
{
	const char* filename = _filename;

	long filesize = 0;
	char* filestream = NULL;
	char* imagedata = NULL;
	long imagesize = 0;

	FILE* fp = NULL;

	fp = fopen(filename, "rb");
	if(fp)
	{
		filesize = c_file_size(fp);
		filestream = (char*)c_alloc(filesize);
		fread(filestream, filesize, 1, fp);
		fclose(fp);
	}

	if(filestream != NULL && filesize > 0)
	{
		imagedata = (char*)c_alloc(2 + C_BASE64_ENLEN(filesize));
		imagesize = c_base64_encode_ex(filestream, filesize, imagedata + 2, 0, C_BASE64_URLSAFE);
		c_free(filestream);

		memcpy(imagedata, "m=", 2);
	}

	if(_out_length)
	{
		*_out_length = imagesize;
	}

	return imagedata;
}



void c_dati_test()
{
	char result[2049] = {0};
	int k = SendImage("tianlunvip", "35861541", 1000,0,0,200,100,0,60,"worker", result);
	printf("%s\n",result);
}


void main() {

	c_dati_test();

	system("pause");

}