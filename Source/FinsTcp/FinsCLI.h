// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 FINSCLI_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// FINSCLI_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef FINSCLI_EXPORTS
#define FINSCLI_API __declspec(dllexport)
#else
#define FINSCLI_API __declspec(dllimport)
#endif

#include <vector>

// 此类是从 FinsCLI.dll 导出的
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

