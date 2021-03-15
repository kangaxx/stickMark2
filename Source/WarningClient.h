#ifndef __WARNING_CLIENT_H__
#define __WARNING_CLIENT_H__

#include "JuceHeader.h"
// 需要与Communication程序的定义保持一致
#include "CameraDataModel.h"
// 需要与Communication程序的定义保持一致
typedef struct _warning_mes {
	int		ip;						// 尾号ip
	int		type;					// 类型，1报警or2停机
	char	workStation[512];		// 机台信息
	char	mes[512];				// 具体信息
	bool	isSend2Plc;				// 是否成功发送到plc
	char	cTime[512];				// 当前时间
}WARNING_MES;
typedef struct __INTER_MESSAGE
{
	int		_spec;	// 消息的特定编码201404，如果不是此编码，则忽略消息
	int		_flag;	// 消息标志，< 0：数据/状态，> 0：命令。具体的数字表示相应的数据或命令
	// -1：左贴标机的贴标坐标，-2：右贴标机的贴标坐标
	// -5：报警，-6：CCD系统当前状态，-8：严重错误/报警，设备需要停机
	// -7：当前设备纵向坐标，-9：设备状态变更
	// 1：纵向坐标清零，2：TizerAC向Communication查询当前设备纵向坐标
	// -11：海目星切割分条机贴标测试坐标位置，到MD5000
	// -12：与-11相关，贴标位置给出后，给出标志位MD5003
	// -13: CCD发送纠偏闭环上相机涂覆区宽度或左侧极耳涂覆高度，对应MD13534
	// -14: CCD发送纠偏闭环下相机涂覆区宽度或右侧极耳涂覆高度，对应MD13534
	// -15: CCD发送纠偏闭环上相机已检测极耳数量或左侧已检测极耳数量，对应MD13536
	// -16: CCD发送纠偏闭环下相机已检测极耳数量或右侧已检测极耳数量，对应MD13540
	// -17: CCD当前测得的上带路总长，对应MD13808
	// -18: CCD当前测得的下带路总长，对应MD13809
	// -19: 发送20个字节的膜卷号到TizerAC
	// -20: TizerAC向Communication请求发送INTER_MESSAGE_LASER_SIZE。一般用于模拟结束后，恢复实机环境用
	// -21: CCD发送良品数据
	// -22: CCD发送不良品数据
	// -23: CCD良品/不良品计数清零。Communication->TizerAC
	// -24: TizerAC发送的算法模块存活的心跳，如果一段时间Communication没有收到此消息，则表示算法模块已失效
	// -30: 针对欧姆龙协议v5.0，加入停机复位命令。如果CCD发生问题导致停机，由CCD发送此命令到PLC，才能开始继续运行

		/*
	-190：各工位收到膜卷号解析包
	*/

	/*		DM20004报警地址,Tizer To Plc
	-40：纠偏工位1 运行异常
	-41：上带路 极耳检测 运行异常
	-42：上带路 瑕疵检测 运行异常
	-43：上带路 纠偏工位 运行异常
	-44：上带路 分条测宽 运行异常
	-45：下带路 极耳检测 运行异常
	-46：下带路 瑕疵检测 运行异常
	-47：下带路 纠偏工位 运行异常
	-48：下带路 分条测宽 运行异常
	*/

	/*		DM20008严重报警并停机地址,CCD发送到plc以供上位机停机,Tizer To Plc
	-80：纠偏工位1报警停机
	-81：上带路 极耳检测 报警停机
	-82：上带路 瑕疵检测 报警停机
	-83：上带路 纠偏工位 报警停机
	-84：上带路 分条测宽 报警停机
	-85：下带路 极耳检测 报警停机
	-86：下带路 瑕疵检测 报警停机
	-87：下带路 纠偏工位 报警停机
	-88：下带路 分条测宽 报警停机
	-89：上带路 内测贴标机缺标 报警停机 带路1
	-90：上带路 外侧贴标机缺标 报警停机 带路2
	-91：下带路 内测贴标机缺标 报警停机 带路3
	-92：下带路 外侧贴标机缺标 报警停机 带路4

	-99：纠偏器限位停机！
	*/

	/*		DM20116设备状态改变地址,PLC To Tizer
	-60：设备停机
	-61：设备正常运行
	-62：设备调试


	-65：发送速度到打标机服务器

	-66：发送纠偏器偏移量到PLC
	*/

	/*		DM20012各工位换卷信号，	DM20016，指定工位换卷	PLC->Tizer后Tizer->PLC
	-120：所有CCD工位换卷完成后写0，
	-121：所有CCD工位开始换卷操作
	-160：上带路 极耳检测 换卷
	-161：上带路 瑕疵检测 换卷
	-162：上带路 分条测宽 换卷
	-163：下带路 极耳检测 换卷
	-164：下带路 瑕疵检测 换卷
	-165：下带路 分条测宽 换卷
	*/

	/*
	-70：客户端到communication的心跳信号,暂定每秒发一次

	-71：激光偏移，闭环用，data为激光编号，value[0]为偏移量，负为向内
	-72：12激光整体偏移
	-73：34激光整体偏移

	-75：工位1的4连标开始标志或者完成标志,客户端收到指令执行动作，完成后向服务器发此flag表示完成动作
	-76：工位2的4连标开始标志或者完成标志,客户端收到指令执行动作，完成后向服务器发此flag表示完成动作
	-77：工位3的4连标开始标志或者完成标志,客户端收到指令执行动作，完成后向服务器发此flag表示完成动作
	-78：工位4的4连标开始标志或者完成标志,客户端收到指令执行动作，完成后向服务器发此flag表示完成动作
	*/

	int		_dir;	// 传输方向，0：TizerAC->Communication，1：Communication->TizerAC
	int		_data;	// 数据或命令参数
	int		_value[5];	// 附带的值。
	// 当_flag = -5报警时，表示该报警出现在：
	// _value[0]: 0：不区分带路，1：带路1，2：带路2，3：带路3，...
	// _value[1]: 0：不区分正反面，1：正面，2：反面
	// 当_flag = -21，-22时，
	// _value[0]: 0：不区分带路，1：带路1，2：带路2，3：带路3，...
} INTER_MESSAGE;


// 与打标服务器的通信
class WarningClient : public juce::InterprocessConnection
{
	double m_speed;
	
public:
	WarningClient();
	juce::int64 m_recTime;
	virtual void connectionMade();
	virtual void connectionLost();
	virtual void messageReceived(const juce::MemoryBlock& message);

	int		SendData(INTER_MESSAGE& data);

	void	SetComponent(juce::Component* mainComponent) { _mainComponent = mainComponent; }
	

	double GetMachineSpeed();
private:
	juce::Component*		_mainComponent;
};

class MainFormClient : public juce::InterprocessConnection
{

public:
	MainFormClient();

	virtual void connectionMade();
	virtual void connectionLost();
	virtual void messageReceived(const juce::MemoryBlock& message);

	int		SendData(StickMarkInfo& data);

	void	SetComponent(juce::Component* mainComponent) { _mainComponent = mainComponent; }
	int		SendMsgData(bool bNewUI, juce::String data);
	int		SendData(WS_INFO& header, WARNING_MSG& data);
	int     SendData(NumCount& numcount);

private:
	juce::Component* _mainComponent;
};
#endif // __STICK_CLIENT_H__