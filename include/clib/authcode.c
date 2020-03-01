
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/timeb.h>

#include "md5.h"
#include "base64.h"
#include "malloc.h"
#include "authcode.h"

char* c_authcode(const char* _src, size_t _srclen, const char* _operation, 
			const char* _key, size_t _keylen, size_t _expiry, int _rkeylen, int* _ret_length)
{
	const char* src	= _src;
	const char* key_ = _key;
	const char* operation = _operation;
	size_t srclen  = _srclen;
	size_t keylen_ = _keylen;

	int ckey_length = _rkeylen;

	char key[33]  = {0};
	char keya[33] = {0};
	char keyb[33] = {0};
	char keyc[33] = {0};

	size_t nowtime = 0;
	struct timeb timebuffer;

	char timestr[11] = {0};
	char timemd5str[33] = {0};

	char keyac[65] = {0};
	char keyacmd5str[33] = {0};
	char cryptkey[65] = {0};

	size_t key_length = 0;

	char* string = NULL;
	size_t stringlen = 0;

	char* srckeyb = NULL;
	char srckeybmd5str[33] = {0};

	char* result = NULL;

	unsigned char box[256] = {0};
	unsigned char rndkey[256] = {0};
	size_t i = 0, j = 0, tmp = 0, a = 0, t = 0;

	size_t retlength = 0;

	if(_ret_length) *_ret_length = retlength;

	// $_key = md5($key ? $key : $GLOBALS['discuz_auth_key']);  	
	c_md5(key_, keylen_, key);
	// $keya = md5(substr($_key, 0, 16));
	c_md5(key, 16, keya);
	// $keyb = md5(substr($_key, 16, 16));
	c_md5(&key[16], 16, keyb);

	ftime(&timebuffer);
	nowtime = timebuffer.time;
//	nowtime = time(NULL);


	// $keyc = $ckey_length ? ($operation == 'DECODE' ? substr($string, 0, $ckey_length): substr(md5 (microtime()), -$ckey_length)) : '';	
	if(ckey_length > 0)
	{
		if(!strcmp(_operation, "DECODE"))
		{
			memcpy(keyc, src, ckey_length);
		}
		else
		{
			sprintf(timestr, "%010d", nowtime + timebuffer.millitm);
			c_md5(timestr, strlen(timestr), timemd5str);
			memcpy(keyc, &timemd5str[32 - ckey_length], ckey_length);
		}
	}

	// $cryptkey = $keya.md5($keya.$keyc); 
	memcpy(keyac, keya, 32);
	memcpy(&keyac[32], keyc, ckey_length);
	c_md5(keyac, strlen(keyac), keyacmd5str);
	memcpy(cryptkey, keya, 32);
	memcpy(&cryptkey[32], keyacmd5str, 32);

	// $key_length = strlen($cryptkey);  64
	key_length = strlen(cryptkey);

	// $string = $operation == 'DECODE' ? base64_decode(substr($string, $ckey_length)) : sprintf('%010d', $expiry ? $expiry + time() : 0).substr(md5($string.$keyb), 0, 16).$string; 
	if( !strcmp(_operation, "DECODE"))
	{
		int base64len = srclen - ckey_length;
		int delen = C_BASE64_DELEN(base64len);
		string = (char*)c_alloc(delen);
		stringlen = c_base64_decode(&src[ckey_length], base64len, string);
	}
	else if(strcmp(_operation, "ENCODE") == 0)
	{
		// sprintf('%010d', $expiry ? $expiry + time() : 0) 
		if(_expiry > 0)
		{
			_expiry = _expiry + nowtime;			
		}
		sprintf(timestr, "%010d", _expiry);

		// substr(md5($string.$keyb), 0, 16)		
		srckeyb = c_alloc(srclen + 32);
		memcpy(srckeyb, src, srclen);
		memcpy(&srckeyb[srclen], keyb, 32);
		c_md5(srckeyb, srclen + 32, srckeybmd5str);
		c_free(srckeyb);

		// sprintf('%010d', $expiry ? $expiry + time() : 0).substr(md5($string.$keyb), 0, 16).$string;
		string = c_alloc(10+16+srclen);
		memcpy(string,timestr,10);
		memcpy(string+10,srckeybmd5str,16);
		memcpy(string+26,src,srclen);

		stringlen = 26 + srclen;
	}

//	result = c_alloc(stringlen);
	
	// $box = range(0, 255); 
	for(i = 0; i < 256; i++)
	{
		box[i] = (unsigned char)i;
	}
		
	// $rndkey = array(); 
	// for($i = 0; $i <= 255; $i++) { 
    //		$rndkey[$i] = ord($cryptkey[$i % $key_length]); 
    // } 
	for(i = 0; i < 256; i++)
	{
		rndkey[i] = (unsigned char)cryptkey[i % key_length];
	}
		
	// for($j = $i = 0; $i < 256; $i++) { 
    //   $j = ($j + $box[$i] + $rndkey[$i]) % 256; 
    //   $tmp = $box[$i]; 
    //   $box[$i] = $box[$j]; 
    //   $box[$j] = $tmp; 
    // } 
	for(i = 0, j = 0; i < 256; i++)
	{
		j = (j + box[i] + rndkey[i]) % 256;
		tmp = box[i];
		box[i] = box[j];
		box[j] = (unsigned char)tmp;
	}

	// for($a = $j = $i = 0; $i < $string_length; $i++) { 
    //    $a = ($a + 1) % 256; 
    //    $j = ($j + $box[$a]) % 256; 
    //    $tmp = $box[$a]; 
    //    $box[$a] = $box[$j]; 
    //    $box[$j] = $tmp; 
    //    // \u4ECE\u5BC6\u5319\u7C3F\u5F97\u51FA\u5BC6\u5319\u8FDB\u884C\u5F02\u6216\uFF0C\u518D\u8F6C\u6210\u5B57\u7B26 
    //    $result .= chr(ord($string[$i]) ^ ($box[($box[$a] + $box[$j]) % 256])); 
    // } 
	for(a = 0, j = 0, i = 0; i < stringlen; i++)
	{
		a = (a + 1) % 256;
		j = (j + box[a]) % 256;
		tmp = box[a];
		box[a] = box[j];
		box[j] = (unsigned char)tmp;

		string[i] = (string[i] ^ (box[(box[a] + box[j]) % 256]));
//		printf("%d\n", $result[i]);

	}

	//if($operation == 'DECODE') { 
    //    // substr($result, 0, 10) == 0
    //    // substr($result, 0, 10) - time() > 0 
    //    // substr($result, 10, 16) == substr(md5(substr($result, 26).$keyb), 0, 16)
    //
    //    if((substr($result, 0, 10) == 0 || substr($result, 0, 10) - time() > 0) && substr($result, 10, 16) == substr(md5(substr($result, 26).$keyb), 0, 16)) { 
    // //       return substr($result, 26); 
    //    } else { 
    //        return ''; 
    //    } 
    //} else { 
    //   return $keyc.str_replace('=', '', base64_encode($result)); 
    //} 
	if(!strcmp(_operation, "DECODE"))
	{	
		size_t exptime = 0;
		char* tempstring = NULL;
		char tempsrckeyb[17] = {0};
		
		memcpy(timestr, string, 10);
		memcpy(tempsrckeyb,string+10,16);
		tempstring = string + 26;
		exptime = atoi(timestr);

		srckeyb = c_alloc(stringlen-26 + 32);
		memcpy(srckeyb, tempstring, stringlen-26);
		memcpy(&srckeyb[stringlen-26], keyb, 32);
		c_md5(srckeyb, stringlen-26 + 32, srckeybmd5str);
		srckeybmd5str[16] = '\0';
		c_free(srckeyb);

		if((exptime == 0 || (exptime - time(NULL)) > 0) && !strcmp(tempsrckeyb, srckeybmd5str))
		{
			result = c_alloc(stringlen-26);
			memcpy(result,string + 26,stringlen-26);
			retlength = stringlen-26;
		}	
	}
	else
	{
		// $keyc.str_replace('=', '', base64_encode($result)); 
		size_t ensize = C_BASE64_ENLEN(stringlen);
		result = c_alloc(ckey_length + ensize);
		memcpy(result, keyc, ckey_length);
		retlength = c_base64_encode_ex(string, stringlen, &result[ckey_length], 0, C_BASE64_NOPADDING | C_BASE64_URLSAFE);
		retlength += ckey_length;	
	}

	c_free(string);

	if(_ret_length){
		*_ret_length = retlength;
	}

	return result;
}

