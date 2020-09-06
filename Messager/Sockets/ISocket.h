#pragma once

#include <string>
#include <mutex>
#ifdef _WIN32
#include <winsock2.h>
#endif
class ISocket
{
private:

protected:
	std::uint16_t port;
	std::mutex socketIO;

public:
	virtual ~ISocket() 
	{
		std::printf("ISocket destructor called!\n");
	};

	virtual std::string readSocket()
	{
		std::printf("ISocket readSocket called!\n");
		return "";
	}

	virtual void sendSocket(std::string& s)\
	{
		std::printf("ISocket sendSocket called!\n");
	}

	virtual void setSocketNonBlocking()
	{
		std::printf("ISocket setSocketNonBlocking called!\n");
	}

#ifdef _WIN32
	virtual SOCKET getRawSocket()
	{
		std::printf("ISocket getRawSocket called!\n");
		SOCKET sock = INVALID_SOCKET;
		return sock;
	}
#else
#endif

};