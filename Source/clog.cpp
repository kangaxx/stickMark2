#include "cLog.h"
static std::string dumpRoot = "./cpplog"; //
static std::mutex mux;
static FILE * fp = NULL;
static int MaxLenChar = 1024 * 1024 * 50; // ,  1024 * 1024 * 5
static bool isSafe = 0;
static int  MaxLogNum = 100;

static int com(const void *a, const void *b)
{
	long long * p1, *p2;
	p1 = (long long *)a;  
	p2 = (long long *)b;  
	return *p1 > *p2 ? 1 : -1;
}

void GetName(std::vector<std::string> &vPath, std::vector<std::string> &vName)
{
	if (vPath.size() <= 0)
	{
		printf("vPath.size <= 0");
		return;
	}
	for (int i = 0; i < vPath.size(); i++)
	{
		std::string path = vPath[i];
		int pointDot = path.find_last_of('.');
		int pointN = path.find_last_of('\\');
		std::string name = path.substr(pointN + 1, pointDot - pointN - 1);
		vName.push_back(name);
	}
}

int dump_init_temp() {

#undef FUNC_CODE
#define FUNC_CODE 0x01

	//create dump folder
	if (_access(dumpRoot.c_str(), 00) != 0) {
		_mkdir(dumpRoot.c_str());
	}
	return 0;
}

 int LOGINIT(int num)
{
	 if (fp)
		 return 0;
	MaxLogNum = num;
	dump_init_temp();
	std::string logPath = dumpRoot + "/LOG_" + getCurrentSystemTime().substr(0,10);
	fp = fopen(logPath.c_str(), "a");
	if (fp == NULL)
	{
		isSafe = false;
		printf("%s", "failed to create file,init failed!");
		return -1;
	}
	isSafe = 1;
	return 0;
}
int LOGSetMaxSpace(int mSize)
{
	MaxLenChar = mSize;
	return 0;
}

 int logWrite(char * s)
{
	if (fp)
	{
		int len = ftell(fp);
		if (len > MaxLenChar)
		{
			LOGEnd();
			LOGINIT();
			return 0;
		}
		mux.lock();
		fprintf(fp, "%s", s);
		fflush(fp);
		mux.unlock();
	}
	return 0;
}

 int LOGEnd()
{
	if (fp)
	{
		fflush(fp);
		fclose(fp);
		fp = 0;
	}
	return 0;
}

 std::string getCurrentSystemTime()
{
	auto time_now = std::chrono::system_clock::now();
	auto tt = std::chrono::system_clock::to_time_t(time_now);
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	auto duration_in_s = std::chrono::duration_cast<std::chrono::seconds>(time_now.time_since_epoch());
	int theMs = duration_in_ms.count() - duration_in_s.count() * 1000;
	struct tm* ptm = localtime(&tt);

	char date[60] = { 0 };
	sprintf(date, "%d-%02d-%02d %02d:%02d:%02d:%03d%",
		(int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
		(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, theMs);
	return std::string(date);
}
long long getCurrentMs()
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}
 std::string GetName(const char * fileName)
{
	char temp[256] = { 0 };
	memcpy(temp, fileName, strlen(fileName) + 1);
	std::string sName = temp;
	int startP = sName.find_last_of("\\");
	if (startP == -1)
		startP = 0;
	if (sName[startP] == '\\') startP++;
	std::string theLastName = sName.substr(startP, sName.size() - startP);
	return theLastName;
}
