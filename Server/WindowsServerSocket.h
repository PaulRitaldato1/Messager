#pragma once

#include "IServerSocket.h"
#ifdef _WIN32
#include "../Messager/Sockets/WindowsClientSocket.h"
#include <string>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment (lib, "Ws2_32.lib")

class WindowsServerSocket  : public IServerSocket
{
public:
	WindowsServerSocket();
	~WindowsServerSocket();
	std::string readSocket() { return ""; }
	
	void sendSocket(std::string& s) {}
	
	std::shared_ptr<ISocket> startConnectionListening();
	
	SOCKET getRawSocket()
	{
		return m_socket;
	}
	
	void setSocketNonBlocking()
	{
		unsigned long nonBlock = 1;
		ioctlsocket(m_socket, FIONBIO, &nonBlock);
	}
private:
	SOCKET m_socket;
};

#endif