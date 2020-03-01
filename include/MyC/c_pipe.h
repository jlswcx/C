
#ifndef __TLV_C_FUNCORE_DOSOP_H__
#define __TLV_C_FUNCORE_DOSOP_H__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


char* c_readcmd_a(_tlv_in_ const char* _command);
wchar_t* c_readcmd_w(_tlv_in_ const wchar_t* _command);

void c_writecmd_a(_tlv_in_ const char* _command);
void c_writecmd_w(_tlv_in_ const wchar_t* _command);

char* c_dir_a(void);
wchar_t* c_dir_w(void);

bool c_mkdir_a(_tlv_in_ const char* _dirpath, _tlv_in_ const char* _dirname);
bool c_mkdir_w(_tlv_in_ const wchar_t* _dirpath, _tlv_in_ const wchar_t* _dirname);

bool c_mvdir_a(_tlv_in_ const char* _dirname);
bool c_mvdir_w(_tlv_in_ const wchar_t* _dirname);

bool c_rename_a(_tlv_in_ const char* _oldname, _tlv_in_ const char* _newname);
bool c_rename_w(_tlv_in_ const wchar_t* _oldname, _tlv_in_ const wchar_t* _newname);


//void c_settime(_tlv_in_ char* _times);
//void c_settime(_tlv_in_ wchar_t* _times);

//void c_setdate(_tlv_in_ char* _dates);
//void c_setdate(_tlv_in_ wchar_t* _dates);



#ifdef _UNICODE
	#define c_readcmd 		c_readcmd_w
	#define c_writecmd 		c_writecmd_w

	#define c_runcmd 		c_runcmd_w

	#define c_dir			c_dir_w
	#define c_mkdir			c_mkdir_w
	#define c_mvdir			c_mvdir_w
	#define c_rename		c_rename_w

#else
	#define c_readcmd 		c_readcmd_a
	#define c_writecmd 		c_writecmd_a

	#define c_runcmd 		c_runcmd_a

	#define c_dir			c_dir_a
	#define c_mkdir			c_mkdir_a
	#define c_mvdir			c_mvdir_a
	#define c_rename		c_rename_a

#endif


#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __TLV_C_FUNCORE_DOSOP_H__
