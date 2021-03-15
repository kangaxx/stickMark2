#ifndef __myLog_H_
#define __myLog_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <iostream>
#include <ratio>
#include <chrono>
#include <mutex>

#include <vector>
#include <fstream>  

#include <io.h>
#include <direct.h>

#define CLAMPXX(a, s, m) ((a) < (s)? (s) : ((a) > (m) ? (m) : (a)))

 std::string GetName(const char * fileName);

 std::string getCurrentSystemTime();

 long long getCurrentMs();

 int LOGINIT(int num = 3000);

 int LOGEnd();

 int LOGSetMaxSpace(int mSize);

 int logWrite(char * s);

//#define  LOGWT(...)

#define  LOGWT(...)  {\
	char temp[4196] = { 0 };      \
	sprintf(temp, "%s ", getCurrentSystemTime().c_str());  \
	sprintf(temp + strlen(temp), __VA_ARGS__); \
	sprintf(temp + strlen(temp), "\n");    \
	printf("%s", temp);   \
	logWrite(temp); \
}
#endif