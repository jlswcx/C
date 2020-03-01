/* Copyright (C) 2003 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>		/* isspace() */
#include <stdio.h>

#include "base64.h"


static char c_base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
							   "abcdefghijklmnopqrstuvwxyz"
							   "0123456789+/";


int c_base64_needed_encoded_length(int length_of_data)
{
  int nb_base64_chars;
  nb_base64_chars = (length_of_data + 2) / 3 * 4;

  return
	 nb_base64_chars + 1;	/* NUL termination of string */
}

int 
c_base64_needed_encoded_length_ex(int _length_of_data, int _line_size, size_t _flag)
{
  int nb_base64_chars;
  int padding = 1;										/* \r\n or \n string size*/
  nb_base64_chars = (_length_of_data + 2) / 3 * 4;

  if(_flag & C_BASE64_CRLF)
	  padding = 2;

  if(_line_size)
     return
		nb_base64_chars +								/* base64 char incl padding */
		((nb_base64_chars - 1)/ _line_size)*padding +	/* newlines 76 */
		1;												/* NUL termination of string */
  else
	 return
		nb_base64_chars + 1;
}

int
c_base64_needed_decoded_length(int length_of_encoded_data)
{
  return (int) ceil(length_of_encoded_data * 3 / 4) + 
				1;							/* NUL termination of string */
}


/*
  Encode a data as base64.

  Note: We require that dst is pre-allocated to correct size.
        See my_base64_needed_encoded_length().
*/

/*
  返回实际长度 不包括“==”号
*/
int c_base64_encode(const void* _src, size_t _srclen, char* _dst)
{
	return c_base64_encode_ex(_src, _srclen, _dst, 0, 0);
}

#define c_base64_urlsafe(dst, v)				\
{												\
	*dst = v;									\
	if(_flag & C_BASE64_URLSAFE)				\
	{											\
		if(*dst == '=')							\
			*dst = '-';							\
		else if(*dst == '/')					\
			*dst = '!';							\
		else if(*dst == '+')					\
			*dst = '_';							\
	}											\
	dst++;										\
	ret++;										\
}


int c_base64_encode_ex(const void* _src, size_t _srclen, char* _dst, int line_size, size_t _flag)
{
  const unsigned char *s= (const unsigned char*)_src;
  size_t i= 0;
  int len= 0;
  size_t flag = _flag;
  char* dst = _dst;
  int ret = 0;				/* return the encode length */

  if(s == NULL)
	  _srclen = 0;

  for (; i < _srclen; len += 4)
  {
    unsigned c;

    if (line_size > 0 && len >= line_size)
    {
      len= 0;
	  if(flag & C_BASE64_CRLF)
	  {
		  *dst++= '\r';
		  ret++;
	  }
	  *dst++= '\n';
	  ret++;
    }

    c= s[i++];
    c <<= 8;

    if (i < _srclen)
      c += s[i];
    c <<= 8;
    i++;

    if (i < _srclen)
      c += s[i];
    i++;

    c_base64_urlsafe(dst, c_base64_table[(c >> 18) & 0x3f]);
	c_base64_urlsafe(dst, c_base64_table[(c >> 12) & 0x3f]);

	if (i > (_srclen + 1)){
	  if(flag & C_BASE64_NOPADDING)
	  {
		*dst++= '\0';
	  }
	  else
		c_base64_urlsafe(dst, '='); 
	}else
      c_base64_urlsafe(dst, c_base64_table[(c >> 6) & 0x3f]);

	if (i > _srclen){
	  if(flag & C_BASE64_NOPADDING)
	  {
		*dst++= '\0';
	  }
	  else
		c_base64_urlsafe(dst, '=');
	}else
      c_base64_urlsafe(dst, c_base64_table[(c >> 0) & 0x3f]);
  }
  *dst= '\0';

  return ret;
}


#define c_base64_pos(s)															\
  c += (size_t) (strchr(c_base64_table, (unsigned char)s) - c_base64_table)		\



#define c_skip_space(src, i, size)                               \
{                                                               \
  while (i < size && isspace(*src))								\
  {                                                             \
    i++;                                                        \
    src++;                                                      \
  }																\
  tmp = *src++;													\
																\
  if(tmp == '-')												\
	tmp = '=';													\
  else if(tmp == '!')											\
	tmp = '/';													\
  else if(tmp == '_')											\
	tmp = '+';													\
																\
  if(i == size || tmp == '\0' || tmp == '=')					\
	break;														\
}

#define c_skip_space_ex(src, i, size)                           \
{                                                               \
  while (i < size && isspace(*src))								\
  {                                                             \
    i++;                                                        \
    src++;                                                      \
  }																\
  tmp = *src++;													\
																\
  if(tmp == '-')												\
	tmp = '=';													\
  else if(tmp == '!')											\
	tmp = '/';													\
  else if(tmp == '_')											\
	tmp = '+';													\
}

