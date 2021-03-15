
#ifndef LABELDATAMANAGER_H
#define LABELDATAMANAGER_H

#include "JuceHeader.h"

#include <vector>
// This class is exported from the LabelerControl.dll
//������ݹ�����
typedef struct LabelData
{
	int iRoad;
	int iSel;
	double srcPos;
	double destPos;
	bool valid;

};

/*class ReadSignal :public juce::Timer, public juce::Thread
{
public:
	ReadSignal();
	~ReadSignal();
	void timerCallback() override;
	void run() override;
	void setmarksignal(bool);
	void setMarkId(int);
private:
	juce::CriticalSection _cs;
	bool marksignal;
	bool timeout;
	int MarkId;
};*/


//һ��������(������������)
class   CLabelerDataManager 
{

	int		_halfEA; //EA�ܳ�һ��
	juce::CriticalSection	_cs;		// �������ͬ������
	std::vector<LabelData> m_datas;
	
	double m_curLength[2]; //��ǰ���ۼƾ��Գ��� mm*10   0��ߴ���  1�ұߴ���
	int    m_ioPorts[4];
public:
	CLabelerDataManager(void);
	~CLabelerDataManager();
	
	//����һ���������
	//iRoad ��·�� 0---��  1---�� (Ŀǰֻ֧��һ����·,�������ӵ�IP��ַȷ��)
	//iSel: 0---��  1---��
	//srcPos: ��ǰȱ��λ��(��������10*mm) 
	//destPos:����Ŀ��λ��(�������� 10*mm)
	bool AddLabelData(int iRoad, int iSel, double srcPos, double destPos);

	void SetIOports(int ioPorts[4]);
	void SetEALength(int EANum);
	//��ǰģ�л�ǰ������,��λ: 10*mm
	//speed:  m/min
	void Step(int iSel, double speed, double stepImgDistMM);
	
	//��ǰģ�л��ܴ�����,��λ: 10*mm
	//iSel  0---��ߴ��� 1----�ұߴ���
	bool  RunningLength(int iRoad, int iSel, double speed, double runningLen, double& destPos);

	//�¾�����
	void NewRoll();
	//void SetReceiverFunc(void(Component*, String, const DataMessage&));


};

#endif