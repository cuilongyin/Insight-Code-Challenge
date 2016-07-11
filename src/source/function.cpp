#include <cstring>
#include <string>
#include <ctime>
#include <cwchar>
#include <regex>
#include <cstdio>
#include <time.h>
#include "function.h"
#define _CRT_SECURE_NO_WARNINGS

time_t timeConvert (std::string a){

std::string stime=a;

const char * dateStr = stime.c_str();
int y,M,d,h,m,s;

sscanf(dateStr, "%d-%d-%dT%d:%d:%dZ", &y, &M, &d, &h, &m, &s);



tm time;
time.tm_year = y - 1900;
time.tm_mon = M - 1;
time.tm_mday = d;
time.tm_hour = h;
time.tm_min = m;
time.tm_sec = s;

time_t realtime;
realtime = mktime (&time);

return realtime;

}
