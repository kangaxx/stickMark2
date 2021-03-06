/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#include "InterConnection.h"

struct InterConnection::ConnectionThread  : public Thread
{
    ConnectionThread (InterConnection& c)  : Thread ("JUCE IPC"), owner (c) {}

    void run()      { owner.runThread(); }

private:
    InterConnection& owner;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectionThread)
};

//==============================================================================
InterConnection::InterConnection (const bool callbacksOnMessageThread,
                                                const uint32 magicMessageHeaderNumber)
    : callbackConnectionState (false),
      useMessageThread (callbacksOnMessageThread),
      magicMessageHeader (magicMessageHeaderNumber),
      pipeReceiveMessageTimeout (-1)
{
    thread = new ConnectionThread (*this);
}

InterConnection::~InterConnection()
{
    callbackConnectionState = false;
    disconnect();
    masterReference.clear();
    thread = nullptr;
}

//==============================================================================
bool InterConnection::connectToSocket (const String& hostName,
                                              const int portNumber,
                                              const int timeOutMillisecs)
{
    disconnect();

    const ScopedLock sl (pipeAndSocketLock);
    socket = new StreamingSocket();

    if (socket->connect (hostName, portNumber, timeOutMillisecs))
    {
        connectionMadeInt();
        thread->startThread();
        return true;
    }

    socket = nullptr;
    return false;
}

bool InterConnection::connectToPipe (const String& pipeName, const int timeoutMs)
{
    disconnect();

    ScopedPointer<NamedPipe> newPipe (new NamedPipe());

    if (newPipe->openExisting (pipeName))
    {
        const ScopedLock sl (pipeAndSocketLock);
        pipeReceiveMessageTimeout = timeoutMs;
        initialiseWithPipe (newPipe.release());
        return true;
    }

    return false;
}

bool InterConnection::createPipe (const String& pipeName, const int timeoutMs, bool mustNotExist)
{
    disconnect();

    ScopedPointer<NamedPipe> newPipe (new NamedPipe());

    if (newPipe->createNewPipe (pipeName, mustNotExist))
    {
        const ScopedLock sl (pipeAndSocketLock);
        pipeReceiveMessageTimeout = timeoutMs;
        initialiseWithPipe (newPipe.release());
        return true;
    }

    return false;
}

void InterConnection::disconnect()
{
    thread->signalThreadShouldExit();

    {
        const ScopedLock sl (pipeAndSocketLock);
        if (socket != nullptr)  socket->close();
        if (pipe != nullptr)    pipe->close();
    }

    thread->stopThread (4000);
    deletePipeAndSocket();
    connectionLostInt();
}

void InterConnection::deletePipeAndSocket()
{
    const ScopedLock sl (pipeAndSocketLock);
    socket = nullptr;
    pipe = nullptr;
}

bool InterConnection::isConnected() const
{
    const ScopedLock sl (pipeAndSocketLock);

    return ((socket != nullptr && socket->isConnected())
              || (pipe != nullptr && pipe->isOpen()))
            && thread->isThreadRunning();
}

String InterConnection::getConnectedHostName() const
{
    {
        const ScopedLock sl (pipeAndSocketLock);

        if (pipe == nullptr && socket == nullptr)
            return String();

        if (socket != nullptr && ! socket->isLocal())
            return socket->getHostName();
    }

    return IPAddress::local().toString();
}

//==============================================================================
bool InterConnection::sendMessage_Old (const MemoryBlock& message)
{
    uint32 messageHeader[2] = { ByteOrder::swapIfBigEndian (magicMessageHeader),
                                ByteOrder::swapIfBigEndian ((uint32) message.getSize()) };

    MemoryBlock messageData (sizeof (messageHeader) + message.getSize());
    messageData.copyFrom (messageHeader, 0, sizeof (messageHeader));
    messageData.copyFrom (message.getData(), sizeof (messageHeader), message.getSize());

    return writeData (messageData.getData(), (int) messageData.getSize()) == (int) messageData.getSize();
}

bool InterConnection::sendMessage (const MemoryBlock& message)
{
	return writeData ((void*)message.getData(), (int) message.getSize()) == (int) message.getSize();
}

int InterConnection::writeData (void* data, int dataSize)
{
    const ScopedLock sl (pipeAndSocketLock);

    if (socket != nullptr)
        return socket->write (data, dataSize);

    if (pipe != nullptr)
        return pipe->write (data, dataSize, pipeReceiveMessageTimeout);

    return 0;
}

//==============================================================================
void InterConnection::initialiseWithSocket (StreamingSocket* newSocket)
{
    jassert (socket == nullptr && pipe == nullptr);
    socket = newSocket;
    connectionMadeInt();
    thread->startThread();
}

