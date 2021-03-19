#include "stickMarker_codeTrans.h"
#include"PCI1230.h"
#include".\WarningClient.h"
#include".\clog.h"
#include".\MainComponent.h"
extern int isMarkerOnUse[4];
extern MainComponent* g_MainComponent;
extern WarningClient* g_client;
void StickMarker_codeTrans::StopMachine(const juce::String & info)
{
	if (m_edition == 0) //先导
	{
		Pci1230WriteDoBit(15, 1, 1);
		juce::Thread::sleep(50);
		Pci1230WriteDoBit(15, 1, 0);
	}

	else if (m_edition == 1)//海目星
	{
		// if (g_client) // by gxx
		if (true)
		{
			INTER_MESSAGE data;
			data._spec = 201404;
			data._flag = -92;
			//g_client->SendData(data); // by gxx
		}
	}
	
	//if (g_MainComponent) // by gxx
	if (true) // by gxx
	{
		static juce::int64 preReportTime = 0;
		juce::int64 curTime = juce::Time::currentTimeMillis();
		static String preInfo = L"";
		if (preInfo == info && curTime - preReportTime > 15000)
		{
			//g_MainComponent->m_mfClient.SendMsgData(g_MainComponent->m_bNewUI, info); // by gxx
			preReportTime = curTime;
		}
		else if (preInfo != info)
		{
			// g_MainComponent->m_mfClient.SendMsgData(g_MainComponent->m_bNewUI, info); // by gxx
			preReportTime = curTime;
		}
		preInfo = info;

	}
}

void StickMarker_codeTrans::handleAsyncUpdate()
{
	juce__textEditor->setText(Info, false);

	for (int i = 0; i < 4; ++i)
	{
		//g_MainComponent->Label_MarkCount[i]->setText(L"打标机" + juce::String(i + 1) + L"计数  " + String(g_MainComponent->Num_Mark[i]), NotificationType::dontSendNotification); // by gxx
	}
	/* by gxx
	juce__label35->setText(juce::String(sticked[0]), juce::NotificationType::dontSendNotification);
	juce__label38->setText(juce::String(sticked[1]), juce::NotificationType::dontSendNotification);
	juce__label41->setText(juce::String(sticked[2]), juce::NotificationType::dontSendNotification);
	juce__label44->setText(juce::String(sticked[3]), juce::NotificationType::dontSendNotification);

	juce__label36->setText(juce::String(response[0]), juce::NotificationType::dontSendNotification);
	juce__label39->setText(juce::String(response[1]), juce::NotificationType::dontSendNotification);
	juce__label42->setText(juce::String(response[2]), juce::NotificationType::dontSendNotification);
	juce__label45->setText(juce::String(response[3]), juce::NotificationType::dontSendNotification);


	juce__label50->setText(juce::String(sensorRes[0]), juce::NotificationType::dontSendNotification);
	juce__label51->setText(juce::String(sensorRes[1]), juce::NotificationType::dontSendNotification);
	juce__label52->setText(juce::String(sensorRes[2]), juce::NotificationType::dontSendNotification);
	juce__label53->setText(juce::String(sensorRes[3]), juce::NotificationType::dontSendNotification);
	
	if (m_prebState != m_bState)
		repaint();
	m_prebState = m_bState;
	*/
}

StickMarker_codeTrans::StickMarker_codeTrans(juce::String plcIP):juce::Thread("")
{
	this->_strPLC = plcIP;
}

StickMarker_codeTrans::~StickMarker_codeTrans()
{

}

bool StickMarker_codeTrans::SendCmd(int addr, int val)
{
	_cs.enter();
	COMMAND_WRITE_CT data;
	data.flag = val;
	data.regAddr = addr;
	_cmds.push_back(data);
	_cs.exit();
	return true;
}

void StickMarker_codeTrans::Reconnect()
{
	m_bReconnect = true;
}

