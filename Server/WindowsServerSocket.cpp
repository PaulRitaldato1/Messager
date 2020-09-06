#include "WindowsServerSocket.h"

WindowsServerSocket::WindowsServerSocket()
{
	struct addrinfo* result = nullptr, hints;

	int resultHandler;
	resultHandler = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (resultHandler != 0) 
	{
		printf("WSAStartup failed with error: %d\n", resultHandler);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int resultH = getaddrinfo(nullptr, "6969", &hints, &result);
	if (resultH != 0)
	{
		std::printf("getaddrinfo failed: %d\n", resultH);
		WSACleanup();
		throw std::runtime_error("");
	}

	m_socket = INVALID_SOCKET;
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_socket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		throw std::runtime_error("");
	}

	resultH = bind(m_socket, result->ai_addr, (int)result->ai_addrlen);
	if (resultH == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(m_socket);
		WSACleanup();
	}

	freeaddrinfo(result);

	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(m_socket);
		WSACleanup();
		throw std::runtime_error("");
	}
}

std::shared_ptr<ISocket> WindowsServerSocket::startConnectionListening()
{
	SOCKET ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(m_socket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(m_socket);
		WSACleanup();
		return nullptr;
	}

	return std::make_shared<WindowsClientSocket>(ClientSocket);
}