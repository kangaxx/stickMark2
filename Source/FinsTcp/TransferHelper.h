#ifndef __TRANSFER_HELPER_H__
#define __TRANSFER_HELPER_H__

#include <vector>
#include <string>

// modbus协议支持
class TransferHelper
{
public:
	TransferHelper(const char* ip, int port);
	~TransferHelper();

	// 初始化，获取dll函数入口，连接到PLC
	virtual int		Initialize();

	// 连接PLC。为添加连接不上重试的功能而单独封装
	virtual int		Connect();

	bool SetIPAndPort(const char* ip, int port)
	{
		_ip		= ip;
		_port	= port;
		return true;
	}

	// 是否与PLC建立了连接
	virtual bool	IsConnected() { return _connected; }
	virtual void	Disconnect();
	virtual void	UnInitialize();
	virtual bool	SetDWordData(int address, int value);
	virtual bool	SetWordData(int address, short value);
	virtual bool	SetWordData2(int address, short value); // 使用address时，可不用再除以2，例如SetWordData(27041, data._data)
	virtual bool	SetDoubleData(int address, const double& value) { return false; }
	virtual int		GetDWordData(int address, int& value);

	virtual bool	SetDWordData(int address, std::vector<int> vectValue);

	// 获取多个DWORD数据
	virtual int		GetMultiDWordData(int address, int n, std::vector<int>& data, char* pSerial = 0);

private:
	void*			_hInstance;
	void*			_fnClose;
	void*			_fnRead;
	void*			_fnWrite;
	void*			_fnConnect;

protected:
	bool			_connected;

	std::string		_ip;
	int				_port;
};
#endif // __TRANSFER_HELPER_H__