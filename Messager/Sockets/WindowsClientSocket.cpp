#include "WindowsClientSocket.h"

#ifdef _WIN32

WindowsClientSocket::WindowsClientSocket(std::string&& hostname, std::string&& ip, std::string&& port) 
{

	//create socket
	int resultHandler;
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

WindowsClientSocket::~WindowsClientSocket() 
{
	std::printf("WindowsClientSocket destructor called\n");
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	WSACleanup();
}

std::string WindowsClientSocket::readSocket() 
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

void WindowsClientSocket::sendSocket(std::string& s)
{

	int error = 0;
	std::size_t length = s.length();
	char* len = nullptr;
	*len = '0' + length;
	
	{
		std::scoped_lock<std::mutex> lock(socketIO);

		send(m_socket, len, sizeof(char), 0);
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

#endif
