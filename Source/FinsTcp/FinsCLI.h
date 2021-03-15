// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� FINSCLI_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// FINSCLI_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef FINSCLI_EXPORTS
#define FINSCLI_API __declspec(dllexport)
#else
#define FINSCLI_API __declspec(dllimport)
#endif

#include <vector>

// �����Ǵ� FinsCLI.dll ������
class FINSCLI_API CFinsCLI {
public:
	CFinsCLI(void);
	~CFinsCLI();
	
	bool Connect(const char* plcIPAddress, int plcPort);
	void DisConnect();
	bool ReadWordData(unsigned char eMemory, int offset, int readLength, std::vector<short>& data, int bOffset = 0, int dbNo = 0);
	bool ReadDWordData(unsigned char eMemory, int offset, int readLength, std::vector<int>& data, int bOffset = 0, int dbNo = 0);
	bool WriteWordData(unsigned char eMemory, int offset, int writeLength, const std::vector<short>& data, int bOffset = 0, int dbNo = 0);
	bool WriteDWordData(unsigned char eMemory, int offset, int writeLength, const std::vector<int>& data, int bOffset = 0, int dbNo = 0);

private:
	void*  _fins;
};

