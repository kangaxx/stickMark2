#include "TransferFins.h"

//#define  _Performance_Debug_

#ifdef _Performance_Debug_
#include "..\..\BaseClass\StructureDef.h"
//extern _SYSTEM_CONFIG	g_configPara;
PerformanceCounter* g_pPC;
#endif // _Performance_Debug_

TransferFins::TransferFins(const char* ip, int port) : TransferHelper(ip, port)
{
#ifdef _Performance_Debug_
	g_pPC = new PerformanceCounter("SetWord Or GetWord", 100, File(L"D:\\CCD-PLC-Comm.txt"));
#endif // _Performance_Debug_

	_fins = new CFinsCLI();
}

TransferFins::~TransferFins()
{
	UnInitialize();
	delete _fins;
}

int	TransferFins::Initialize()
{
	int res = Connect();
	if (res != 0)
		return res;

	return 0;
}

int	TransferFins::Connect()
{
	if (_connected)
		return 0;

	bool ret = _fins->Connect(_ip.c_str(), _port);

	if (!ret)
	{
		_connected = false;
		return -6;
	}

	_connected = true;
	return 0;
}

void TransferFins::UnInitialize()
{
	if (_connected)
		_fins->DisConnect();
	_connected = false;
}



bool TransferFins::SetDWordData(int address, int value)
{
#ifdef _Performance_Debug_
	if (g_configPara.nPLCCommDebugEnable == 2)
		g_pPC->start();
#endif // _Performance_Debug_

	std::vector<int> data;
	data.push_back(value);
	bool ret = _fins->WriteDWordData(2, address, 1, data);

#ifdef _Performance_Debug_
	if (g_configPara.nPLCCommDebugEnable == 2)
		g_pPC->stop();
#endif // _Performance_Debug_

	return ret;
}

bool TransferFins::SetDWordData(int address, std::vector<int> vectValue)
{
	bool ret = _fins->WriteDWordData(2, address, vectValue.size(), vectValue);

	return ret;
}

bool TransferFins::SetDoubleData(int address, const double& value)
{
	std::vector<int> data;
	data.push_back(*((int*)&value));
	data.push_back(*((int*)&value + 1));
	bool ret = _fins->WriteDWordData(2, address, 2, data);

	return ret;
}

bool TransferFins::SetWordData(int address, short value)
{
	std::vector<short> data;
	data.push_back(value);
	bool ret = _fins->WriteWordData(2, address, 1, data);

	return ret;
}

int	TransferFins::GetDWordData(int address, int& value)
{
#ifdef _Performance_Debug_
	if (g_configPara.nPLCCommDebugEnable == 1)
		g_pPC->start();
#endif // _Performance_Debug_

	value = 0;
	std::vector<int> data;
	bool ret = _fins->ReadDWordData(2, address, 1, data);
	if (ret && data.size() >= 1)
	{
		value = data[0];
		return 0;
	}

#ifdef _Performance_Debug_
	if (g_configPara.nPLCCommDebugEnable == 1)
		g_pPC->stop();
#endif // _Performance_Debug_

	return -1;
}
int	TransferFins::GetMultiWordData(int address, int n, std::vector<short>& data)
{
	data.clear();
	bool ret = _fins->ReadWordData(2, address, n, data);
	if (ret && data.size() >= n)
	{
		return 0;
	}
	return -1;
}
int	TransferFins::GetMultiDWordData(int address, int n, std::vector<int>& data, char* pSerial)
{
	data.clear();
	bool ret = _fins->ReadDWordData(2, address, n, data);
	if (ret && data.size() >= n)
	{
		return 0;
	}

	return -1;
}

int TransferFins::GetWordData(int address, int& value)
{
	value = 0;
	std::vector<short> data;
	bool ret = _fins->ReadWordData(2, address, 1, data);
	if (ret && data.size() >= 1)
	{
		value = data[0];
		return 0;
	}

	return -1;
}
