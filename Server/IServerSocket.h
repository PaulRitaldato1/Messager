#pragma once
#include "../Messager/Sockets/ISocket.h"

class IServerSocket
{
public:
	virtual ~IServerSocket()
	{
		std::printf("IServerSocket destructor called!\n");
	}
	virtual std::shared_ptr<ISocket> startConnectionListening()
	{
		std::printf("IServerSocket startConnectionListening called!\n");
		return std::shared_ptr<ISocket>();
	}
#ifdef _WIN32
	virtual SOCKET getRawSocket()
	{
		std::printf("IServerSocket getRawSocket called!\n");
		SOCKET sock = INVALID_SOCKET;
		return sock;
	}
#else
#endif
};