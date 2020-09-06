#pragma once

#ifdef _WIN32
#include "ISocket.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <WinUser.h>
#include <windows.h>
#pragma comment(lib,"WS2_32")


class WindowsClientSocket : virtual public ISocket
{
public:
	WindowsClientSocket(std::string&& hostname, std::string&& ip, std::string&& port);
	WindowsClientSocket(SOCKET sock) : m_socket(sock) 
	{
		std::printf("New WindowsClientSocket from another socket constructing\n");
	}
	~WindowsClientSocket()
	{
		std::printf("WindowsClientSocket destructor called\n");
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
		WSACleanup();
	}
	
	void sendSocket(std::string& s)
	{
		int error = 0;
		std::size_t length = s.length();
		const char len = '0' + length;

		{
			std::scoped_lock<std::mutex> lock(socketIO);

			send(m_socket, &len, sizeof(char), 0);
			if (error == SOCKET_ERROR)
			{
				closesocket(m_socket);
				WSACleanup();
				throw std::runtime_error("Send Failed");
			}

			send(m_socket, s.c_str(), (int)s.length(), 0);
			if (error == SOCKET_ERROR)
			{
				closesocket(m_socket);
				WSACleanup();
				throw std::runtime_error("Second Send Failed");
			}
		}
	}

	std::string readSocket()
	{
		char* tmpLen = nullptr;
		char* msg = nullptr;

		{
			std::scoped_lock<std::mutex> lock(socketIO);
			int readBytes = recv(m_socket, tmpLen, sizeof(char), 0);
			if (readBytes <= 0)
			{
				return "";
			}
			int size = *tmpLen - '0';
			//extract the size, create a buffer the appropriate size
			msg = new(std::nothrow) char[size + 1]();

			if (!msg)
			{
				throw std::runtime_error("ClientSocket::readSocket(): Failed to allocate memory for message\n");
			}
			msg[size] = '\0';
			readBytes = recv(m_socket, msg, size, 0);
			if (readBytes <= 0)
			{
				throw std::runtime_error("Connection closed");
			}
		}
		std::string message(msg);
		delete[] msg;

		return message;
	}

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
	WSADATA m_wsaData;
};
#endif