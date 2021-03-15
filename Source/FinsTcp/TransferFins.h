#ifndef __TRANSFER_FINS_H__
#define __TRANSFER_FINS_H__

#include "JuceHeader.h"
#include "TransferHelper.h"
#include "FinsCLI.h"
//#include "../../BaseClass/SysData.h"

// modbusЭ��֧��
class TransferFins : public TransferHelper
{
public:
	TransferFins(const char* ip, int port);
	~TransferFins();

	// ��ʼ������ȡdll������ڣ����ӵ�PLC
	virtual int		Initialize();

	// ����PLC��Ϊ������Ӳ������ԵĹ��ܶ�������װ
	virtual int		Connect();

	// �Ƿ���PLC����������
	virtual bool	IsConnected() { return _connected; }
	virtual void	UnInitialize();
	virtual bool	SetDWordData(int address, int value);
	virtual bool	SetWordData(int address, short value);
	virtual bool	SetDoubleData(int address, const double& value);
	virtual int		GetDWordData(int address, int& value);
	virtual int     GetWordData(int address, int& value);
	virtual bool	SetDWordData(int address, std::vector<int> vectValue);
	
	int				GetMultiWordData(int address, int n, std::vector<short>& data);

	// ��ȡ���DWORD����
	virtual int		GetMultiDWordData(int address, int n, std::vector<int>& data, char* pSerial = 0);

private:
	CFinsCLI*		_fins;
};

#endif // __TRANSFER_FINS_H__