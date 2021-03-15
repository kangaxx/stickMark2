#ifndef __GLOBAL_CONFIG_H__
#define __GLOBAL_CONFIG_H__

#include "JuceHeader.h"
#include <list>
using namespace juce;

class Config 
{
public:
	Config();
	~Config();
	// ������Ͽ�
	bool  Load(const char* file);
	String  upPlcCtrlIP;
	String  downPlcCtrlIP;

private:

	void Close();


};
#endif // __GLOBAL_CONFIG_H__