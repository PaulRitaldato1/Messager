#pragma once

#ifdef linux
#include "IClientSocket.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <sys/time.h>
#include <netdb.h>
class UnixClientSocket : public ISocket
{
public:
	std::string readSocket();
	void sendSocket(std::string& s);
private:
	long resolveHostname(const char* hostname, std::string& ip);
};
#endif