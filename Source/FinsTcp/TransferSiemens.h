#ifndef __TRANSFER_SIEMENS_H__
#define __TRANSFER_SIEMENS_H__

#include "TransferHelper.h"
#include "TizerPLCComm.h"

enum SiemensDataType
{
	SiemensData_M	= 0,	// M�͵�ַ�ռ�
	SiemensData_DB	= 1,	// DB�͵�ַ�ռ�
	SiemensData_Invalid		// ��Ч����
};

// modbusЭ��֧��
class TransferSiemens : public TransferHelper
{
public:
	TransferSiemens(const char* ip, int port);
	~TransferSiemens();

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

	virtual bool	SetDWordData(int address, std::vector<int> vectValue);

	// ��ȡ���DWORD����
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