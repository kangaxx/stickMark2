/*
  ==============================================================================

    XMLConfig.cpp
    Created: 1 Jun 2016 12:51:20pm
    Author:  zhouqi

  ==============================================================================
*/

#include "XMLConfig.h"

CXMLConfig::CXMLConfig()
{
    
    rootData = nullptr;
    
	_strMarkingIP = "192.168.1.200";
	_markingPort = 8000;
	_ioPorts[0] = 0;
	_ioPorts[1] = 2;
	_ioPorts[2] = 6;
	_ioPorts[3] = 4;

	_pulseLast = 50;
	_EALen = 4000.;
	_enableW = true;
	_remainStick = false;
}

CXMLConfig::~CXMLConfig()
{
    
    
    
    
}

void  CXMLConfig::createDefaultConfig()
{
	SavePara(100, 100, 100, 100,0,4000);

}
	


bool  CXMLConfig::SavePara(int D0, int D1, int U0, int U1,int LicenseLimit,double EASumLen)
{
    
    
    String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if(!sFile.existsAsFile())
	{
		ScopedPointer<OutputStream> outStream (sFile.createOutputStream().get());
		if (outStream != nullptr)
		{
			String sText("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
			outStream->writeText(sText,false,false,0);
			sText = "<ProductData></ProductData>";
			outStream->writeText(sText,false,false,0);
			outStream->flush();
		}
	}
	else
	{
		//sFile.deleteFile();
		//	
		//std::unique_ptr<OutputStream> outStream  = sFile.createOutputStream();
		//if (outStream != nullptr)
		//{
		//	String sText("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		//	outStream->writeText(sText,false,false,"\n");
		//	sText = "<ProductData></ProductData>";
		//	outStream->writeText(sText,false,false,0);
		//	outStream->flush();
		//}	
	}
	
	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();


	String strCount = "Product_Count";
	XmlElement* eleSecret = rootData->getChildByName (strCount);
	if (eleSecret==nullptr)
	{
		eleSecret = new XmlElement(strCount);
		rootData->addChildElement(eleSecret);

	}		
	eleSecret->setAttribute("D0", D0);
	eleSecret->setAttribute("D1", D1);
	eleSecret->setAttribute("U0", U0);
	eleSecret->setAttribute("U1", U1);

	eleSecret->setAttribute("LicenseLimit",LicenseLimit);
	eleSecret->setAttribute("EASumLen", EASumLen);

	eleSecret->setAttribute("road1", _ioPorts[0]);
	eleSecret->setAttribute("road2", _ioPorts[1]);
	eleSecret->setAttribute("road3", _ioPorts[2]);
	eleSecret->setAttribute("road4", _ioPorts[3]);
	eleSecret->setAttribute("pulseLast", _pulseLast);

	eleSecret->setAttribute("markingIP", _strMarkingIP);
	eleSecret->setAttribute("markingPort", String(_markingPort));
	eleSecret->setAttribute("EnableMA", String((int)_enableW));
	eleSecret->setAttribute("remainStick", String((int)_remainStick));

	
	if(rootData != nullptr)
	{
		bool bRet = rootData->writeToFile(sFile2,"","UTF-8",150);
		return true;
	}
	
	return false;
    
}

void  CXMLConfig::GetEnableWaring(int& enableW)
{

	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if (!sFile.existsAsFile())
	{
		createDefaultConfig();
		return ;
	}


	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();



	String strCount = "Product_Count";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return ;
	}

	if (eleSecret->hasAttribute("EnableMA"))
	{
		_enableW = enableW = eleSecret->getStringAttribute("EnableMA").getIntValue();

	}
}


