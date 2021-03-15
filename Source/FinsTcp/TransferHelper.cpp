#include "TransferHelper.h"
#include <Windows.h>

typedef int(*MBTCPCONNECT)(int nport, int IpPort, DWORD IpAddress);
typedef int(*MBTCPCLOSE)(int nport);
typedef int(*MBTCPFCN03)(int nport, int node, int address, int Count, int* RxdBuffer, int* RxdLength);
typedef int(*MBTCPFCN16)(int nport, int node, int address, int Count, WORD* TxdBuffer, int* RxdBuffer, int* RxdLength);

TransferHelper::TransferHelper(const char* ip, int port) : _hInstance(0), _fnClose(0), _fnRead(0), _fnWrite(0), _fnConnect(0), _connected(false),
_ip(ip), _port(port)
{
}

TransferHelper::~TransferHelper()
{
	UnInitialize();
}

int	TransferHelper::Initialize()
{
	HINSTANCE hInst = LoadLibraryA("HCTcp.dll");
	if (NULL == hInst)
	{
		//AfxMessageBox(_T("加载动态库HCTcp.dll 失败."));
		return -1;
	}
	_hInstance = hInst;

	_fnConnect = (MBTCPCONNECT)GetProcAddress(hInst, "mbtcpConnect");
	if (!_fnConnect)
	{
		//AfxMessageBox(_T("获取函数mbtcpConnect 失败."));
		return -2;
	}
	//MBTCPCLOSE m_fnClose;
	_fnClose = (MBTCPCLOSE)GetProcAddress(hInst, "mbtcpClose");
	if (!_fnClose)
	{
		//AfxMessageBox(_T("获取函数mbtcpClose 失败."));
		return -3;
	}
	//MBTCPFCN03 m_fnMbt03;
	_fnRead = (MBTCPFCN03)GetProcAddress(hInst, "mbtcpfcn03");
	if (!_fnRead)
	{
		//AfxMessageBox(_T("获取函数mbtcpfcn03 失败."));
		return -4;
	}

	_fnWrite = (MBTCPFCN16)GetProcAddress(hInst, "mbtcpfcn16");
	if (!_fnWrite)
	{
		//AfxMessageBox(_T("获取函数m_fnMbt16 失败."));
		return -5;
	}

	int res = Connect();
	if (res != 0)
		return res;

	return 0;
}
void TransferHelper::Disconnect()
{
	_connected = false;

}
int	TransferHelper::Connect()
{
	if (!_fnConnect)
		return -2;

	if (_connected)
		return 0;

	DWORD IpAddress = inet_addr(_ip.c_str());

	int nRes = ((MBTCPCONNECT)_fnConnect)(1, _port, IpAddress);

	if (!nRes)
	{
		//AfxMessageBox(_T("连接 失败."));
		_connected = false;
		return -6;
	}

	_connected = true;
	return 0;
}

void TransferHelper::UnInitialize()
{
	if (_fnClose)
		((MBTCPCLOSE)_fnClose)(1);
}

bool TransferHelper::SetDWordData(int address, int value)
{
	WORD TxdBuffer[20];
	BYTE RxdBuffer[20];
	TxdBuffer[0] = LOWORD(value);
	TxdBuffer[1] = HIWORD(value);
	int len = 0;
	int nReadCount = 2;
	int nRes = ((MBTCPFCN16)_fnWrite)(1, 255, address * 2, nReadCount, TxdBuffer, (int*)RxdBuffer, &len);
	if (nRes < 1)
		return false;

	return true;
}

bool TransferHelper::SetDWordData(int address, std::vector<int> vectValue)
{
	//WORD TxdBuffer[20];
	//BYTE RxdBuffer[20];
	//TxdBuffer[0] = LOWORD(value);
	//TxdBuffer[1] = HIWORD(value);
	//int len = 0;
	//int nReadCount = 2;
	//int nRes = ((MBTCPFCN16)_fnWrite)(1, 255, address * 2, nReadCount, TxdBuffer, (int*)RxdBuffer, &len);
	//if (nRes < 1)
	//	return false;

	return true;
}

bool TransferHelper::SetWordData(int address, short value)
{
	WORD TxdBuffer[20];
	BYTE RxdBuffer[20];
	TxdBuffer[0] = value;
	int len = 0;
	int nReadCount = 1;
	int nRes = ((MBTCPFCN16)_fnWrite)(1, 255, address * 2, nReadCount, TxdBuffer, (int*)RxdBuffer, &len);
	if (nRes < 1)
		return false;

	return true;
}

bool TransferHelper::SetWordData2(int address, short value)
{
	WORD TxdBuffer[20];
	BYTE RxdBuffer[20];
	TxdBuffer[0] = value;
	int len = 0;
	int nReadCount = 1;
	int nRes = ((MBTCPFCN16)_fnWrite)(1, 255, address, nReadCount, TxdBuffer, (int*)RxdBuffer, &len);
	if (nRes < 1)
		return false;

	return true;
}

int	TransferHelper::GetDWordData(int address, int& value)
{
	value = 0;

	BYTE RxdBuffer[20];
	int len = 0;
	int nReadCount = 2;
	int nRes = ((MBTCPFCN03)_fnRead)(1, 255, address * 2, nReadCount, (int*)RxdBuffer, &len);

	WORD* pWord = new WORD[nReadCount];
	int nDataLen = 2 * nReadCount;
	if (len > nDataLen)
	{
		for (int i = len - nDataLen; i < len; i += 2)
		{
			pWord[(i - (len - nDataLen)) / 2] = MAKEWORD(RxdBuffer[i + 1], RxdBuffer[i]);
		}
	}
	else
	{
		delete[]pWord;
		return -1;
	}

	//delete []RxdBuffer;
	DWORD data = *(DWORD*)pWord;
	delete[]pWord;
	//CString strData;
	//strData.Format(_T("从地址600内读取的数据为:%d."), data);
	//AfxMessageBox(strData);
	value = data;
	return 0;
}

int	TransferHelper::GetMultiDWordData(int address, int n, std::vector<int>& data,char* pSerial)
{
	data.clear();

	//BYTE RxdBuffer[20];
	BYTE* RxdBuffer = new BYTE[n * sizeof(int) + 20];
	int len = 0;
	int nReadCount = n * 2;
	int nRes = ((MBTCPFCN03)_fnRead)(1, 255, address * 2, nReadCount, (int*)RxdBuffer, &len);

	WORD* pWord = new WORD[nReadCount];
	int nDataLen = 2 * nReadCount;
	if (len > nDataLen)
	{
		for (int i = len - nDataLen; i < len; i += 2)
		{
			pWord[(i - (len - nDataLen)) / 2] = MAKEWORD(RxdBuffer[i + 1], RxdBuffer[i]);
		}
	}
	else
	{
		delete[] pWord;
		delete[] RxdBuffer;
		return -1;
	}

	for (int i = 1; i < nReadCount; i += 2)
	{
		int v = (((int)pWord[i]) << 16) & 0xffff0000 | ((int)pWord[i - 1]);
		data.push_back(v);
	}


	if (pSerial)
	{
		char strProdName[128] = { 0 };
		memcpy(strProdName, &RxdBuffer[0], len);
		strcpy(pSerial, strProdName);

		wchar_t strProdNameM[128];
		int nRtn = MultiByteToWideChar(CP_ACP, 0, strProdName, -1, strProdNameM, 128);
	}

	delete[] pWord;
	delete[] RxdBuffer;

	return 0;
}