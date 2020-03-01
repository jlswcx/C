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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

#ifndef __tlv_c_base64_h__
#define __tlv_c_base64_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
  Calculate how much memory needed for dst of base64_encode() ���˽����� */
int c_base64_needed_encoded_length(int length_of_data);

/*
  Calculate how much memory needed for dst of base64_decode() ���˽����� */
int c_base64_needed_decoded_length(int length_of_encoded_data);


#define C_BASE64_LF				0x00000010
#define C_BASE64_CRLF			0x00000100
#define C_BASE64_NOPADDING		0x00010000
#define C_BASE64_URLSAFE		0x00100000
/*
  1.DEFAULT ���������Ĭ�ϣ�ʹ��Ĭ�ϵķ��������� ������ ������=���
  2.CRLF ��������������Ƚ����죬������Win���Ļ��з�����˼����ʹ��CR LF��һ����Ϊһ�еĽ�β������Unix����LF
  3.NOPADDING �����������ȥ�����ַ������ġ�=��
  5.URLSAFE ���������˼�Ǽ���ʱ��ʹ�ö�URL���ļ���������������ַ�����Ϊ�����ַ������������-��_ȡ��+��/
*/

/*
  Encode data as a base64 string */
int c_base64_encode(const void* _in_src, size_t _in_srclen, char* _out_dst);

/*
  Decode a base64 string into data */
int c_base64_decode(const char *_in_src, size_t _in_srclen, void *_out_dst);

/*
  Encode data as a base64 string with flag 
  һ�� _line_size ����0  Ĭ��Ϊ \n ���� 
  _line_size Ϊ4�ı��� _flag ������ \r\n ��Ч */
int c_base64_needed_encoded_length_ex(int _length_of_data, int _line_size, size_t _flag);

int c_base64_encode_ex(const void* _in_src, size_t _in_srclen, char* _out_dst, int _in_line_size, size_t _in_flag);

/*
  l Ϊÿ�гߴ�
*/
#define C_BASE64_ENLEN(x)	c_base64_needed_encoded_length(x)
#define C_BASE64_DELEN(x)	c_base64_needed_decoded_length(x)

#define C_BASE64_ENLEN_EX(x,l,f) c_base64_needed_encoded_length_ex(x,l,f)


#ifdef C_BASE64_TEST
void c_base64_test(int argc, char **argv);
#endif


#ifdef __cplusplus
}
#endif

#endif /* !__tlv_c_base64_h__ */