void InterConnection::initialiseWithPipe (NamedPipe* newPipe)
{
    jassert (socket == nullptr && pipe == nullptr);
    pipe = newPipe;
    connectionMadeInt();
    thread->startThread();
}

//==============================================================================
struct ConnectionStateMessage  : public MessageManager::MessageBase
{
    ConnectionStateMessage (InterConnection* ipc, bool connected) noexcept
        : owner (ipc), connectionMade (connected)
    {}

    void messageCallback() 
    {
        if (InterConnection* const ipc = owner)
        {
            if (connectionMade)
                ipc->connectionMade();
            else
                ipc->connectionLost();
        }
    }

    WeakReference<InterConnection> owner;
    bool connectionMade;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectionStateMessage)
};

void InterConnection::connectionMadeInt()
{
    if (! callbackConnectionState)
    {
        callbackConnectionState = true;

        if (useMessageThread)
            (new ConnectionStateMessage (this, true))->post();
        else
            connectionMade();
    }
}

void InterConnection::connectionLostInt()
{
    if (callbackConnectionState)
    {
        callbackConnectionState = false;

        if (useMessageThread)
            (new ConnectionStateMessage (this, false))->post();
        else
            connectionLost();
    }
}

struct DataDeliveryMessage  : public Message
{
    DataDeliveryMessage (InterConnection* ipc, const MemoryBlock& d)
        : owner (ipc), data (d)
    {}

    void messageCallback() 
    {
        if (InterConnection* const ipc = owner)
            ipc->messageReceived (data);
    }

    WeakReference<InterConnection> owner;
    MemoryBlock data;
};

void InterConnection::deliverDataInt (const MemoryBlock& data)
{
    jassert (callbackConnectionState);

    if (useMessageThread)
        (new DataDeliveryMessage (this, data))->post();
    else
        messageReceived (data);
}

//==============================================================================
bool InterConnection::readNextMessageInt_Old()
{
    uint32 messageHeader[2];
    const int bytes = socket != nullptr ? socket->read (messageHeader, sizeof (messageHeader), true)
                                        : pipe  ->read (messageHeader, sizeof (messageHeader), -1);

    if (bytes == sizeof (messageHeader)
         && ByteOrder::swapIfBigEndian (messageHeader[0]) == magicMessageHeader)
    {
        int bytesInMessage = (int) ByteOrder::swapIfBigEndian (messageHeader[1]);

        if (bytesInMessage > 0)
        {
            MemoryBlock messageData ((size_t) bytesInMessage, true);
            int bytesRead = 0;

            while (bytesInMessage > 0)
            {
                if (thread->threadShouldExit())
                    return false;

                const int numThisTime = jmin (bytesInMessage, 65536);
                void* const data = addBytesToPointer (messageData.getData(), bytesRead);

                const int bytesIn = socket != nullptr ? socket->read (data, numThisTime, true)
                                                      : pipe  ->read (data, numThisTime, -1);

                if (bytesIn <= 0)
                    break;

                bytesRead += bytesIn;
                bytesInMessage -= bytesIn;
            }

            if (bytesRead >= 0)
                deliverDataInt (messageData);
        }
    }
    else if (bytes < 0)
    {
        if (socket != nullptr)
            deletePipeAndSocket();

        connectionLostInt();
        return false;
    }

    return true;
}

bool InterConnection::readNextMessageInt()
{
	int bytesInMessage = 65536;

	MemoryBlock messageData ((size_t) bytesInMessage, true);
	int bytesRead = 0;
	int bytesIn = 0;

	{
		if (thread->threadShouldExit())
			return false;

		void* const data = addBytesToPointer (messageData.getData(), bytesRead);

		bytesIn = socket != nullptr ? socket->read (data, bytesInMessage, false)
									: pipe  ->read (data, bytesInMessage, -1);

		bytesRead += bytesIn;
	}

	if (bytesRead > 0)
	{
		MemoryBlock messageDeliver ((size_t)bytesRead,true);
		messageDeliver.copyFrom (messageData.getData(), 0, messageData.getSize());
		deliverDataInt (messageDeliver);
	}

	if (bytesIn <= 0)
	{
		if (socket != nullptr)
			deletePipeAndSocket();

		connectionLostInt();
		return false;
	}

	return true;
}

void InterConnection::runThread()
{
    while (! thread->threadShouldExit())
    {
        if (socket != nullptr)
        {
            const int ready = socket->waitUntilReady (true, 5);

            if (ready < 0)
            {
                deletePipeAndSocket();
                connectionLostInt();
                break;
            }

            if (ready == 0)
            {
                thread->wait (1);
                continue;
            }
        }
        else if (pipe != nullptr)
        {
            if (! pipe->isOpen())
            {
                deletePipeAndSocket();
                connectionLostInt();
                break;
            }
        }
        else
        {
            break;
        }

        if (thread->threadShouldExit() || ! readNextMessageInt())
            break;
    }
}
