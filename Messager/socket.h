#pragma once
#include "common.h"

#ifdef _WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <WinUser.h>
#pragma comment(lib,"WS2_32")
#endif

class ClientSocket {
public:
	ClientSocket(std::string hostname, std::string ip);
	~ClientSocket();
	
	std::string read();
	void sendMsg(std::string s);

private:
	std::mutex socketIO;
	uint16_t port = 6969;
#ifdef _WIN32
	struct addrinfo* result = nullptr, * ptr = nullptr, hints;
	SOCKET _socket = INVALID_SOCKET;
#elif linux
	struct sockaddr_in addrress, serverAddress;
	int _socket;
#endif
	int resolveHostname(const char* hostname, std::string& ip);
	
};