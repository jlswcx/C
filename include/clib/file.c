
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>

#include "file.h"
#include "malloc.h"


char* c_file_read(const char* _filename, const char* _mode, long* _out_length)
{
	FILE* fp = NULL;
	char* filestream = NULL;
	long file_length = 0;

	fp = fopen(_filename, _mode);
	if(fp)
	{
		file_length = c_file_size(fp);
		filestream = c_alloc(file_length);
		fread(filestream, file_length, 1, fp);
		fclose(fp);
	}

	if(_out_length)
		*_out_length = file_length;
	return filestream;
}




long c_file_size(FILE* _fp)
{
	long flen = 0, oldsite = 0;

	oldsite = ftell(_fp);			// ftell get current point site  save the old  site error  return -1 
	fseek(_fp, 0, SEEK_END);		// fseek move point site error return -1 succeed return 0
	flen = ftell(_fp);
	fseek(_fp, oldsite, SEEK_SET);

	return flen;	
}








