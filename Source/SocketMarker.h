/*
  ==============================================================================

    SocketMarker.h
    Created: 16 Aug 2016 5:02:43pm
    Author:  fang.c.h

  ==============================================================================
*/

#ifndef SOCKETMARKER_H_INCLUDED
#define SOCKETMARKER_H_INCLUDED

#include "JuceHeader.h"
#include "InterConnection.h"
#include <vector>
#include <list>
using namespace std;

//#pragma pack(8)
//#pragma pack()

#define Marker_Message_Max 100

enum SendMode
{
	SendMode_Direct = 0,
	SendMode_Thread = 1
};

enum Marker_Defect_Type
{
	MDT_1 = 1,		// 1 漏金属（漏凹版）
	MDT_2 = 2,		// 2 接带
	MDT_3 = 3,		// 3 脱碳
	MDT_4 = 4,		// 4 气泡（极片鼓包）
	MDT_5 = 5,		// 5 划痕
	MDT_6 = 6,		// 6 条痕（打皱）
	MDT_7 = 7,		// 7 气泡
	MDT_8 = 8,		// 8 极片黑点
	MDT_9 = 9,		// 9 亮点
	MDT_10 = 10,	// 10 极片尺寸不良
	MDT_Invalid
};

class NGPosition
{
public:
	NGPosition(Marker_Defect_Type nType,float fTop = -1, float fLeft = -1, float fBottom = -1, float fRight = -1)
	{
		_nType		= nType;
		_fTop		= fTop;
		_fLeft		= fLeft;
		_fBottom	= fBottom;
		_fRight		= fRight;
		_nDeltaIndex = -1;
	}

	~NGPosition(){ ; }

public:
	Marker_Defect_Type _nType;	// NG 种类
	int	  _nDeltaIndex;			// 异常错位的索引，0：Delta1；1：Delta2；。。。。7：Delta8
	float _fTop;				// 单位：毫米
	float _fLeft;				// 单位：毫米
	float _fBottom;				// 单位：毫米
	float _fRight;				// 单位：毫米
};

class MarkerMessage
{
public:
	MarkerMessage(int nCamerID = -1, float fCoatWidth = -1, float fCoatGap = -1, int nTimeDelay = -1,float fBase = -1);
	~MarkerMessage();

public:
	void AddNGMark(NGPosition NGPos){ vectNGPos.push_back(NGPos); }
	int GetNGCount(){ return vectNGPos.size(); }
	String GetString();

public:
	int		_nCamerID;		// 相机索引
	float	_fCoatWidth;	// 涂布宽度：单位毫米
	float	_fCoatGap;		// 纵向间隙：单位毫米
	int		_nTimeDelay;	// 计算处理引起的延时时长：单位 毫秒
	Time	_tTime;			// 时间
	float	_fBase;			// 基材位置；

	vector<NGPosition> vectNGPos;
};

//////////////////////////////////////////////////////////////////////////
class MarkerThread : public Thread
{
public:
	MarkerThread(Component* pOwner);
	~MarkerThread();

	void run();

private:
	void exitThread();

private:
	Component*	m_pOwer;
	bool		m_bShouldExit;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MarkerThread);
};

class SocketMarker : public InterConnection,
					 public Timer,
					 public Component
{
public:
	SocketMarker(void* pParent,String strServer,int nPort);
	~SocketMarker();

public:
	void SetServerIP(String strServer, int nPort);
	bool IsMarkerEnable() { return m_bMarkerEnable; }
	void SetMarkerEnable(bool bEnable);

	void SetSendMode(SendMode nMode);


private:
	void timerCallback();

private:
    //==============================================================================
    /** Called when the connection is first connected.

        If the connection was created with the callbacksOnMessageThread flag set, then
        this will be called on the message thread; otherwise it will be called on a server
        thread.
    */
    virtual void connectionMade();

    /** Called when the connection is broken.

        If the connection was created with the callbacksOnMessageThread flag set, then
        this will be called on the message thread; otherwise it will be called on a server
        thread.
    */
    virtual void connectionLost();

    /** Called when a message arrives.

        When the object at the other end of this connection sends us a message with sendMessage(),
        this callback is used to deliver it to us.

        If the connection was created with the callbacksOnMessageThread flag set, then
        this will be called on the message thread; otherwise it will be called on a server
        thread.

        @see sendMessage
    */
    virtual void messageReceived (const MemoryBlock& message);


public:
	bool connectTo(String strServer,int nPort);
	bool connectTo();
	bool disconnect();
	bool isConnected();

	String getHostName()
	{
		return getSocket()->getHostName();
	}

	static int ICMPPing(const char* host);
	static bool PingIP(String strIP);
	static void WriteNetWorkLog(int nStatus);

protected:

public:
	void*	m_pParent;			// 保存信息转发的父指针
	bool	m_bConnected;		// 是否连接到Server服务器
	String	m_strServerIP;		// 服务器IP地址
	int		m_nPort;			// 服务器监听端口号
	bool	m_bMarkerEnable;	// 喷码功能是否有效；
	SendMode m_nThreadSend;		// 消息发送模式：直接发送、独立线程发送

public:
	CriticalSection _cs;
public:
	// 利用队列方案来发送喷码消息
	bool AddMarker(MarkerMessage markMsg);
	void ClearMarker();

	// 直接发送喷码消息
	void SendMarkerMsg(MarkerMessage markMsg);

	bool SendMarker(MarkerMessage markMsg);

public:
	//发送IO信号
	void SendIOSig(int ioNum);
	//
	bool SendMarkerMsg2(MarkerMessage markMsg);
	//
	void SendIOSigAndMarkerMsg(int ioNum, MarkerMessage markMsg, int pulseLast);

public:
	std::list<MarkerMessage> listMarker;

	MarkerThread* m_pMarkThread;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SocketMarker)
private:
	int m_pulseLast;
};
#endif  // SOCKETMARKER_H_INCLUDED