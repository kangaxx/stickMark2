#ifndef __TRANSFER_HELPER_H__
#define __TRANSFER_HELPER_H__

#include <vector>
#include <string>

// modbusЭ��֧��
class TransferHelper
{
public:
	TransferHelper(const char* ip, int port);
	~TransferHelper();

	// ��ʼ������ȡdll������ڣ����ӵ�PLC
	virtual int		Initialize();

	// ����PLC��Ϊ������Ӳ������ԵĹ��ܶ�������װ
	virtual int		Connect();

	bool SetIPAndPort(const char* ip, int port)
	{
		_ip		= ip;
		_port	= port;
		return true;
	}

	// �Ƿ���PLC����������
	virtual bool	IsConnected() { return _connected; }
	virtual void	Disconnect();
	virtual void	UnInitialize();
	virtual bool	SetDWordData(int address, int value);
	virtual bool	SetWordData(int address, short value);
	virtual bool	SetWordData2(int address, short value); // ʹ��addressʱ���ɲ����ٳ���2������SetWordData(27041, data._data)
	virtual bool	SetDoubleData(int address, const double& value) { return false; }
	virtual int		GetDWordData(int address, int& value);

	virtual bool	SetDWordData(int address, std::vector<int> vectValue);

	// ��ȡ���DWORD����
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