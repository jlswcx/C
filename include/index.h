

#ifndef _TLV_INCLUDE_INDEX_H_
#define _TLV_INCLUDE_INDEX_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _CRT_SECURE_NO_WARNINGS

// ********Lua*********
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
#pragma comment(lib, "lua.lib") 

// *******Sqlite3******
#include "sqlite3/sqlite3.h"
// #include "sqlite3ext.h"	 不要包含这个文件 不然会出现 未定义 sqlite3_api 编译错误  
#pragma comment(lib, "sqlite3.lib") 

// *******Pcre******
#include "pcre/pcre.h"
#include "pcre/pcreposix.h"
#pragma comment(lib, "pcre.lib") 

// *******MySQL******

// ***MySQL扩展函数**
#include "mysql/my_global.h"
#include "mysql/base64.h"
#include "mysql/my_xml.h"
#include "mysql/my_md5.h"
#include "mysql/my_sys.h"
#include "mysql/mysql.h"
#pragma comment(lib, "mysqlclient.lib") 


// *******Zlib*******
#include "zlib/zlib.h"
#pragma comment(lib, "zlib.lib") 


// *******Stdc*******
#include "stdc/stdc.h"
#pragma comment(lib, "stdc.lib") 

//*******************
#ifdef __cplusplus
}
#endif

#endif	// _TLV_INCLUDE_INDEX_H_