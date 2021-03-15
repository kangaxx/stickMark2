#include "StickServer.h"
#include"MainComponent.h"
#include "clog.h"
extern juce::FileLogger* g_log;
extern MainComponent* g_maincomponent;

InterprocessStickServer::InterprocessStickServer() : _component(0)
{

}

InterprocessStickServer::~InterprocessStickServer()
{
	for (int i = 0; i < _clientConnects.size(); i++)
		delete _clientConnects[i];
}

juce::InterprocessConnection* InterprocessStickServer::createConnectionObject()
{
	// 再次连接时，删除原先断开的连接
	std::vector<juce::InterprocessConnection*>::iterator it;
	for (it = _clientConnects.begin(); it != _clientConnects.end();)
	{
		if (!(*it)->isConnected())
		{
			delete* it;
			it = _clientConnects.erase(it);
		}
		else
			it++;
	}
	
	InterprocessStick* ic = new InterprocessStick;
	ic->SetComponent(_component);
	_clientConnects.push_back(ic);
	return ic;
}

InterprocessStick::InterprocessStick() : _component(0)
{

}

void InterprocessStick::connectionMade()
{
	juce::String ip = getConnectedHostName();
	LOGWT("%s连接上来了", ip.toStdString().c_str());
}

void InterprocessStick::connectionLost()
{

}

void InterprocessStick::messageReceived(const juce::MemoryBlock& message)
{
	juce::Thread::setCurrentThreadPriority(10);
	if (message.getSize() < sizeof(CTLMARK_PG))
		return;

	else if (message.getSize() == sizeof(TRANSER_OUT_LOGIC))//收到打标规格表数据
	{
		TRANSER_OUT_LOGIC msg;
		memcpy(&msg, message.getData(), sizeof(TRANSER_OUT_LOGIC));

		//((PLCComponent*)_component)->SetLabelStd(msg);
		g_maincomponent->SetLabelStd(msg);
		return;
	}

	CTLMARK_PG msg;
	memcpy(&msg, message.getData(), sizeof(CTLMARK_PG));

	HDataMessage hMsg;
	hMsg.data = msg;
	/*((PLCComponent*)_component)->HandleClientMessage(getConnectedHostName(), &hMsg);*/
	//g_maincomponent->HandleClientMessage(getConnectedHostName(), &hMsg);
	g_maincomponent->HandleClientMessage(getConnectedHostName(),&hMsg);
	



}



int	InterprocessStickServer::SendData(CTLMARK_PG& data)
{
	juce::MemoryBlock msg(&data, sizeof(CTLMARK_PG));
	int ret = 0;
	if (_clientConnects.size() == 0)
		ret = 1;
	std::vector<juce::InterprocessConnection*>::iterator it;
	for (it = _clientConnects.begin(); it != _clientConnects.end(); it++)
	{
		if ((*it)->isConnected())
		{
			if (!((*it)->sendMessage(msg)))
				ret = 1;
		}
		else
		{
			ret = 1;
		}
	}
	return ret;
}