//dpi---pixels per mm
bool  CXMLConfig::LoadPara(int& D0, int& D1, int& U0, int& U1,int& LicenseLimit,double& eaLen)
{
  
    String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if(!sFile.existsAsFile())
	{
		createDefaultConfig();	
		return false;
	}

	
	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();



  	String strCount = "Product_Count";
	XmlElement* eleSecret = rootData.get()->getChildByName (strCount);

	if (eleSecret==nullptr)
	{
		return false;
	}
		
	U0 = eleSecret->getStringAttribute("U0").getIntValue();
	U1 = eleSecret->getStringAttribute("U1").getIntValue();
	D0 = eleSecret->getStringAttribute("D0").getIntValue();
	D1 = eleSecret->getStringAttribute("D1").getIntValue();

	
	if(eleSecret->hasAttribute("LicenseLimit"))
	{

		LicenseLimit = eleSecret->getStringAttribute("LicenseLimit").getIntValue();
	}
	if (eleSecret->hasAttribute("EASumLen"))
	{
		_EALen = eaLen = eleSecret->getStringAttribute("EASumLen").getDoubleValue();
	}
	
	return true;
        
}
int   CXMLConfig::GetMarkingIOPorts(int ioPorts[4], int& pulseLast)
{
	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";
	File sFile(sPath);
	if (!sFile.existsAsFile())
	{
		return 0;
	}


	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();

	String strCount = "Product_Count";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}
	if (eleSecret->hasAttribute("road1"))
	{
		_ioPorts[0] = ioPorts[0] = eleSecret->getStringAttribute("road1").getIntValue();
	}
	if (eleSecret->hasAttribute("road2"))
	{
		_ioPorts[1] = ioPorts[1] = eleSecret->getStringAttribute("road2").getIntValue();
	}
	if (eleSecret->hasAttribute("road3"))
	{
		_ioPorts[2] = ioPorts[2] = eleSecret->getStringAttribute("road3").getIntValue();
	}
	if (eleSecret->hasAttribute("road4"))
	{
		_ioPorts[3] = ioPorts[3] = eleSecret->getStringAttribute("road4").getIntValue();
	}

	if (eleSecret->hasAttribute("pulseLast"))
	{
		_pulseLast= pulseLast = eleSecret->getStringAttribute("pulseLast").getIntValue();
	}	
	return 1;

}


int    CXMLConfig::GetPLCServerAddr(String& strIP)
{
	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if (!sFile.existsAsFile())
	{
		return 0;
	}


	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();

	String strCount = "Product_Count";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}
	if (eleSecret->hasAttribute("StickPLC"))
	{
		strIP = eleSecret->getStringAttribute("StickPLC");
		return 1;
	}
	return 0;

}

int    CXMLConfig::GetCommuServerAddr(String& strIP)
{
	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if (!sFile.existsAsFile())
	{
		return 0;
	}


	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();

	String strCount = "Product_Count";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}
	if (eleSecret->hasAttribute("commuIP"))
	{
		strIP  = eleSecret->getStringAttribute("commuIP");
		return 1;
	}	
	return 0;

}

int    CXMLConfig::GetMarkingServerAddr(String& strIP,int& port,int ioPort[4],bool& bSupportMC)
{
	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if (!sFile.existsAsFile())
	{		
		return 0;
	}


	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();

	String strCount = "MarkCode";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}
	if (eleSecret->hasAttribute("markingIP"))
	{
		_strMarkingIP = strIP = eleSecret->getStringAttribute("markingIP");
	}
	if (eleSecret->hasAttribute("markingPort"))
	{
		_markingPort = port = eleSecret->getStringAttribute("markingPort").getIntValue();
	}
	if (eleSecret->hasAttribute("road1"))
	{
		ioPort[0] = eleSecret->getStringAttribute("road1").getIntValue();
	}
	if (eleSecret->hasAttribute("road2"))
	{
		ioPort[1] = eleSecret->getStringAttribute("road2").getIntValue();
	}
	if (eleSecret->hasAttribute("road3"))
	{
		ioPort[2] = eleSecret->getStringAttribute("road3").getIntValue();
	}
	if (eleSecret->hasAttribute("road4"))
	{
		ioPort[3] = eleSecret->getStringAttribute("road4").getIntValue();
	}
	if (eleSecret->hasAttribute("supportMC"))
	{
		bSupportMC = eleSecret->getStringAttribute("supportMC").getIntValue();
	}	
	return 1;

}


