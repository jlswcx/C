
#include <string.h>
#include <ctype.h>

#include "mstring.h"


char* c_ltrim(char *_string) 
{  
	char* string = _string;
	char* tmp = _string;
	int i = 0;

	if(string == NULL) return NULL;

	while (*string != '\0' && isspace(*string))
	{
		i++;
		string++;
	}
	if(i)		/* 说明有空格存在 */
	{
		do {
			*tmp++ = *string;
		}while(*string++ != '\0');			/* 将字符串前移 */
	}
	return _string;
}  
  
char* c_rtrim(char *_string) 
{  
	char* string = _string;
    int i = strlen(string) - 1; 

	if(string == NULL) return NULL;

    while (i >= 0) {  
		if(!isspace(string[i]))
			break;  
        i--;  
    }  
    _string[++i] = '\0';
	return _string;
}  
  
char* c_trim(char *_string) 
{  
	if(_string == NULL) return NULL;

    c_ltrim(_string);  
    c_rtrim(_string); 
	return _string;
}  

char* c_string_en_urlsafe(char* string)
{
	char* tempstr = string;
	if (!tempstr) return NULL;
	while(*tempstr!='\0')
	{
		if(*tempstr == '=')
			*tempstr = '-';
		else if(*tempstr == '/')
			*tempstr = '!';
		else if(*tempstr == '+')
			*tempstr = '_';
		tempstr++;
	}
	return string;
}


char* c_string_de_urlsafe(char* string)
{
	char* tempstr = string;
	if (!tempstr) return NULL;
	while(*tempstr!='\0')
	{
		if(*tempstr == '-')
			*tempstr = '=';
		else if(*tempstr == '!')
			*tempstr = '/';
		else if(*tempstr == '_')
			*tempstr = '+';
		tempstr++;
	}
	return string;
}


