
#ifndef LABELDATAMANAGER_H
#define LABELDATAMANAGER_H

#include "JuceHeader.h"

#include <vector>
// This class is exported from the LabelerControl.dll
//打标数据管理器
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


//一个控制器(控制两个打标机)
class   CLabelerDataManager 
{

	int		_halfEA; //EA总长一半
	juce::CriticalSection	_cs;		// 命令队列同步保护
	std::vector<LabelData> m_datas;
	
	double m_curLength[2]; //当前卷累计绝对长度 mm*10   0左边打标机  1右边打标机
	int    m_ioPorts[4];
public:
	CLabelerDataManager(void);
	~CLabelerDataManager();
	
	//增加一个打标数据
	//iRoad 带路： 0---上  1---下 (目前只支持一个带路,根据连接的IP地址确定)
	//iSel: 0---左  1---右
	//srcPos: 当前缺陷位置(绝对坐标10*mm) 
	//destPos:打标机目标位置(绝对坐标 10*mm)
	bool AddLabelData(int iRoad, int iSel, double srcPos, double destPos);

	void SetIOports(int ioPorts[4]);
	void SetEALength(int EANum);
	//当前模切机前进距离,单位: 10*mm
	//speed:  m/min
	void Step(int iSel, double speed, double stepImgDistMM);
	
	//当前模切机跑带长度,单位: 10*mm
	//iSel  0---左边打标机 1----右边打标机
	bool  RunningLength(int iRoad, int iSel, double speed, double runningLen, double& destPos);

	//新卷重置
	void NewRoll();
	//void SetReceiverFunc(void(Component*, String, const DataMessage&));


};

#endif