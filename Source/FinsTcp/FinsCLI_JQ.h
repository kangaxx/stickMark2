
#ifndef CFINS_CLI_JQ
#define CFINS_CLI_JQ
#include <vector>
#include <string>
#ifdef FINSCLI_EXPORTS
#define FINSCLI_API __declspec(dllexport)
#else
#define FINSCLI_API __declspec(dllimport)
#endif
class  FINSCLI_API CFinsCLI_JQ {
public:
	CFinsCLI_JQ(const char* plcIPAddress, int plcPort);
	~CFinsCLI_JQ();
	
	int Connect();
	void Disconnect();
	// 初始化，获取dll函数入口，连接到PLC
	int		Initialize();
	void	UnInitialize();
	bool	IsConnected();

	bool ReadWordData(unsigned char eMemory, int offset, int readLength, short* data);
	bool ReadDWordData(unsigned char eMemory, int offset, int readLength, int* data);
	bool WriteWordData(unsigned char eMemory, int offset, int writeLength, const std::vector<short>& data);
	bool WriteDWordData(unsigned char eMemory, int offset, int writeLength, const std::vector<int>& data);
	bool ReadStringData(unsigned char eMemory, int offset, int readLength, short* data);

	bool	SetDWordData(int address, int value);
	bool	SetWordData(int address, short value);
	bool	SetDoubleData(int address, const double& value);
	int		GetDWordData(int address, int& value);
	int     GetWordData(int address, int& value);
	bool	SetDWordData(int address, std::vector<int> vectValue);
	int		GetMultiWordData(int address, int n, short* data);
	int		GetStringData(int address, int n, short* data);

	// 获取多个DWORD数据
	int		GetMultiDWordData(int address, int n, int* data);

private:
	void* _fins;
	bool			_connected;
	std::string		_ip;
	int				_port;
};

#endif
