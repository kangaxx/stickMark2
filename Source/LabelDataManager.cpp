// LabelerControl.cpp : Defines the exported functions for the DLL application.
//
#include "LabelDataManager.h"
#include"PCI1230.h"
#include"MainComponent.h"
#include "clog.h"

extern juce::int64 _preStickTime[4];
extern double _preStickLenPos[4];
extern double g_halEA;

int isMarkerOnUse[4] = { 1,1,1,1 };
//一个控制器(控制两个打标机)
CLabelerDataManager::CLabelerDataManager()
{
	m_curLength[0] = 0.;
	m_curLength[1] = 0.;
	m_datas.reserve(1000);
	_halfEA = 2000;
}


CLabelerDataManager::~CLabelerDataManager()
{

}
void CLabelerDataManager::SetEALength(int EANum)
{
	_halfEA = EANum*0.5;
	g_halEA = _halfEA;
}

//增加一个打标数据
//iRoad 带路： 0---上  1---下
//iSel: 0---左  1---右
//srcPos: 当前缺陷位置(绝对坐标10*mm) 
//destPos:打标机目标位置(绝对坐标 10*mm)
bool CLabelerDataManager::AddLabelData(int iRoad, int iSel, double srcPos, double destPos)
{
	_cs.enter();

	int i = m_datas.size() - 1;
	int iEnd = m_datas.size() - 10;
	for (;  i >= iEnd; i--)
	{
		if (i < 0)
			continue;
		
		//if (m_datas[i].valid == false)//已经出过标的也要比较，避免重复打标,需要用户新卷 ！切记
		//	continue;
		
		if (m_datas[i].iRoad == iRoad &&  m_datas[i].iSel == iSel &&  fabs(destPos - m_datas[i].destPos) < _halfEA)
		{

			_cs.exit();

			return false;
		}
	}

	LabelData data;
	data.iRoad = iRoad;
	data.iSel = iSel;
	data.srcPos = srcPos;
	data.destPos = destPos;
	data.valid = true;
	m_datas.push_back(data);

	_cs.exit();


	//发出打标命令到打标机缓冲区
	//LabelingCmd(iSel, destPos, 30.);
	return true;
}

//当前模切机前进距离,单位: 10*mm
void CLabelerDataManager::Step(int iSel, double speed, double stepImgDistMM)
{
	m_curLength[iSel] += stepImgDistMM;

	double dTol = 5; //5mm
	_cs.enter();
	for (int i = 0; i < m_datas.size(); i++)
	{
		if (m_datas[i].iSel != iSel)
			continue;

		//到达目标位置前一个步长 3000*0.06开始出标
		if(fabs (m_datas[i].destPos - stepImgDistMM) < dTol)
		{
			//TestLabel(iSel, speed);
			m_datas[i].valid = false;
		}
	}
	_cs.exit();
}

void CLabelerDataManager::SetIOports(int ioPorts[4])
{
	for (int i = 0; i < 4; i++)
		m_ioPorts[i] = ioPorts[i];

}

