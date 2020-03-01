
#ifndef __tlv_c_dati_h__
#define __tlv_c_dati_h__

#pragma once


#ifdef DATI_EXPORTS
//#define DATI_API __declspec(dllexport)
#define DATI_API
#else
#define DATI_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define	C_DATI_ERR_USER	3001	//"�û�������"
#define	C_DATI_ERR_CARD	3002	//"���⿨����"
#define	C_DATI_ERR_PWD	3003	//"�������"
#define	C_DATI_ERR_ITEM	3004	//"��Ŀ���ʹ���"
#define	C_DATI_ERR_NOCD	3005	//"���ֲ���"
#define	C_DATI_ERR_ULCK	3006	//"�˺�����"
#define	C_DATI_ERR_IP	3007	//"IP����"




#define	C_DATI_ERR_TIME	3004	//"ϵͳʱ���쳣"
#define	C_DATI_ERR_PRNT	3005	//"��ͼʧ��"
#define	C_DATI_ERR_FILE	3006	//"��ȡ�ļ�ʧ��"
#define	C_DATI_ERR_NET	3006	//"���������쳣"



char* c_dati_post(const char* _urlsuf, unsigned int _urlsuf_length, char* _imagedata, unsigned int _imagelength, int _timeout);
char* c_dati_readfile(const char* _filename, long* _out_length);
int c_dati_endata(const char* _username, 
				  const char* _password,
				  int _itemcode,
				  int _timeout, 
				  const char* _worker,
				  const char* _dtkind,
				  char* _out_buffer,
				  int* _out_length);


DATI_API int __stdcall
SendImage(const char* _username, 
		  const char* _password,
		  int _itemcode,
		  int _x, int _y, int _w, int _h,
		  int _hwnd,
		  int _timeout, 
		  const char* _worker,
		  char* _out_result);

DATI_API int __stdcall
SendFile(const char* _username, 
		 const char* _password,
		 int _itemcode,
		 const char* _filename,
		 int _timeout, 
		 const char* _worker,
		 char* _out_result);


void c_dati_test();


#ifdef __cplusplus
}
#endif

#endif	// __tlv_c_dati_h__