/*
  Decode a base64 string

  SYNOPSIS
    base64_decode()
    src      Pointer to base64-encoded string
    len      Length of string at 'src'
    dst      Pointer to location where decoded data will be stored
    end_ptr  Pointer to variable that will refer to the character
             after the end of the encoded data that were decoded. Can
             be NULL.

  DESCRIPTION

    The base64-encoded data in the range ['src','*end_ptr') will be
    decoded and stored starting at 'dst'.  The decoding will stop
    after 'len' characters have been read from 'src', or when padding
    occurs in the base64-encoded data. In either case: if 'end_ptr' is
    non-null, '*end_ptr' will be set to point to the character after
    the last read character, even in the presence of error.

  NOTE
    We require that 'dst' is pre-allocated to correct size.

  SEE ALSO
    my_base64_needed_decoded_length().

  RETURN VALUE
    Number of bytes written at 'dst' or -1 in case of failure

	该方法很操蛋 ， 一定要有 == 号存在才会成功
*/

int
c_base64_decode(const char *_src, size_t _srclen, void *_dst)
{
  char b[3];
  size_t i= 0;
  char *dst_base= (char *)_dst;
  char const *src= _src;
  char tmp;
  char *d= dst_base;
  size_t j;
  size_t len =_srclen;

  if(src == NULL)
	len = 0;

  while (i < len)
  {
    unsigned int c= 0;
    size_t mark= 0;

    c_skip_space(src, i, len);

	c_base64_pos(tmp);
    c <<= 6;
    i++;

    c_skip_space(src, i, len);

    c_base64_pos(tmp);
    c <<= 6;
    i++;

    c_skip_space_ex(src, i, len);

    if (tmp != '=' && tmp != '\0')
      c_base64_pos(tmp);
    else
    {
//	  src += 2;                // There should be two bytes padding 
	  i= len;
      mark= 2;
      c <<= 6;
      goto end;
    }
    c <<= 6;
    i++;

    c_skip_space_ex(src, i, len);

    if (tmp != '=' && tmp != '\0')
      c_base64_pos(tmp);
    else
    {
//	  src += 1;                 // There should be one byte padding 
	  i= len;
      mark= 1;
      goto end;
    }
    i++;

  end:
    b[0]= (c >> 16) & 0xff;
    b[1]= (c >>  8) & 0xff;
    b[2]= (c >>  0) & 0xff;

    for (j=0; j<3-mark; j++)
      *d++= b[j];
  }

//  if (end_ptr != NULL)
//    *end_ptr= src;

//  
//   The variable 'i' is set to 'len' when padding has been read, so it
//   does not actually reflect the number of bytes read from 'src'.
//   
//  return i != len ? -1 : (int) (d - dst_base);
  return (d - dst_base);
}



#ifdef C_BASE64_TEST

void c_base64_test(int argc, char **argv)
{

	char buffer[4096] = {0};
	size_t nbuf = 0;
	char* enstring = NULL;
	int enlen = 0;

	char* destring = NULL;
	int delen = 0;

	if(0)
		goto _enbase64;

	printf("请输入字符串>>\n");
	fgets(buffer, 4096, stdin);			// 换行时 fgets 会接受 \n 字符
	buffer[strlen(buffer) - 1] = '\0';

	while (strcmp(buffer, "exit"))
	{
		nbuf = strlen(buffer);
		enlen = C_BASE64_ENLEN_EX(nbuf, 0, C_BASE64_CRLF);
		enstring = (char*)malloc(enlen);
		memset(enstring, 0, enlen);
		enlen = c_base64_encode_ex(buffer, nbuf, enstring, 0, C_BASE64_CRLF | C_BASE64_URLSAFE);
		printf("字符串长度%d\n加密长度为%d\n%s\n\n", nbuf, enlen, enstring);

		// {{
		printf("开始解密...\n");
		delen = C_BASE64_DELEN(enlen);
		destring = (char*)malloc(delen);
		memset(destring, 0, delen);
		delen = c_base64_decode(enstring, enlen, destring);
		printf("字符长度为%d\n%s\n\n", delen, destring);
		free((void*)destring);
		// }}

		free((void*)enstring);

		printf("请输入字符串>>\n");
		fgets(buffer, 4096, stdin);			// 换行时 fgets 会接受 \n 字符
		buffer[strlen(buffer) - 1] = '\0';
	}

	return;

_enbase64:

	printf("请输入已加密字符串>>");
	fgets(buffer, 4096, stdin);			// 换行时 fgets 会接受 \n 字符
	buffer[strlen(buffer) - 1] = '\0';

	while (strcmp(buffer, "exit"))
	{
		nbuf = strlen(buffer);
		enlen = C_BASE64_DELEN(nbuf);
		enstring = (char*)malloc(enlen);
		memset(enstring, 0, enlen);
		enlen = c_base64_decode(buffer, nbuf, enstring);
		printf("原字符长度为%d\n%s\n\n", enlen, enstring);
		free((void*)enstring);

		printf("请输入已加密字符串>>");
		fgets(buffer, 4096, stdin);			// 换行时 fgets 会接受 \n 字符
		buffer[strlen(buffer) - 1] = '\0';
	}
}

#endif