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
	~WindowsServerSocket()
	{
		std::printf("WindowsServerSocket destructor called\n");
		shutdown(m_socket, SD_SEND);
		closesocket(m_socket);
		WSACleanup();
	}
	
	std::shared_ptr<ISocket> startConnectionListening();
	
	SOCKET getRawSocket()
	{
		return m_socket;
	}
	
private:
	SOCKET m_socket;
	WSADATA m_wsaData;

};

#endif