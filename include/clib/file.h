
#ifndef __tlv_c_file_h__
#define __tlv_c_file_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/********************MODE**********************
			  rt and r are same
"r"		open and only read file, the file must exist
"w"		create and only writer file, if the file exist, it will be empty contents 
"a"		create and append to a only read file
"rb"	read binary file, the file must exist
"wb"	create and only writer binary file, if the file exist, it will be empty contents  
"ab"	create and append to a noly read binary file
"r+"	open a file to read and writer, the file must exist
"w+"	create a file to read and writer, if the file exist, it will be empty contents 
"a+"	create and open a file to read and writer
"rb+"	open a binary file to read and writer, the file must exist
"rt+"	open a file to read and writer, the file must exist
"wb+"	create and open a binary file to read and writer
"wt+"	create and open a text file to read and writer
"ab+"	create and append a binary file to read and writer
"at+"	create and append a text file to read and writer
	
***********************************************/

long c_file_size(FILE* _fp);
char* c_file_read(const char* _filename, const char* _mode, long* out_length);

#ifdef __cplusplus
}
#endif

#endif	// __tlv_c_file_h__
