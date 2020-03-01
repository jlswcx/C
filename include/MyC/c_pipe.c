
#include <tlv.h>

#ifdef __cplusplus
extern "C" {
#endif

//	char* result = nullptr;
//	char* buffer = c_strcat_a("c://windows//system32//cmd.exe /c ", _command);
//	c_free_a(&buffer);

char* c_readcmd_a(_tlv_in_ const char* _command)
{
	char* v_command = c_strcat_a(_command, " 2>&1");
	char* v_result = c_readpipe_a(v_command);
	c_free_a(&v_command);
	return v_result;
}


wchar_t* c_readcmd_w(_tlv_in_ const wchar_t* _command)
{
	wchar_t* v_command = c_strcat_w(_command, L" 2>&1");
	wchar_t* v_result = c_readpipe_w(v_command);
	c_free_w(&v_command);
	return v_result;
}


void c_writecmd_a(_tlv_in_ const char* _command)
{
	c_writepipe_a(_command);
}


void c_writecmd_w(_tlv_in_ const wchar_t* _command)
{
	c_writepipe_w(_command);
}


char* c_dir_a()
{
	int v_st = 0;

	char* cd = c_readcmd_a("cd");
	if(cd == nullptr) return nullptr;

	v_st = c_findchr_a(cd, '\n');
	if(v_st != c_str_err)
	{
		cd[v_st] = '\0';
		return cd;
	}
	c_free_a(&cd);
	return nullptr;
}


wchar_t* c_dir_w()
{
	wchar_t* cd = nullptr;
	int v_st = 0;

	cd = c_readcmd_w(L"cd");
	if(cd == nullptr) return nullptr;

	v_st = c_findchr_w(cd, L'\n');
	if(v_st != c_str_err)
	{
		cd[v_st] = '\0';
		return cd;
	}
	c_free_w(&cd);
	return nullptr;
}


bool c_mkdir_a(_tlv_in_ const char* dirpath, _tlv_in_ const char* dirname)
{
	FILE* fp = nullptr;
	int result = EOF;

	char* s1 = nullptr;
	char* s2 = nullptr;
	char* s3 = nullptr;
	char* s4 = nullptr;

	c_assert(dirname != nullptr);
	if(dirname == nullptr) return false;

	fp = _popen("c://windows//system32//cmd.exe", "wt");			// error return nullptr
	c_assert(fp != nullptr);
	if(fp == nullptr) return false;

	if(dirpath == nullptr)
	{
		s1 = c_strcat_a("mkdir \"", dirname);
		s2 = c_strcat_a(s1, "\"\r\n");
		result = fputs(s2, fp);
//		result = fwrite(s2, sizeof(char), c_strlen_a(s2), fp);
		c_free_a(&s2);
		c_free_a(&s1);
		if(result == EOF) return false;
	}
	else
	{
		s1 = c_strcat_a("mkdir \"", dirpath);
		s2 = c_strcat_a(s1, "\\");
		s3 = c_strcat_a(s2, dirname);
		s4 = c_strcat_a(s3, "\"\r\n");
		result = fputs(s4, fp);
//		result = fwrite(s4, sizeof(char), c_strlen_a(s4), fp);
		c_free_a(&s4);
		c_free_a(&s3);
		c_free_a(&s2);
		c_free_a(&s1);
		if(result == EOF) return false;
	}

	_pclose(fp);
	
	return true;
}


bool c_mkdir_w(_tlv_in_ const wchar_t* dirpath, _tlv_in_ const wchar_t* dirname)
{
	FILE* fp = nullptr;
	int result = WEOF;

	wchar_t* s1 = nullptr;
	wchar_t* s2 = nullptr;
	wchar_t* s3 = nullptr;
	wchar_t* s4 = nullptr;

	c_assert(dirname != nullptr);
	if(dirname == nullptr) return false;

	fp = _wpopen(L"c://windows//system32//cmd.exe", L"wt");			// error return nullptr
	c_assert(fp != nullptr);
	if(fp == nullptr) return false;

	if(dirpath == nullptr)
	{
		s1 = c_strcat_w(L"mkdir \"", dirname);
		s2 = c_strcat_w(s1, L"\"\r\n");
		result = fputws(s2, fp);
//		result = fwrite(s2, sizeof(char), c_strlen_w(s2), fp);
		c_free_w(&s2);
		c_free_w(&s1);
		if(result == WEOF) return false;
	}
	else
	{
		s1 = c_strcat_w(L"mkdir \"", dirpath);
		s2 = c_strcat_w(s1, L"\\");
		s3 = c_strcat_w(s2, dirname);
		s4 = c_strcat_w(s3, L"\"\r\n");
		result = fputws(s4, fp);
//		result = fwrite(s4, sizeof(wchar_t), c_strlen_w(s4), fp);
		c_free_w(&s4);
		c_free_w(&s3);
		c_free_w(&s2);
		c_free_w(&s1);
		if(result == WEOF) return false;
	}

	_pclose(fp);
	
	return true;
}


bool c_mvdir_a(_tlv_in_ const char* _dirname)
{
	FILE* fp = nullptr;
	int result = EOF;

	char* s1 = nullptr;
	char* s2 = nullptr;

	c_assert(_dirname != nullptr);
	if(_dirname == nullptr) return false;

	fp = _popen("c://windows//system32//cmd.exe", "wt");			// error return nullptr
	c_assert(fp != nullptr);
	if(fp == nullptr) return false;

	s1 = c_strcat_a("rd /s /q \"", _dirname);
	s2 = c_strcat_a(s1, "\"\r\n");
	result = fputs(s2, fp);
	c_free_a(&s2);
	c_free_a(&s1);
	if(result == EOF) return false;

	_pclose(fp);
	
	return true;
}


bool c_mvdir_w(_tlv_in_ const wchar_t* _dirname)
{
	FILE* fp = nullptr;
	int result = WEOF;

	wchar_t* s1 = nullptr;
	wchar_t* s2 = nullptr;

	c_assert(_dirname != nullptr);
	if(_dirname == nullptr) return false;

	fp = _wpopen(L"c://windows//system32//cmd.exe", L"wt");			// error return nullptr
	c_assert(fp != nullptr);
	if(fp == nullptr) return false;

	s1 = c_strcat_w(L"rd /s /q \"", _dirname);
	s2 = c_strcat_w(s1, L"\"\r\n");
	result = fputws(s2, fp);
	c_free_w(&s2);
	c_free_w(&s1);
	if(result == WEOF) return false;

	_pclose(fp);
	
	return true;
}


bool c_rename_a(_tlv_in_ const char* _oldname, _tlv_in_ const char* _newname)
{
	FILE* fp = nullptr;
	int result = EOF;

	char* s1 = nullptr;
	char* s2 = nullptr;
	char* s3 = nullptr;
	char* s4 = nullptr;

	c_assert(_oldname != nullptr);
	c_assert(_newname != nullptr);
	if(_oldname == nullptr || _newname == nullptr) return false;

	fp = _popen("c://windows//system32//cmd.exe", "wt");			// error return nullptr
	c_assert(fp != nullptr);
	if(fp == nullptr) return false;

	s1 = c_strcat_a("rename \"", _oldname);
	s2 = c_strcat_a(s1, "\" \"");
	s3 = c_strcat_a(s2, _newname);
	s4 = c_strcat_a(s3, "\"\r\n");
	result = fputs(s4, fp);
	c_free_a(&s4);
	c_free_a(&s3);
	c_free_a(&s2);
	c_free_a(&s1);
	if(result == EOF) return false;

	_pclose(fp);
	
	return true;
}


bool c_rename_w(_tlv_in_ const wchar_t* _oldname, _tlv_in_ const wchar_t* _newname)
{
	FILE* fp = nullptr;
	int result = WEOF;

	wchar_t* s1 = nullptr;
	wchar_t* s2 = nullptr;
	wchar_t* s3 = nullptr;
	wchar_t* s4 = nullptr;

	c_assert(_oldname != nullptr);
	c_assert(_newname != nullptr);
	if(_oldname == nullptr || _newname == nullptr) return false;

	fp = _wpopen(L"c://windows//system32//cmd.exe", L"wt");			// error return nullptr
	c_assert(fp != nullptr);
	if(fp == nullptr) return false;

	s1 = c_strcat_w(L"rename \"", _oldname);
	s2 = c_strcat_w(s1, L"\" \"");
	s3 = c_strcat_w(s2, _newname);
	s4 = c_strcat_w(s3, L"\"\r\n");
	result = fputws(s4, fp);
	c_free_w(&s4);
	c_free_w(&s3);
	c_free_w(&s2);
	c_free_w(&s1);
	if(result == WEOF) return false;

	_pclose(fp);
	
	return true;
}


#ifdef __cplusplus
}
#endif

