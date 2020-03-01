
#include <clib.h>
#pragma comment(lib, "clib.lib")


void main(int argc, char **argv)
{
	int i;
	for(i = 0; i < argc; i++)
	{
//		printf("%s\n", argv[i]);
	}

//	c_malloc_test();
//	c_md5_test(argc, argv);
	c_base64_test(argc, argv);
//	c_authcode_test(argc, argv);
//	c_curl_test();
//	c_dati_test();

//	char input[120] = "        ";

//	char* s = c_rtrim(input);

	

	system("pause");
}
