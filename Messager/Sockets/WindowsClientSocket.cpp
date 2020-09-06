#include "WindowsClientSocket.h"

#ifdef _WIN32

WindowsClientSocket::WindowsClientSocket(std::string&& hostname, std::string&& ip, std::string&& port) 
{
	std::printf("Fresh WindowsClientSocket constructing\n");
	int resultHandler;
	resultHandler = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (resultHandler != 0) {
		printf("WSAStartup failed with error: %d\n", resultHandler);
	}

	//create socket
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	addrinfo* ptr = nullptr;

	
	resultHandler = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &ptr);
	
	if (resultHandler != 0)
	{
		char error[50];
		sprintf_s(error, 50, "Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		throw std::runtime_error(error);
	}

	for (addrinfo* addr = ptr; addr != nullptr; addr = addr->ai_next)
	{
		m_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
		if (m_socket == INVALID_SOCKET)
		{
			char error[50];
			sprintf_s(error, 50, "Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(ptr);
			WSACleanup();
			throw std::runtime_error(error);
		}

		if (connect(m_socket, addr->ai_addr, addr->ai_addrlen) == 0)
			break;
		m_socket = INVALID_SOCKET;
		closesocket(m_socket);
	}

	if (m_socket == INVALID_SOCKET) 
	{
		char error[50];
		sprintf_s(error, 50, "Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(ptr);
		WSACleanup();
		throw std::runtime_error(error);
	}

	//TODO, IF CONNECT FAILS, CHECK NEXT ADDRESS IN RESULT

	freeaddrinfo(ptr);
	if (m_socket == INVALID_SOCKET) 
	{
		WSACleanup();
		throw std::runtime_error("Could not connect to server");
	}

}

//std::string WindowsClientSocket::readSocket() 
//{
//	
//	
//
//}

//void WindowsClientSocket::sendSocket(std::string& s)
//{
//
//	int error = 0;
//	std::size_t length = s.length();
//	char* len = nullptr;
//	*len = '0' + length;
//	
//	{
//		std::scoped_lock<std::mutex> lock(socketIO);
//
//		send(m_socket, len, sizeof(char), 0);
//		if (error == SOCKET_ERROR)
//		{
//			closesocket(m_socket);
//			WSACleanup();
//			throw std::runtime_error("Send Failed");
//		}
//
//		send(m_socket, s.c_str(), (int)s.length(), 0);
//		if (error == SOCKET_ERROR)
//		{
//			closesocket(m_socket);
//			WSACleanup();
//			throw std::runtime_error("Second Send Failed");
//		}
//	}
//}

#endif
