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
	virtual ~ISocket();
	virtual std::string readSocket() = 0;
	virtual void sendSocket(std::string& s) = 0;

	virtual void setSocketNonBlocking() = 0;
#ifdef _WIN32
	virtual SOCKET getRawSocket() = 0;
#else
#endif

};