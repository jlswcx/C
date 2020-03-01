
#include "stdc.h"


#ifdef _trace_on_

void c_debugtrace(const char *szformat, ...)
{
	if (szformat != null)
	{
		va_list args;

		FILE* fp = null;
		int i = 0, nbuf = 0;

		char zbuffer[256] = { 0 };
		char tbuffer[256] = { 0 };
		
		struct tm* stm = null;
		time_t t = time(null);	
		char timebuf[20] = { 0 };
		char* szfmt = null;
		int h = 0; stm = gmtime(&t);
		h = stm->tm_hour + 8;
		if (h >= 24) { h -= 24; }
		stm->tm_hour = h;
		szfmt = "%Y-%m-%d %H:%M:%S";
		strftime(timebuf, 20, szfmt, stm);		// strftime _sizeinbyte contain null

		va_start(args, szformat);
		nbuf = vsnprintf(tbuffer, sizeof(tbuffer), szformat, args);
		va_end(args);
		
		if (nbuf > 0)
		{
			sprintf(zbuffer, "[%s]\t%s", timebuf, tbuffer);
		}
		else
		{
			sprintf(zbuffer, "[%s]\tError ID : %d : %s", timebuf, errno, strerror(errno));
		}

		fp = fopen("trace.log", "ab+");
		if(fp != null)
		{
			while (zbuffer[i] != 0)
			{
				putc(zbuffer[i++], fp);
			}
			putc('\r', fp);
			putc('\n', fp);

			fclose(fp);
		}	
	}
}

#else

void c_debugtrace_a(const char *szformat, ...) { }

#endif