//当前模切机跑带长度,单位: 10*mm
bool CLabelerDataManager::RunningLength(int iRoad,int iSel, double speed, double stepImgDistMM, double& destPos)
{

	bool bDoStick = false;
	double dTol = 300; //300mm


	_cs.enter();
	m_curLength[iSel] = stepImgDistMM;
	//if (iSel == 1)
	//{
	//	m_countRecord++;
	//	if (m_countRecord > 50000)
	//	FILE* fp = fopen("RealTimePos.txt", "a");
	//	if (fp)
	//	{
	//		fprintf(fp, "receive coord: road=%d, iSel=%d, destPos=%f\n", iRoad,iSel,stepImgDistMM);
	//		fclose(fp);
	//	}
	//}

	for (int i = 0; i < m_datas.size(); i++)
	{

		//到达目标位置前一个步长 3000*0.06开始出标
		if (m_datas[i].iSel != iSel || iRoad != m_datas[i].iRoad)
		{
			continue;
		}

		//到达目标位置前一个步长 3000*0.06开始出标
		if (m_datas[i].iRoad == iRoad && iSel == m_datas[i].iSel && m_datas[i].valid == false)
		{
			continue;

		}
		if (m_datas[i].iRoad == iRoad && iSel == m_datas[i].iSel && fabs(m_datas[i].destPos - stepImgDistMM) < dTol)
		//if (m_curLength[iSel] >= (m_datas[i].destPos - stepImgDistMM - dTol))
		{
			int userRoad = 0;
			int ioPort = m_ioPorts[0];
			if (iRoad == 0 && iSel == 1)
			{
				userRoad = 1;
			}
			else if (iRoad == 1 && iSel == 1)
			{
				userRoad = 2;
			}
			else if (iRoad == 0 && iSel == 0)
			{
				userRoad = 3;
			}
			else if (iRoad == 1 && iSel == 0)
			{
				userRoad = 4;
			}
			ioPort = m_ioPorts[userRoad-1];
			m_datas[i].valid = false;

			//发出打标命令到打标机缓冲区
			//LabelingCmd(iSel, 1000, 60.);	//需要确认问题
		//	TestLabel(iSel, speed);
			if (isMarkerOnUse[userRoad - 1])
			{
				Pci1230WriteDoBit(15, ioPort, 1);
				juce::Thread::sleep(150);
				Pci1230WriteDoBit(15, ioPort, 0);
				bDoStick = true;
				destPos = m_datas[i].destPos;	
				_preStickTime[userRoad - 1] = Time::currentTimeMillis();
				_preStickLenPos[userRoad - 1] = stepImgDistMM;
				LOGWT("%d带路发送打标信号到IO %d", userRoad, ioPort);
			}
			break;
		}
	}

	_cs.exit();

	if (stepImgDistMM < 0.) //复位所有
	{
		NewRoll();
	}

	return bDoStick;

}
//新卷重置
void CLabelerDataManager::NewRoll()
{
	_cs.enter();

	m_curLength[0] = 0.;
	m_curLength[1] = 0.;

	m_datas.clear();
	m_datas.reserve(1000);
	//CLabelerControl::NewRoll();

	_cs.exit();


}

//ReadSignal::ReadSignal() :Thread("")
//{
//	marksignal = false;
//	timeout = false;
//	MarkId = 0;
//}
//
//ReadSignal::~ReadSignal()
//{
//
//}
//
//void ReadSignal::timerCallback()
//{
//	timeout = true;
//}
//
//void ReadSignal::run()
//{
//	while (!threadShouldExit())
//	{
//		bool ret = wait(-1);
//		_flcs.enter();
//		(*stream) << juce::String(L"线程" + juce::String(MarkId) + L"已被唤醒") + '\n';
//		(*stream).flush();
//		_flcs.exit();
//		if (ret)
//		{
//			unsigned int data = 0;
//			if (marksignal== true) 
//			{
//				startTimer(1500);
//				while (!data && !timeout)
//				{
//					Pci1230ReadDiBit(15, 2 * MarkId + 8, &data);
//				}
//				if (data)
//				{
//					stopTimer();
//					marksignal = false;
//					juce::String str = L"读取打标机"+juce::String(MarkId)+L"反馈信号成功"+'\n';
//					_flcs.enter();
//					stream->writeString(str);
//					stream->flush();
//					_flcs.exit();
//				}
//
//				else if (timeout)
//				{
//					stopTimer();
//					timeout = false;
//					marksignal = false;
//					juce::String str = L"读取打标机"+juce::String(MarkId)+L"反馈信号超时"+'\n';
//					_flcs.enter();
//					stream->writeString(str);
//					stream->flush();
//					_flcs.exit();
//					//Pci1230WriteDoBit(15, 1, 1);//报警停机
//				}
//			}
//
//		}
//	}
//}
//
//void ReadSignal::setmarksignal(bool isSent=false)
//{
//	juce::ScopedLock mylock(_cs);
//	marksignal = isSent;
//}
//
//void ReadSignal::setMarkId(int id)
//{
//	MarkId = id;
//}