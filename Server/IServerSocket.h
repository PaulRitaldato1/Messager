#pragma once
#include "../Messager/Sockets/WindowsClientSocket.h"

class IServerSocket : public ISocket
{
public:
	virtual std::shared_ptr<ISocket> startConnectionListening() = 0;
private:

};