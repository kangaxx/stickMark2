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
	MDT_1 = 1,		// 1 ©������©���棩
	MDT_2 = 2,		// 2 �Ӵ�
	MDT_3 = 3,		// 3 ��̼
	MDT_4 = 4,		// 4 ���ݣ���Ƭ�İ���
	MDT_5 = 5,		// 5 ����
	MDT_6 = 6,		// 6 ���ۣ����壩
	MDT_7 = 7,		// 7 ����
	MDT_8 = 8,		// 8 ��Ƭ�ڵ�
	MDT_9 = 9,		// 9 ����
	MDT_10 = 10,	// 10 ��Ƭ�ߴ粻��
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
	Marker_Defect_Type _nType;	// NG ����
	int	  _nDeltaIndex;			// �쳣��λ��������0��Delta1��1��Delta2����������7��Delta8
	float _fTop;				// ��λ������
	float _fLeft;				// ��λ������
	float _fBottom;				// ��λ������
	float _fRight;				// ��λ������
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
	int		_nCamerID;		// �������
	float	_fCoatWidth;	// Ϳ����ȣ���λ����
	float	_fCoatGap;		// �����϶����λ����
	int		_nTimeDelay;	// ���㴦���������ʱʱ������λ ����
	Time	_tTime;			// ʱ��
	float	_fBase;			// ����λ�ã�

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
	void*	m_pParent;			// ������Ϣת���ĸ�ָ��
	bool	m_bConnected;		// �Ƿ����ӵ�Server������
	String	m_strServerIP;		// ������IP��ַ
	int		m_nPort;			// �����������˿ں�
	bool	m_bMarkerEnable;	// ���빦���Ƿ���Ч��
	SendMode m_nThreadSend;		// ��Ϣ����ģʽ��ֱ�ӷ��͡������̷߳���

public:
	CriticalSection _cs;
public:
	// ���ö��з���������������Ϣ
	bool AddMarker(MarkerMessage markMsg);
	void ClearMarker();

	// ֱ�ӷ���������Ϣ
	void SendMarkerMsg(MarkerMessage markMsg);

	bool SendMarker(MarkerMessage markMsg);

public:
	//����IO�ź�
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