void StickMarker_codeTrans::run()
{
#ifdef OMRON_COAT_LIB
	TransferFins* transferfins = new TransferFins(_strPLC.toStdString().c_str(), 9600);
#else
	CFinsCLI_JQ *transferfins = new CFinsCLI_JQ(_strPLC.toStdString().c_str(), 9600);
#endif
	transferfins->Initialize();
	if (!transferfins->IsConnected())
	{
		m_bIsConnected = false;
		juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "", L"    连接打标机PLC失败");
	}
	else
	{
		m_bIsConnected = true;
	}
	int markNum = 0;
	int markNum2 = 0;
	int address = 4300;
	int isWarn = 0;
	int rets[4] = { 1,1,1,1 };
	int nTry = 0;
	int nSumFail = 0;
	bool bCanRead = false;
	int nReadTime = 0;
	juce::int64 passTime = 0;
	int iRead = 0;
	int iWrite = 0;
	int failWrite = 0;
	int nFailedRes = 0;
	while (!threadShouldExit())
	{
		bool bUpdate = false;
		int nTryTime = 0;
		for (int i = 0; i < 4; ++i)
		{
			m_bState[i + 28] = isMarkerOnUse[i];
		}
		if (m_bReconnect)
		{
			transferfins->UnInitialize();
			wait(3000);
			transferfins->Connect();
			m_bReconnect = false;

			if (transferfins->IsConnected())
				m_bIsConnected = true;
		}

		if (transferfins)
		{
			if (transferfins->IsConnected())
			{
				if (iWrite % 2 == 0)
				{
					iWrite = 0;
					m_pulse = !m_pulse;
					juce::int64 curTime = Time::currentTimeMillis();
					m_linkState = transferfins->SetDWordData(4200, m_pulse);//与plc通信心跳
					if (nReadTime < 101)
					{
						passTime += (Time::currentTimeMillis() - curTime);
						nReadTime++;
					}
					if (!m_linkState)
					{
						failWrite++;
					}
					else
					{
						failWrite = 0;
					}
					if (failWrite > 4)
					{
						transferfins->Disconnect();
						transferfins->Connect();
						failWrite = 0;
					}
				}
				iWrite++;

				if (nReadTime == 100)
				{
					LOGWT("100次网络平均执行时间:%d ms", passTime);
				}


				_cs.enter();
				while (_cmds.size())
				{
					COMMAND_WRITE_CT data = _cmds[0];
					transferfins->SetDWordData(data.regAddr, data.flag);
					_cmds.erase(_cmds.begin());
				}
				_cs.exit();
			}
			else
			{
				m_bIsConnected = false;
				wait(2000);
				transferfins->Connect();
				if (transferfins->IsConnected())
				{
					m_bIsConnected = true;
					LOGWT("重新连接到PLC成功");
					Info << L"重新连接到PLC成功";
					Info << L"\n";
					//juce__textEditor->setText(Info, false);
					nSumFail = 0;
					bUpdate = true;
				}
				else
				{
					LOGWT("重新连接到PLC失败!");
					Info << L"重新连接到PLC失败";
					Info << L"\n";
					//juce__textEditor->setText(Info, false);
					bUpdate = true;
				}
			}

			if (iRead >= 10)
			{
				iRead = 0;


				if (!m_bLocalC4)
				{
					juce::int64 curTime = juce::Time::currentTimeMillis();
					for (int i = 0; i < 4; i++)
					{
						if (!isMarkerOnUse[i] || !m_bC4Mark[i])
							continue;
						/* gxx
						juce::int64 passTime = curTime - _preStickTime[i];
						int resC4 = 0;
						int ret1 = getDword(transferfins, 4450, resC4);
						if (ret1 == 0 && resC4) //带路1完成C4
						{
							LOGWT("带路%d完成4连标", i + 1);
							m_bC4Mark[i] = false;
						}
						*/
					}
				}

				for (int k = 0; k < 4; k++)
				{
					sticked[k] = response[k] = -1;
					rets[k] = 0;
				}
				//int ret = getStickResData(transferfins, sticked, rets, response); // by gxx
				int ret = -1; //by gxx

				markNum = sticked[0];

				if (ret == 0)
				{
					bUpdate = true;
				}
				else
				{
					nTryTime++;
					LOGWT("%s", "读取1号贴标数失败");
				}

				markNum = sticked[1];
				if (ret == 0)
					bUpdate = true;
				else
				{
					nTryTime++;
					LOGWT("%s", "读取2号贴表数目失败");
				}

				markNum = sticked[2];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					nTryTime++;
					LOGWT("%s", "读取3号贴标数失败");
				}

				markNum = sticked[3];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					LOGWT("%s", "读取4号贴标数失败");
					nTryTime++;
				}

				markNum = response[0];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					nTryTime++;
					LOGWT("%s", "读取贴标机1收到的信号数失败");
				}

				markNum = response[1];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					nTryTime++;
					LOGWT("%s", "读取贴标机2收到的信号数失败");
				}

				markNum = response[2];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					nTryTime++;
					LOGWT("%s", "读取贴标机3收到的信号数失败");
				}

				markNum = response[3];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					nTryTime++;
					LOGWT("%s", "读取贴标机4收到的信号数失败");
				}

				for (int k = 0; k < 4; k++)
					sensorRes[k] = -1;

				//int ret2 = getSensorData(transferfins, sensorRes); // by gxx
				int ret2 = -1; //by gxx
				int sensorNum = sensorRes[0];
				if (ret2 == 0)
				{
					bUpdate = true; 
				}
				else
				{
					LOGWT("%s", "读取sensor1失败");
					nTryTime++;
				}

				sensorNum = sensorRes[1];
				//ret = getDword(612, sensorNum);
				if (ret2 == 0)
				{
					bUpdate = true; //juce__label51->setText(juce::String(sensorNum), juce::NotificationType::dontSendNotification);

				}
				else
				{
					LOGWT("%s", "读取sensor2失败");
					nTryTime++;
				}

				sensorNum = sensorRes[2];
				//ret = getDword(614, sensorNum);
				if (ret2 == 0)
				{
					bUpdate = true; //juce__label52->setText(juce::String(sensorNum), juce::NotificationType::dontSendNotification);

				}
				else
				{
					LOGWT("%s", "读取sensor3失败");
					nTryTime++;
				}

				sensorNum = sensorRes[3];
				//ret = getDword(616, sensorNum);
				if (ret2 == 0)
				{
					bUpdate = true; //juce__label53->setText(juce::String(sensorNum), juce::NotificationType::dontSendNotification);

				}
				else
				{
					LOGWT("%s", "读取sensor4失败");
					nTryTime++;
				}
				if (nTryTime)
					nTry++;
				else
					nTry = 0;

				if (ret == 0 && ret2 == 0)
				{
					// getStickNumCount(transferfins, sticked, response, sensorRes); // by gxx
				}
				//安全性检查,发出打标信号2s后开始检查接收的信号数
				juce::int64 curTime = juce::Time::currentTimeMillis();
				//if (g_MainComponent != nullptr) //by gxx
				if (false) // by gxx
				{

					if (nTry > 0 && nTry <= 2)//只要发生一次失败重新读取
					{
						LOGWT("读取打标数失败!");
						bUpdate = true;
					}
					if (nTry > 2)
					{
						Info << juce::Time::getCurrentTime().formatted("%H:%M:%S") << ": ";
						Info << L"多次读取打标数失败，需要重新连接";
						LOGWT("多次读取打标数失败,需要重新连接!!!");
						bUpdate = true;
						//g_MainComponent->_finsLock.enter(); //by gxx
						m_bIsConnected = false;
						transferfins->UnInitialize();
						wait(2000);
						transferfins->Connect();
						//g_MainComponent->_finsLock.exit(); //by gxx

						if (transferfins->IsConnected())
						{
							m_bIsConnected = true;
							LOGWT("重新连接到PLC成功");
							Info << L"重新连接到PLC成功\n";
							//juce__textEditor->setText(Info, false);
							nSumFail = 0;
							bUpdate = true;
						}
						else
						{
							m_bIsConnected = false;
							LOGWT("重新连接到PLC失败");
							Info << L"重新连接到PLC失败\n";
							//juce__textEditor->setText(Info, false);
							bUpdate = true;
						}


						nTry = 0;
						nSumFail++;
						//if (nSumFail >= 1 && g_client)  // by gxx
						if (false) // by gxx
						{
							StopMachine(L"多次读取打标数失败,需要重新连接!!!");
							nSumFail = 0;
						}
						continue;
					}
					for (int i = 0; i < 4; i++)
					{
						if (!isMarkerOnUse[i])
							continue;
						int minElap = 3000;
						int maxElap = 6000;
						if (m_bLocalC4 && m_bC4Mark[i])
						{
							minElap = 5000;
							maxElap = 8000;
						}

						//juce::int64 passTime = curTime - _preStickTime[i]; // by gxx
						juce::int64 passTime = 0; // by gxx
						//if ((_preStickTime[i] > 0 && passTime > minElap && passTime <= maxElap)) // by gxx
						if (false) // by gxx
						{
							if (m_bC4Mark[i])
								m_bC4Mark[i] = false;

							bool bErr = false;
							if (_allowLess > 0)
							{
								//if ((sensorRes[i] + _allowLess) < g_MainComponent->Num_Mark[i]) // by gxx
								if (false)
								{
									bErr = true;
								}
							}
							else if (sensorRes[i] > sticked[i])
							{
								//if ((sensorRes[i] - _allowMore) < g_MainComponent->Num_Mark[i]) // by gxx
								if (false)
								{
									bErr = true;
								}
							}
							else if (_allowLess == 0 || _allowMore == 0)
							{
								//if (sensorRes[i] < g_MainComponent->Num_Mark[i]) // by gxx
								if (false)
								{
									bErr = true;
								}
							}

							//_preStickLenPos[i] = -1.;
							//if (sticked[i] < g_MainComponent->Num_Mark[i] || bErr) // by gxx
							if (false)
							{
								Info << juce::Time::getCurrentTime().formatted("%H:%M:%S") << ": ";
								//Info << i + 1 << L"号打标数不一致(发出:接收:反馈)" << g_MainComponent->Num_Mark[i] << L":" << sticked[i] << L":" << sensorRes[i] << '\n'; //检测到报警信息，报警停机;  // by gxx
								//juce__textEditor->setText(Info, false);
								bUpdate = true;
								String reason = String(i + 1);
								reason << L"号打标机指令数与贴标数不一致";
								m_bState[i] = 1;
								LOGWT("允许sensor数少%d", _allowLess);
								LOGWT("允许sensor数多%d", _allowMore);
								// LOGWT("%d号打标机报警停机：发出指令数%d,贴标数%d 信号反馈数%d", i + 1, g_MainComponent->Num_Mark[i], sticked[i], sensorRes[i]); // by gxx

								// if (g_client) //by gxx
								if (false)
								{
									/*INTER_MESSAGE data;
									data._spec = 201404;
									data._flag = -92;
									g_client->SendData(data);*/
									StopMachine(reason);
								}
								// LOGWT("报警停机：发出指令位置%f,当前位置%f", _preStickLenPos[i], _curRunningLenPos[i]); // by gxx
								// LOGWT("%d号打标机报警停机：发出指令数%d,贴标数%d 信号反馈数%d", i + 1, g_MainComponent->Num_Mark[i], sticked[i], sensorRes[i]); // by gxx

							}
						}
					}
				}
				//
				std::vector<int> responses(27);
				if (transferfins)
				{
					if (transferfins->IsConnected())
					{

#ifdef OMRON_COAT_LIB
						std::vector<int> responses2;
#else
						int responses2[54] = { 0 };
#endif
						int ret2 = transferfins->GetMultiDWordData(4300, 27 * 2, responses2);

						if (ret2 == 0)
						{
							nFailedRes = 0;
							for (int i = 0; i < 27; ++i)
							{
								responses[i] = responses2[2 * i];
								m_bState[i] = responses[i];
							}
							for (int i = 4; i < 27; ++i)//for (int i = 4; i < 27; ++i)
							{
								if (i <= 7 && !isMarkerOnUse[i - 4])
									continue;
								
								if (responses[i] == 1)
								{
									Info << juce::Time::getCurrentTime().formatted("%H:%M:%S") << ": ";
									Info << WarningInfo[i] << '\n'; //检测到报警信息，报警停机;
									bUpdate = true;
									//if (g_client) // by gxx
									if (false) // by gxx
									{
										LOGWT("报警停机WarningInfo[%d]", i);
										StopMachine(WarningInfo[i]);
										break;
									}
								}
							}
						}
						else
						{
							nFailedRes++;
							if (nFailedRes >= 2)
							{
								Info << juce::Time::getCurrentTime().formatted("%H:%M:%S") << ": ";
								Info << L"多次读取状态反馈信息失败,停机处理" << '\n'; //检测到报警信息，报警停机;
								//juce__textEditor->setText(Info, false);
								bUpdate = true;
								//if (g_client) // by gxx
								if (false)
								{
									LOGWT("多次读取状态反馈信息失败,停机处理");
									StopMachine(L"多次读取状态反馈信息失败,停机处理");
									nFailedRes = 0;
								}
							}
						}
					}
					else //网络断开连接
					{
						//if (g_client) //by gxx
						if (false)
						{
							LOGWT("多次读取反馈信息失败,停机处理");
							StopMachine(L"多次读取反馈信息失败,停机处理");
						}
					}
				}
			}
			iRead++;

		}
		wait(100);

		if (bUpdate)
		{
			//triggerAsyncUpdate(); by gxx
		}
	}

	//if (g_client) // by gxx
	if(true) // by gxx
	{
		LOGWT("打标服务器数据处理结束！");
	}

	transferfins->UnInitialize();
	delete transferfins;
}

void StickMarker_codeTrans::timerCallback()
{

}

void StickMarker_codeTrans::ClearLessMore()
{
}
