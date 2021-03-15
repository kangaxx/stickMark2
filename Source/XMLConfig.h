/*
  ==============================================================================

    XMLConfig.h
    Created: 1 Jun 2016 12:51:05pm
    Author:  zhouqi

  ==============================================================================
*/

#ifndef XMLCONFIG_H_INCLUDED
#define XMLCONFIG_H_INCLUDED

#include "JuceHeader.h"
#include <vector>
struct _CAMER_CONFIG
{
    double offsetX;
    double offsetY;
    int exposureTime; //us
    int gain;   
};

using namespace juce;

class CXMLConfig
{
	String _strMarkingIP;
	int _markingPort;
	int _ioUse[4];

	int _ioPorts[4];
	int _pulseLast;
	double _EALen;
	bool _enableW;
	bool _remainStick;
public:
	CXMLConfig();
	~CXMLConfig();
	void createDefaultConfig();
	bool  SavePara(int D0, int D1, int U0, int U1,int LicenseLimit,double eaLen);
	bool  LoadPara(int& D0, int& D1, int& U0, int& U1,int& LicenseLimit,double& eaLen);
	int   GetParam(int& licenseLimit);
	void  GetEnableWaring(int& enableW);
	int   GetMarkingServerAddr(String& strIP, int& port, int ioPort[4],bool& bSupportMC);
	int   GetMarkingIOPorts(int ioPorts[4], int& pulseLast);
	int   GetRemainStick(int& remainStick);
	int   GetPLCServerAddr(String& strIP);
	int   GetMarkerState(int*);
	int   SaveMarkerState(int*);
	int   GetCommuServerAddr(String& strIP);
	int   GetC4Mode(int& bC4MarkLocal);
	int   GetEdition(int& e);
	int   GetUIMode(int& iUIMode);
	int	  GetContiC5Stop(int& bC5MarkStop);

	std::unique_ptr<XmlElement>	rootData;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CXMLConfig)

};



#endif  // XMLCONFIG_H_INCLUDED
