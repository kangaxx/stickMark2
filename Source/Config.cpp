#include "Config.h"

Config::Config() 
{
	upPlcCtrlIP = "192.168.1.9";
	downPlcCtrlIP = "192.168.1.10";
}
Config::~Config()
{
	Close();
}
bool  Config::Load(const char* file)
{
	return  false;

}

void Config::Close()
{
	
}

