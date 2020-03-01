
#include "c_file.h"
#include "c_malloc.h"



char* c_file_read(const char* _filename, const char* _mode, long* _out_length)
{
	FILE* fp = NULL;
	char* filestream = NULL;
	long file_length = 0;

	fp = fopen(_filename, _mode);
	if(fp)
	{
		file_length = c_file_getsize(fp);
		filestream = c_alloc(file_length);
		fread(filestream, file_length, 1, fp);
		fclose(fp);
	}

	if(_out_length)
		*_out_length = file_length;
	return filestream;
}




long c_file_getsize(FILE* _fp)
{
	long flen = 0, oldsite = 0;

	oldsite = ftell(_fp);			// ftell get current point site  save the old  site error  return -1 
	fseek(_fp, 0, SEEK_END);		// fseek move point site error return -1 succeed return 0
	flen = ftell(_fp);
	fseek(_fp, oldsite, SEEK_SET);

	return flen;	
}








