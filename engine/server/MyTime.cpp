#include "MyTime.h"
#include <time.h>
#ifdef WIN32
#   include <windows.h>
#else
#   include <sys/time.h>
#endif

#ifdef WIN32
int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year     = wtm.wYear - 1900;
	tm.tm_mon     = wtm.wMonth - 1;
	tm.tm_mday     = wtm.wDay;
	tm.tm_hour     = wtm.wHour;
	tm.tm_min     = wtm.wMinute;
	tm.tm_sec     = wtm.wSecond;
	tm. tm_isdst    = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;

	return (0);
}
#endif


double MyTime::getNowSecs()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec;
}
double MyTime::getNowMSecs()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000 + tv.tv_usec/1000;
}

std::string MyTime::getNowTime()
{
	 time_t tt = time(NULL);
	 tm* t= localtime(&tt);
	 char buffer[1024] = {'\0'};
	 sprintf(buffer,"%d-%02d-%02d %02d:%02d:%02d", 
	  t->tm_year + 1900,
	  t->tm_mon + 1,
	  t->tm_mday,
	  t->tm_hour,
	  t->tm_min,
	  t->tm_sec);
	 return buffer;
}

std::string MyTime::getNowTime(unsigned long nowTime)
{
	time_t tt = nowTime;
	tm* t= localtime(&tt);
	char buffer[1024] = {'\0'};
	sprintf(buffer,"%d_%02d_%02d-%02d.%02d.%02d", 
	t->tm_year + 1900,
	t->tm_mon + 1,
	t->tm_mday,
	t->tm_hour,
	t->tm_min,
	t->tm_sec);
	 return buffer;
}