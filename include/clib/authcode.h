
#ifndef __tlv_c_authcode_h__
#define __tlv_c_authcode_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
  返回字符串需要手动释放
*/

char* c_authcode(const char* _in_src, size_t _in_srclen, const char* _in_operation, 
			const char* _in_key, size_t _in_keylen, size_t _in_expiry, int _in_rkeylen, int* _ret_length);


#ifdef C_AUTHCODE_TEST
void c_authcode_test(int argc, char **argv);
#endif

#ifdef __cplusplus
}
#endif

#endif	// __tlv_c_authcode_h__
