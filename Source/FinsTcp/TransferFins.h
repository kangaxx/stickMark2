#ifndef __TRANSFER_FINS_H__
#define __TRANSFER_FINS_H__

#include "JuceHeader.h"
#include "TransferHelper.h"
#include "FinsCLI.h"
//#include "../../BaseClass/SysData.h"

// modbus协议支持
class TransferFins : public TransferHelper
{
public:
	TransferFins(const char* ip, int port);
	~TransferFins();

	// 初始化，获取dll函数入口，连接到PLC
	virtual int		Initialize();

	// 连接PLC。为添加连接不上重试的功能而单独封装
	virtual int		Connect();

	// 是否与PLC建立了连接
	virtual bool	IsConnected() { return _connected; }
	virtual void	UnInitialize();
	virtual bool	SetDWordData(int address, int value);
	virtual bool	SetWordData(int address, short value);
	virtual bool	SetDoubleData(int address, const double& value);
	virtual int		GetDWordData(int address, int& value);
	virtual int     GetWordData(int address, int& value);
	virtual bool	SetDWordData(int address, std::vector<int> vectValue);
	
	int				GetMultiWordData(int address, int n, std::vector<short>& data);

	// 获取多个DWORD数据
	virtual int		GetMultiDWordData(int address, int n, std::vector<int>& data, char* pSerial = 0);

private:
	CFinsCLI*		_fins;
};

#endif // __TRANSFER_FINS_H__