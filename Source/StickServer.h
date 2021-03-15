#ifndef __STICK_SERVER_H__
#define __STICK_SERVER_H__

#include <vector>
#include "JuceHeader.h"


// 用于向打标机发送输出项规格
typedef struct __TRANSFER_OUTPUT_LOGIC
{
	int		size;
	int		isCheck[20];
	int		isAlarm[20];
	int		isStop[20];
	int		isMark[20];
	int		num[20];
	int		id[20];
	int		flag[20][10];

}TRANSER_OUT_LOGIC;


typedef struct __CTLMARK_PG
{
	//from ip
	int EA;
	int code;//打标原因
	int iRoad;
	int iSel;
	double srcPos;
	double destPos;
	bool valid; //发送时  true--打标数据 false---步长计数 srcPos为步长
	double speed;
} CTLMARK_PG;


class HDataMessage :public juce::Message
{
public:
	CTLMARK_PG data;

};
// 服务器与客户端对应的port
class InterprocessStick : public juce::InterprocessConnection
{
public:
	InterprocessStick();
	virtual void connectionMade();
	virtual void connectionLost();
	virtual void messageReceived(const juce::MemoryBlock& message);

	void	SetComponent(juce::Component* component) { _component = component; }

private:
	juce::Component* _component;
};

// 服务器
class InterprocessStickServer : public juce::InterprocessConnectionServer
{
public:
	InterprocessStickServer();
	~InterprocessStickServer();


	void	SetComponent(juce::Component* component) { _component = component; }
	//返回值 0--成功, 1--失败
	int	SendData(CTLMARK_PG& data);

protected:
	// 可建立多个客户端到服务器的链接
	virtual juce::InterprocessConnection* createConnectionObject();


private:
	std::vector<juce::InterprocessConnection*>	_clientConnects;
	juce::Component* _component;
};

#endif // __STICK_SERVER_H__