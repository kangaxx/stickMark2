#include "TransferSiemens.h"
#include "..\DataInterface.h"

extern int g_nProtocolBaseAddress;			// 与PLC之间通信的基地址,add by fchh 2020.04.18

TransferSiemens::TransferSiemens(const char* ip, int port) : TransferHelper(ip, port)
{
	m_nSiemensType = SiemensData_DB;

	_Siemens = new CTizerPLCComm;
}

TransferSiemens::~TransferSiemens()
{
	UnInitialize();
}

int	TransferSiemens::Initialize()
{
	int res = Connect();
	if (res != 0)
		return res;

	return 0;
}

int	TransferSiemens::Connect()
{
	if (_connected)
		return 0;

	bool ret = _Siemens->Connect(_ip.c_str(), _port);

	if (!ret)
	{
		_connected = false;
		return -6;
	}

	_connected = true;
	return 0;
}

void TransferSiemens::UnInitialize()
{
	if (_connected)
		_Siemens->DisConnect();
	_connected = false;
}

void TransferSiemens::ConvertSiemensAddress_M(int& nAddress)
{
	// 西门子PLC的内存访问是以字节为单位的，欧姆龙的Fins是以Word为单位

	// g_nProtocolBaseAddress = 1000;	// 西门子PLC通信基地址设定 1000 开始。
	nAddress = (nAddress - g_nProtocolBaseAddress) / 2 * 4 + g_nProtocolBaseAddress;
}

void TransferSiemens::ConvertSiemensAddress_DB(int& nAddress)
{
	// 西门子PLC的内存访问是以字节为单位的，欧姆龙的Fins是以Word为单位

	// g_nProtocolBaseAddress = 1000;	// 西门子PLC通信基地址设定 1000 开始。
	nAddress = (nAddress - g_nProtocolBaseAddress) / 2 * 4;
}

bool TransferSiemens::SetDWordData(int address, int value)
{
	int nAddressTmp = address - Address_Rectify_Coat_Count;

	char szAddress[30];

	if (m_nSiemensType == SiemensData_M)
	{
		ConvertSiemensAddress_M(nAddressTmp);
		sprintf_s(szAddress, "M%d", nAddressTmp);
	}
	else if (m_nSiemensType == SiemensData_DB)
	{
		ConvertSiemensAddress_DB(nAddressTmp);
		sprintf_s(szAddress, "DB%d.%04d", g_nProtocolBaseAddress, nAddressTmp);
	}

	bool ret = _Siemens->WriteIntData(szAddress, value);

	return ret;
}

bool TransferSiemens::SetDWordData(int address, std::vector<int> vectValue)
{
	bool ret = false;

	int nAddressTmp = address - Address_Rectify_Coat_Count;

	char szAddress[30];

	if (m_nSiemensType == SiemensData_M)
	{
		ConvertSiemensAddress_M(nAddressTmp);

		for (int i = 0; i < vectValue.size(); i++)
		{
			sprintf_s(szAddress, "M%d", nAddressTmp + i * 4);

			ret = _Siemens->WriteIntData(szAddress, vectValue[i]);

			if (!ret)
				break;
		}
	}
	else if (m_nSiemensType == SiemensData_DB)
	{
		ConvertSiemensAddress_DB(nAddressTmp);

		for (int i = 0; i < vectValue.size(); i++)
		{
			sprintf_s(szAddress, "DB%d.%04d", g_nProtocolBaseAddress, nAddressTmp + i * 2);

			ret = _Siemens->WriteIntData(szAddress, vectValue[i]);

			if (!ret)
				break;
		}

	}

	return ret;
}

bool TransferSiemens::SetDoubleData(int address, const double& value)
{
	bool ret = false;

	int nAddressTmp = address - Address_Rectify_Coat_Count;

	char szAddress[30];

	if (m_nSiemensType == SiemensData_M)
	{
		ConvertSiemensAddress_M(nAddressTmp);

		std::vector<int> data;
		data.push_back(*((int*)&value));
		data.push_back(*((int*)&value + 1));

		for (int i = 0; i < data.size(); i++)
		{
			sprintf_s(szAddress, "M%d", nAddressTmp + i * 4);
			ret = _Siemens->WriteIntData(szAddress, data[i]);

			if (!ret)
				break;
		}
	}
	else if (m_nSiemensType == SiemensData_DB)
	{
		ConvertSiemensAddress_DB(nAddressTmp);

		std::vector<int> data;
		data.push_back(*((int*)&value));
		data.push_back(*((int*)&value + 1));

		for (int i = 0; i < data.size(); i++)
		{
			sprintf_s(szAddress, "DB%d.%04d", g_nProtocolBaseAddress, nAddressTmp + i * 2);
			ret = _Siemens->WriteIntData(szAddress, data[i]);

			if (!ret)
				break;
		}
	}

	return ret;
}

bool TransferSiemens::SetWordData(int address, short value)
{
	bool ret = false;

	//std::vector<short> data;
	//data.push_back(value);
	//ret = _Siemens->WriteWordData(2, address, 1, data);

	return ret;
}

int	TransferSiemens::GetDWordData(int address, int& value)
{
	bool bRet = 0;

	int nAddressTmp = address - Address_Rectify_Coat_Count;

	char szAddress[30];

	if (m_nSiemensType == SiemensData_M)
	{
		ConvertSiemensAddress_M(nAddressTmp);
		sprintf_s(szAddress, "M%d", nAddressTmp);

		bRet = _Siemens->ReadIntData(szAddress, value);
	}
	else if (m_nSiemensType == SiemensData_DB)
	{
		ConvertSiemensAddress_DB(nAddressTmp);
		sprintf_s(szAddress, "DB%d.%04d", g_nProtocolBaseAddress, nAddressTmp);

		bRet = _Siemens->ReadIntData(szAddress, value);
	}

	if (bRet)
		return 0;

	else
		return -1;
}

int	TransferSiemens::GetMultiDWordData(int address, int n, std::vector<int>& data, char* pSerial)
{
	bool bRet = false;

	data.clear();

	int nAddressTmp = address - Address_Rectify_Coat_Count;

	char szAddress[30];

	if (m_nSiemensType == SiemensData_M)
	{
		ConvertSiemensAddress_M(nAddressTmp);

		for (int i = 0; i < n; i++)
		{
			int nValue = 0;
			sprintf_s(szAddress, "M%d", nAddressTmp + i * 4);

			bRet = _Siemens->ReadIntData(szAddress, nValue);

			if (!bRet)
				break;
			else
				data.push_back(nValue);
		}
	}
	else if (m_nSiemensType == SiemensData_DB)
	{
		ConvertSiemensAddress_DB(nAddressTmp);

		for (int i = 0; i < n; i++)
		{
			int nValue = 0;
			sprintf_s(szAddress, "DB%d.%04d", g_nProtocolBaseAddress, nAddressTmp + i * 2);

			bRet = _Siemens->ReadIntData(szAddress, nValue);

			if (!bRet)
				break;
			else
				data.push_back(nValue);
		}
	}

	if (bRet)
		return 0;
	else
		return -1;
}