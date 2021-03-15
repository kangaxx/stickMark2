#ifndef __STICK_SERVER_H__
#define __STICK_SERVER_H__

#include <vector>
#include "JuceHeader.h"


// ��������������������
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
	int code;//���ԭ��
	int iRoad;
	int iSel;
	double srcPos;
	double destPos;
	bool valid; //����ʱ  true--������� false---�������� srcPosΪ����
	double speed;
} CTLMARK_PG;


class HDataMessage :public juce::Message
{
public:
	CTLMARK_PG data;

};
// ��������ͻ��˶�Ӧ��port
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

// ������
class InterprocessStickServer : public juce::InterprocessConnectionServer
{
public:
	InterprocessStickServer();
	~InterprocessStickServer();


	void	SetComponent(juce::Component* component) { _component = component; }
	//����ֵ 0--�ɹ�, 1--ʧ��
	int	SendData(CTLMARK_PG& data);

protected:
	// �ɽ�������ͻ��˵�������������
	virtual juce::InterprocessConnection* createConnectionObject();


private:
	std::vector<juce::InterprocessConnection*>	_clientConnects;
	juce::Component* _component;
};

#endif // __STICK_SERVER_H__