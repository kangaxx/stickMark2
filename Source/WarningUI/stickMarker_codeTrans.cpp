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
	if (m_edition == 0) //�ȵ�
	{
		Pci1230WriteDoBit(15, 1, 1);
		juce::Thread::sleep(50);
		Pci1230WriteDoBit(15, 1, 0);
	}

	else if (m_edition == 1)//��Ŀ��
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
		//g_MainComponent->Label_MarkCount[i]->setText(L"����" + juce::String(i + 1) + L"����  " + String(g_MainComponent->Num_Mark[i]), NotificationType::dontSendNotification); // by gxx
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
		juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "", L"    ���Ӵ���PLCʧ��");
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
					m_linkState = transferfins->SetDWordData(4200, m_pulse);//��plcͨ������
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
					LOGWT("100������ƽ��ִ��ʱ��:%d ms", passTime);
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
					LOGWT("�������ӵ�PLC�ɹ�");
					Info << L"�������ӵ�PLC�ɹ�";
					Info << L"\n";
					//juce__textEditor->setText(Info, false);
					nSumFail = 0;
					bUpdate = true;
				}
				else
				{
					LOGWT("�������ӵ�PLCʧ��!");
					Info << L"�������ӵ�PLCʧ��";
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
						if (ret1 == 0 && resC4) //��·1���C4
						{
							LOGWT("��·%d���4����", i + 1);
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
					LOGWT("%s", "��ȡ1��������ʧ��");
				}

				markNum = sticked[1];
				if (ret == 0)
					bUpdate = true;
				else
				{
					nTryTime++;
					LOGWT("%s", "��ȡ2��������Ŀʧ��");
				}

				markNum = sticked[2];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					nTryTime++;
					LOGWT("%s", "��ȡ3��������ʧ��");
				}

				markNum = sticked[3];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					LOGWT("%s", "��ȡ4��������ʧ��");
					nTryTime++;
				}

				markNum = response[0];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					nTryTime++;
					LOGWT("%s", "��ȡ�����1�յ����ź���ʧ��");
				}

				markNum = response[1];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					nTryTime++;
					LOGWT("%s", "��ȡ�����2�յ����ź���ʧ��");
				}

				markNum = response[2];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					nTryTime++;
					LOGWT("%s", "��ȡ�����3�յ����ź���ʧ��");
				}

				markNum = response[3];
				if (ret == 0)
					bUpdate = true; 
				else
				{
					nTryTime++;
					LOGWT("%s", "��ȡ�����4�յ����ź���ʧ��");
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
					LOGWT("%s", "��ȡsensor1ʧ��");
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
					LOGWT("%s", "��ȡsensor2ʧ��");
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
					LOGWT("%s", "��ȡsensor3ʧ��");
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
					LOGWT("%s", "��ȡsensor4ʧ��");
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
				//��ȫ�Լ��,��������ź�2s��ʼ�����յ��ź���
				juce::int64 curTime = juce::Time::currentTimeMillis();
				//if (g_MainComponent != nullptr) //by gxx
				if (false) // by gxx
				{

					if (nTry > 0 && nTry <= 2)//ֻҪ����һ��ʧ�����¶�ȡ
					{
						LOGWT("��ȡ�����ʧ��!");
						bUpdate = true;
					}
					if (nTry > 2)
					{
						Info << juce::Time::getCurrentTime().formatted("%H:%M:%S") << ": ";
						Info << L"��ζ�ȡ�����ʧ�ܣ���Ҫ��������";
						LOGWT("��ζ�ȡ�����ʧ��,��Ҫ��������!!!");
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
							LOGWT("�������ӵ�PLC�ɹ�");
							Info << L"�������ӵ�PLC�ɹ�\n";
							//juce__textEditor->setText(Info, false);
							nSumFail = 0;
							bUpdate = true;
						}
						else
						{
							m_bIsConnected = false;
							LOGWT("�������ӵ�PLCʧ��");
							Info << L"�������ӵ�PLCʧ��\n";
							//juce__textEditor->setText(Info, false);
							bUpdate = true;
						}


						nTry = 0;
						nSumFail++;
						//if (nSumFail >= 1 && g_client)  // by gxx
						if (false) // by gxx
						{
							StopMachine(L"��ζ�ȡ�����ʧ��,��Ҫ��������!!!");
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
								//Info << i + 1 << L"�Ŵ������һ��(����:����:����)" << g_MainComponent->Num_Mark[i] << L":" << sticked[i] << L":" << sensorRes[i] << '\n'; //��⵽������Ϣ������ͣ��;  // by gxx
								//juce__textEditor->setText(Info, false);
								bUpdate = true;
								String reason = String(i + 1);
								reason << L"�Ŵ���ָ��������������һ��";
								m_bState[i] = 1;
								LOGWT("����sensor����%d", _allowLess);
								LOGWT("����sensor����%d", _allowMore);
								// LOGWT("%d�Ŵ�������ͣ��������ָ����%d,������%d �źŷ�����%d", i + 1, g_MainComponent->Num_Mark[i], sticked[i], sensorRes[i]); // by gxx

								// if (g_client) //by gxx
								if (false)
								{
									/*INTER_MESSAGE data;
									data._spec = 201404;
									data._flag = -92;
									g_client->SendData(data);*/
									StopMachine(reason);
								}
								// LOGWT("����ͣ��������ָ��λ��%f,��ǰλ��%f", _preStickLenPos[i], _curRunningLenPos[i]); // by gxx
								// LOGWT("%d�Ŵ�������ͣ��������ָ����%d,������%d �źŷ�����%d", i + 1, g_MainComponent->Num_Mark[i], sticked[i], sensorRes[i]); // by gxx

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
									Info << WarningInfo[i] << '\n'; //��⵽������Ϣ������ͣ��;
									bUpdate = true;
									//if (g_client) // by gxx
									if (false) // by gxx
									{
										LOGWT("����ͣ��WarningInfo[%d]", i);
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
								Info << L"��ζ�ȡ״̬������Ϣʧ��,ͣ������" << '\n'; //��⵽������Ϣ������ͣ��;
								//juce__textEditor->setText(Info, false);
								bUpdate = true;
								//if (g_client) // by gxx
								if (false)
								{
									LOGWT("��ζ�ȡ״̬������Ϣʧ��,ͣ������");
									StopMachine(L"��ζ�ȡ״̬������Ϣʧ��,ͣ������");
									nFailedRes = 0;
								}
							}
						}
					}
					else //����Ͽ�����
					{
						//if (g_client) //by gxx
						if (false)
						{
							LOGWT("��ζ�ȡ������Ϣʧ��,ͣ������");
							StopMachine(L"��ζ�ȡ������Ϣʧ��,ͣ������");
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
		LOGWT("�����������ݴ��������");
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
