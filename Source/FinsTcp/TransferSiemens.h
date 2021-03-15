#ifndef __TRANSFER_SIEMENS_H__
#define __TRANSFER_SIEMENS_H__

#include "TransferHelper.h"
#include "TizerPLCComm.h"

enum SiemensDataType
{
	SiemensData_M	= 0,	// M型地址空间
	SiemensData_DB	= 1,	// DB型地址空间
	SiemensData_Invalid		// 无效类型
};

// modbus协议支持
class TransferSiemens : public TransferHelper
{
public:
	TransferSiemens(const char* ip, int port);
	~TransferSiemens();

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

	virtual bool	SetDWordData(int address, std::vector<int> vectValue);

	// 获取多个DWORD数据
	virtual int		GetMultiDWordData(int address, int n, std::vector<int>& data, char* pSerial = 0);

	void SetSiemensDataType(SiemensDataType nType) { m_nSiemensType = nType; }
	SiemensDataType GetSiemensDataType(){ return m_nSiemensType; }


private:
	void ConvertSiemensAddress_M(int& nAddress);
	void ConvertSiemensAddress_DB(int& nAddress);

private:
	CTizerPLCComm*		_Siemens;
	SiemensDataType		m_nSiemensType;
};

#endif // __TRANSFER_SIEMENS_H__