int    CXMLConfig::GetUIMode(int& iUIMode)
{
	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if (!sFile.existsAsFile())
	{
		return 0;
	}


	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();

	String strCount = "UI";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}

	if (eleSecret->hasAttribute("style"))
	{
		iUIMode  = eleSecret->getStringAttribute("style").getIntValue();
		return 1;
	}
	return 0;

}
int    CXMLConfig::GetRemainStick(int& remainStick)
{
	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if (!sFile.existsAsFile())
	{
		return 0;
	}


	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();

	String strCount = "Product_Count";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}

	if (eleSecret->hasAttribute("remainStick"))
	{
		_remainStick = remainStick = eleSecret->getStringAttribute("remainStick").getIntValue();
	}
	return 1;

}

int    CXMLConfig::GetParam(int& licenseLimit)
{

   String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";
   
	File sFile(sPath);

	if(!sFile.existsAsFile())
	{
		createDefaultConfig();	
		return false;
	}

	
	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();



  	String strCount = "Product_Count";
	XmlElement* eleSecret = rootData->getChildByName (strCount);

	if (eleSecret==nullptr)
	{
		return 0;
	}
	if(eleSecret->hasAttribute("LicenseLimit"))
	{
		licenseLimit = eleSecret->getStringAttribute("LicenseLimit").getIntValue();
	}
	return 0;

}
int CXMLConfig::GetContiC5Stop(int& bC5MarkStop)
{
	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";
	File sFile(sPath);

	if (!sFile.existsAsFile())
	{
		return 0;
	}

	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();

	String strCount = "StickMarker";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}
	if (eleSecret->hasAttribute("c5Stop"))
	{
		bC5MarkStop = eleSecret->getStringAttribute("c5Stop").getIntValue();
	}
	return 0;
}

int CXMLConfig::GetC4Mode(int& bC4MarkLocal)
{
	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";
	File sFile(sPath);

	if (!sFile.existsAsFile())
	{
		return false;
	}

	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();
	   
	String strCount = "StickMarker";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}
	if (eleSecret->hasAttribute("localC4"))
	{
		bC4MarkLocal = eleSecret->getStringAttribute("localC4").getIntValue();
	}
	return 1;
}
int CXMLConfig::GetMarkerState(int* isOnUse)
{
	//默认为开启所有打标机
	for (int i = 0; i < 4; i++)
		_ioUse[i] = isOnUse[i] = 1;

	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if (!sFile.existsAsFile())
	{
		return false;
	}


	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();



	String strCount = "StickMarker";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}

	if (eleSecret->hasAttribute("M0"))
	{
		_ioUse[0] = isOnUse[0] = eleSecret->getStringAttribute("M0").getIntValue();
	}

	if (eleSecret->hasAttribute("M1"))
	{
		_ioUse[1] = isOnUse[1] = eleSecret->getStringAttribute("M1").getIntValue();
	}

	if (eleSecret->hasAttribute("M2"))
	{
		_ioUse[2] = isOnUse[2] = eleSecret->getStringAttribute("M2").getIntValue();
	}

	if (eleSecret->hasAttribute("M3"))
	{
		_ioUse[3] = isOnUse[3] = eleSecret->getStringAttribute("M3").getIntValue();
	}
	return 1;
}

int CXMLConfig::SaveMarkerState(int* isOnUse)
{
	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if (!sFile.existsAsFile())
	{
		return false;
	}


	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();



	String strCount = "StickMarker";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}

	for (int i = 0; i < 4; ++i)
	{
		eleSecret->setAttribute("M" + String(i), isOnUse[i]);
		rootData->writeTo(sFile2);
	}

	return 1;
}

int CXMLConfig::GetEdition(int& e)
{
	String sPath = File::getCurrentWorkingDirectory().getFullPathName() + "\\config.xml";


	File sFile(sPath);

	if (!sFile.existsAsFile())
	{
		return false;
	}


	File sFile2(sPath);
	XmlDocument dataDoc(sFile2);
	rootData = dataDoc.getDocumentElement();



	String strCount = "Edition";
	XmlElement* eleSecret = rootData->getChildByName(strCount);

	if (eleSecret == nullptr)
	{
		return 0;
	}
	
	else if (eleSecret->hasAttribute("E"))
	{
		e = eleSecret->getStringAttribute("E").getIntValue();
	}

	return 1;
}
