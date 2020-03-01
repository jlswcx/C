
#ifndef __tlv_c_dll_dati_h__
#define __tlv_c_dll_dati_h__

#pragma once

#include <string>

std::string SendImage(std::string _Username, std::string _Password, std::string _ProjectId,
					  int x, int y, int w, int h, int hWnd, int _timeout, std::string _Worker);




#endif	// !__tlv_c_dll_dati_h__
