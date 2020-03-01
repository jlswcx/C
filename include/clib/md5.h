/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2015 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Alexander Peslyak (Solar Designer) <solar at openwall.com>   |
   |         Rasmus Lerdorf <rasmus@lerdorf.on.ca>                        |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef __tlv_c_md5_h__
#define __tlv_c_md5_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void c_md5_make_digest(char *md5str, const unsigned char *digest);
void c_md5_make_digest_ex(char *md5str, const unsigned char *digest, int len, int caps);

/*
 * This is an OpenSSL-compatible implementation of the RSA Data Security,
 * Inc. MD5 Message-Digest Algorithm (RFC 1321).
 *
 * Written by Solar Designer <solar at openwall.com> in 2001, and placed
 * in the public domain.  There's absolutely no warranty.
 *
 * See md5.c for more information.
 */

/* MD5 context. */
typedef struct {
	size_t lo, hi;
	size_t a, b, c, d;
	unsigned char buffer[64];
	size_t block[16];
} c_md5_ctx;


void c_md5_init(c_md5_ctx *ctx);
void c_md5_update(c_md5_ctx *ctx, const void *data, size_t size);
void c_md5_final(unsigned char *result, c_md5_ctx *ctx);


void c_md5(const char* _in_src, size_t _in_srclen, char* _out_md5str);
void c_md5_file(const char* _in_filename, int _in_caps, char* _out_md5str);

void c_md5_ex(const char* _in_src, size_t _in_srclen, int _in_caps, char* _out_md5str);


#ifdef C_MD5_TEST
void c_md5_test(int argc, char **argv);
#endif


#ifdef __cplusplus
}
#endif

#endif	// !__tlv_c_md5_h__
