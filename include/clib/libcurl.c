
#include "config.h"

#include <stdlib.h>
#include <stdio.h>

#include <curl/curl.h>

#pragma comment(lib, "curl.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib ")
#pragma comment(lib, "advapi32.lib")

#include "malloc.h"
#include "libcurl.h"


unsigned int c_curl_write_callback(char *buffer, unsigned int size, unsigned int nmemb, void *userdata);


char* c_curl_http_post(const char* _url, int _port, void* _data, size_t _datasize, int _timeout)
{
	CURL* curl = NULL;
	CURLcode res = CURLE_OK;
	struct curl_slist *http_header = NULL;

	char* write_buffer = NULL;

	curl_global_init(CURL_GLOBAL_ALL);			/* In windows, this will init the winsock stuff */ 
	curl = curl_easy_init();					/* get a curl handle */ 
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, _url);									// First set the URL
		curl_easy_setopt(curl, CURLOPT_PORT, _port);								// port
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, _data);							// POST data
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, _datasize);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, c_curl_write_callback);		// 返回数据调用回调函数
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_buffer);					// 传递给回调函数的参数，可以是字符串保存读取的数据
 
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, _timeout);							// 超时时间秒数

//		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1000);			// 超时时间好毫秒数 
//		curl_easy_setopt(curl, CURLOPT_POST, 1);					// 设置为非0表示本次操作为POST
//		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);					// 设置为非0在执行时打印请求信息
//		curl_easy_setopt(curl, CURLOPT_HEADER, 1);					// 设置为非0将响应头信息同响应体一起传给WRITEFUNCTION
//		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);			// 设置为非0，响应头信息Location
//		curl_easy_setopt(curl, CURLOPT_COOKIE, "s=wreere");			// Cookie 部分内容 fruit=apple; colour=red
//		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "dookie.txt"); 

		res = curl_easy_perform(curl);								// Perform the request, res will get the return code
		if(res != CURLE_OK)											// Check for errors
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	return write_buffer;
}


char* c_curl_http_get(const char* _url, int _port, void* _data, size_t _datasize, int _timeout)
{
	return NULL;
}

void c_curl_http_download(char* url, char* data, int timeout)
{

}

void c_curl_http_upload(char* url, char* data, int timeout)
{
}



unsigned int c_curl_write_callback(char *buffer, unsigned int size, unsigned int nmemb, void *userdata)
{
	char* ptr = *(char**)userdata;
	if(nmemb > 0)
	{	
		c_block* pblock = c_blockdata(ptr);
		if(pblock)
		{
			int datalen = pblock->data_size;
			ptr = c_realloc(ptr, datalen + nmemb, 1);
			memcpy((unsigned char*)ptr + datalen, buffer, nmemb);
		}
		else
		{
			ptr = c_alloc(nmemb);
			memcpy(ptr, buffer, nmemb);
		}
		*(char**)userdata = ptr;
	}
	return size * nmemb;
}


#ifdef C_CURL_TEST

void c_curl_test()
{
	char* buffer;

	system("pause");
	buffer = c_curl_http_post("http://bbs.tianlunvip.com", 80, "",0,1000);
	printf("%s\n", buffer);
}

#endif