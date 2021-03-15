#ifndef __WARNING_CLIENT_H__
#define __WARNING_CLIENT_H__

#include "JuceHeader.h"
// ��Ҫ��Communication����Ķ��屣��һ��
#include "CameraDataModel.h"
// ��Ҫ��Communication����Ķ��屣��һ��
typedef struct _warning_mes {
	int		ip;						// β��ip
	int		type;					// ���ͣ�1����or2ͣ��
	char	workStation[512];		// ��̨��Ϣ
	char	mes[512];				// ������Ϣ
	bool	isSend2Plc;				// �Ƿ�ɹ����͵�plc
	char	cTime[512];				// ��ǰʱ��
}WARNING_MES;
typedef struct __INTER_MESSAGE
{
	int		_spec;	// ��Ϣ���ض�����201404��������Ǵ˱��룬�������Ϣ
	int		_flag;	// ��Ϣ��־��< 0������/״̬��> 0�������������ֱ�ʾ��Ӧ�����ݻ�����
	// -1������������������꣬-2�������������������
	// -5��������-6��CCDϵͳ��ǰ״̬��-8�����ش���/�������豸��Ҫͣ��
	// -7����ǰ�豸�������꣬-9���豸״̬���
	// 1�������������㣬2��TizerAC��Communication��ѯ��ǰ�豸��������
	// -11����Ŀ���и�����������������λ�ã���MD5000
	// -12����-11��أ�����λ�ø����󣬸�����־λMD5003
	// -13: CCD���;�ƫ�ջ������Ϳ������Ȼ���༫��Ϳ���߶ȣ���ӦMD13534
	// -14: CCD���;�ƫ�ջ������Ϳ������Ȼ��Ҳ༫��Ϳ���߶ȣ���ӦMD13534
	// -15: CCD���;�ƫ�ջ�������Ѽ�⼫������������Ѽ�⼫����������ӦMD13536
	// -16: CCD���;�ƫ�ջ�������Ѽ�⼫���������Ҳ��Ѽ�⼫����������ӦMD13540
	// -17: CCD��ǰ��õ��ϴ�·�ܳ�����ӦMD13808
	// -18: CCD��ǰ��õ��´�·�ܳ�����ӦMD13809
	// -19: ����20���ֽڵ�Ĥ��ŵ�TizerAC
	// -20: TizerAC��Communication������INTER_MESSAGE_LASER_SIZE��һ������ģ������󣬻ָ�ʵ��������
	// -21: CCD������Ʒ����
	// -22: CCD���Ͳ���Ʒ����
	// -23: CCD��Ʒ/����Ʒ�������㡣Communication->TizerAC
	// -24: TizerAC���͵��㷨ģ��������������һ��ʱ��Communicationû���յ�����Ϣ�����ʾ�㷨ģ����ʧЧ
	// -30: ���ŷķ��Э��v5.0������ͣ����λ������CCD�������⵼��ͣ������CCD���ʹ����PLC�����ܿ�ʼ��������

		/*
	-190������λ�յ�Ĥ��Ž�����
	*/

	/*		DM20004������ַ,Tizer To Plc
	-40����ƫ��λ1 �����쳣
	-41���ϴ�· ������� �����쳣
	-42���ϴ�· 覴ü�� �����쳣
	-43���ϴ�· ��ƫ��λ �����쳣
	-44���ϴ�· ������� �����쳣
	-45���´�· ������� �����쳣
	-46���´�· 覴ü�� �����쳣
	-47���´�· ��ƫ��λ �����쳣
	-48���´�· ������� �����쳣
	*/

	/*		DM20008���ر�����ͣ����ַ,CCD���͵�plc�Թ���λ��ͣ��,Tizer To Plc
	-80����ƫ��λ1����ͣ��
	-81���ϴ�· ������� ����ͣ��
	-82���ϴ�· 覴ü�� ����ͣ��
	-83���ϴ�· ��ƫ��λ ����ͣ��
	-84���ϴ�· ������� ����ͣ��
	-85���´�· ������� ����ͣ��
	-86���´�· 覴ü�� ����ͣ��
	-87���´�· ��ƫ��λ ����ͣ��
	-88���´�· ������� ����ͣ��
	-89���ϴ�· �ڲ������ȱ�� ����ͣ�� ��·1
	-90���ϴ�· ��������ȱ�� ����ͣ�� ��·2
	-91���´�· �ڲ������ȱ�� ����ͣ�� ��·3
	-92���´�· ��������ȱ�� ����ͣ�� ��·4

	-99����ƫ����λͣ����
	*/

	/*		DM20116�豸״̬�ı��ַ,PLC To Tizer
	-60���豸ͣ��
	-61���豸��������
	-62���豸����


	-65�������ٶȵ�����������

	-66�����;�ƫ��ƫ������PLC
	*/

	/*		DM20012����λ�����źţ�	DM20016��ָ����λ����	PLC->Tizer��Tizer->PLC
	-120������CCD��λ������ɺ�д0��
	-121������CCD��λ��ʼ�������
	-160���ϴ�· ������� ����
	-161���ϴ�· 覴ü�� ����
	-162���ϴ�· ������� ����
	-163���´�· ������� ����
	-164���´�· 覴ü�� ����
	-165���´�· ������� ����
	*/

	/*
	-70���ͻ��˵�communication�������ź�,�ݶ�ÿ�뷢һ��

	-71������ƫ�ƣ��ջ��ã�dataΪ�����ţ�value[0]Ϊƫ��������Ϊ����
	-72��12��������ƫ��
	-73��34��������ƫ��

	-75����λ1��4���꿪ʼ��־������ɱ�־,�ͻ����յ�ָ��ִ�ж�������ɺ������������flag��ʾ��ɶ���
	-76����λ2��4���꿪ʼ��־������ɱ�־,�ͻ����յ�ָ��ִ�ж�������ɺ������������flag��ʾ��ɶ���
	-77����λ3��4���꿪ʼ��־������ɱ�־,�ͻ����յ�ָ��ִ�ж�������ɺ������������flag��ʾ��ɶ���
	-78����λ4��4���꿪ʼ��־������ɱ�־,�ͻ����յ�ָ��ִ�ж�������ɺ������������flag��ʾ��ɶ���
	*/

	int		_dir;	// ���䷽��0��TizerAC->Communication��1��Communication->TizerAC
	int		_data;	// ���ݻ��������
	int		_value[5];	// ������ֵ��
	// ��_flag = -5����ʱ����ʾ�ñ��������ڣ�
	// _value[0]: 0�������ִ�·��1����·1��2����·2��3����·3��...
	// _value[1]: 0�������������棬1�����棬2������
	// ��_flag = -21��-22ʱ��
	// _value[0]: 0�������ִ�·��1����·1��2����·2��3����·3��...
} INTER_MESSAGE;


// �����������ͨ��
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