#ifdef C_AUTHCODE_TEST

void c_authcode_test(int argc, char **argv)
{
	char buffer[4096] = {0};
	size_t nbuf = 0;
	char* enstring = NULL;
	int enlen = 0;
	char* key = "e66a6dd11454c20788b7142de5101603密码";
	int rkeylen = 10;
	size_t retlength = 0;
	char* result = NULL;

	c_block* pblock = NULL;

	if(0)
		goto _authcode;

	printf("请输入字符串>>");
	fgets(buffer, 4096, stdin);			// 换行时 fgets 会接受 \n 字符
	buffer[strlen(buffer) - 1] = '\0';

	while (strcmp(buffer, "exit"))
	{	
		nbuf = strlen(buffer);
		result = c_authcode(buffer, nbuf, "ENCODE", key, strlen(key), 0, rkeylen, &retlength);
		printf("加密长度为%d\n%s\n\n", retlength, result);

//		pblock = c_blockdata(result);
//		printf("%x - %d\n", pblock->pplex, ((c_plex*)pblock->pplex)->nfree);

		c_free(result);

		printf("请输入字符串>>");
		fgets(buffer, 4096, stdin);			// 换行时 fgets 会接受 \n 字符
		buffer[strlen(buffer) - 1] = '\0';
	}

	return;

_authcode:

	printf("请输入已加密字符串>>");
	fgets(buffer, 4096, stdin);			// 换行时 fgets 会接受 \n 字符
	buffer[strlen(buffer) - 1] = '\0';

	while (strcmp(buffer, "exit"))
	{
		nbuf = strlen(buffer);
		result = c_authcode(buffer, nbuf, "DECODE", key, strlen(key), 0, rkeylen, &retlength);
		printf("加密长度为%d\n%s\n\n", retlength, result);
		c_free(result);

		printf("请输入字符串>>");
		fgets(buffer, 4096, stdin);			// 换行时 fgets 会接受 \n 字符
		buffer[strlen(buffer) - 1] = '\0';
	}
}

#endif


