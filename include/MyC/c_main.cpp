
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "c_malloc.h"
#include "c_md5.h"
#include "c_base64.h"
#include "c_authcode.h"
#include "c_libcurl.h"
#include "c_datcode.h"


void main(int argc, char **argv)
{
	int i;
	for(i = 0; i < argc; i++)
	{
//		printf("%s\n", argv[i]);
	}

	c_malloc_test();
//	c_md5_test(argc, argv);
//	c_base64_test(argc, argv);
//	c_authcode_test(argc, argv);
//	c_curl_test();
//	c_dati_test();


	system("pause");
}
