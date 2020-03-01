
#ifndef __tlv_c_libcurl_h__
#define __tlv_c_libcurl_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


char* c_curl_http_post(const char* _url, int _port, void* _data, size_t _datasize, int _timeout);
char* c_curl_http_get(const char* _url, int _port, void* _data, size_t _datasize, int _timeout);
void c_curl_http_download(char* url, char* data, int timeout);
void c_curl_http_upload(char* url, char* data, int timeout);


#ifdef C_CURL_TEST
	void c_curl_test();
#endif


#ifdef __cplusplus
}
#endif

#endif	// __tlv_c_libcurl_h__
