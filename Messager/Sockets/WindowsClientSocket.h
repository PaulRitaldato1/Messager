#pragma once

#include "ISocket.h"

#ifdef _WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <WinUser.h>
#include <windows.h>
#pragma comment(lib,"WS2_32")


class WindowsClientSocket : public ISocket
{
public:
	WindowsClientSocket(std::string&& hostname, std::string&& ip, std::string&& port);
	WindowsClientSocket(SOCKET sock) : m_socket(sock) {}
	~WindowsClientSocket();
	
	void sendSocket(std::string& s);
	std::string readSocket();

	void setSocketNonBlocking()
	{
		unsigned long nonBlock = 1;
		ioctlsocket(m_socket, FIONBIO, &nonBlock);
	}

	SOCKET getRawSocket()
	{
		return m_socket;
	}

private:
	addrinfo* result = nullptr;
	SOCKET m_socket = INVALID_SOCKET;
};
#endif