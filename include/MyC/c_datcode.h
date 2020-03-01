
#ifndef __tlv_c_dati_h__
#define __tlv_c_dati_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#define	C_DATI_ERR01	9001	//"�û�������"
#define	C_DATI_ERR02	9002	//"�������"
#define	C_DATI_ERR03	9003	//"ͼƬ���ʹ���"
#define	C_DATI_ERR04	9004	//"��ͼʧ��!"


char* c_dati_post(const char* _urlsuf, unsigned int _urlsuf_length, char* _imagedata, unsigned int _imagelength, int _timeout);
char* c_dati_enimage(const char* _in_filename, long* _in_out_retlength);

int 
c_sendimage(const char* _username, const char* _password,
	int _itemcode,
	int _x, int _y, int _w, int _h,
	int _hwnd,
	int _timeout, const char* _worker, char* _result);

int
c_sendimage2(const char* _datkey,
	int _itemcode,
	int _x, int _y, int _w, int _h,
	int _hwnd,
	int _timeout, const char* _worker, char* _result);

void c_dati_test();


#ifdef __cplusplus
}
#endif

#endif	// __tlv_c_dati_h__
