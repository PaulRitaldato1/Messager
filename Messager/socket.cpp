#include "socket.h"

ClientSocket::ClientSocket(std::string hostname, std::string ip) {

	//create socket
#ifdef _WIN32
	int resultHandler;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	resultHandler = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &result);
	if (resultHandler != 0) {
		char error[50];
		sprintf_s(error, 50, "getaddrinfo failed: %d\n", resultHandler);
		WSACleanup();
		throw std::runtime_error(error);
	}

	ptr = result;
	_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (_socket == INVALID_SOCKET) {
		char error[50];
		sprintf_s(error, 50, "Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		throw std::runtime_error(error);
	}

	resultHandler = connect(_socket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
	if (resultHandler == SOCKET_ERROR) {
		closesocket(_socket);
		_socket = INVALID_SOCKET;
	}

	//TODO, IF CONNECT FAILS, CHECK NEXT ADDRESS IN RESULT

	freeaddrinfo(result);
	if (_socket == INVALID_SOCKET) {
		WSACleanup();
		throw std::runtime_error("Could not connect to server");
	}

#elif linux
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0) {
		std::cerr << "Failed to create socket!" << std::endl;
	}

	memset(&serverAddress, '0', sizeof(serverAddress));
	unsigned short PORT = 1337;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);

	char* port;
	*port = PORT + '0';

	if (ip.empty()) {
		if (resolveHostname(hostname.c_str(), ip) < 0) {
			throw new std::runtime_error("Could no resolve hostname: " + hostname);
		}
	}
	//convert ipv4 to binary data
	if (InetPton(AF_INET, ip.c_str(), &serverAddress.sin_addr) < 0) {
		throw new std::runtime_error("Error ClientSocket::ClientSocket(std::string, std::string): Could not use IP address provided");
	}

	if (connect(_socket, (struct sockaddr*) & serverAddress, sizeof(serverAddress)) < 0) {
		throw new std::runtime_error("Error: Could not connect to server.");
	}
#endif
	
}

ClientSocket::~ClientSocket() {
#ifdef _WIN32
	shutdown(_socket, SD_BOTH);
	closesocket(_socket);
	WSACleanup();
#elif linux
#endif
}

int ClientSocket::resolveHostname(const char* hostname, std::string& ip) {
	
	//addrinfo hints, * result = NULL;
	//hints.ai_flags = AF_INET;
	//hints.ai_protocol = SOCK_STREAM;
	
	/*hostent* host = gethostbyname(hostname);
	if (host == NULL) {
		return -1;
	}

	in_addr* address = (in_addr*)host->h_addr;
	ip = inet_ntoa(*address);
	return 0;
*/
}
std::string ClientSocket::read() {
	
	char* tmpLen = nullptr;
	socketIO.lock();
	int readBytes = recv(_socket, tmpLen, sizeof(char), 0);
	if (readBytes <= 0) {
		throw std::runtime_error("Connection closed");
	}
	int size = *tmpLen - '0';
	//extract the size, create a buffer the appropriate size
	char* msg;
	msg = new(std::nothrow) char[size + 1]();

	if (!msg) {
		throw std::runtime_error("ClientSocket::read(): Failed to allocate memory for message\n");
	}
	msg[size] = '\0';
	readBytes = recv(_socket, msg, size, 0);
	if (readBytes <= 0) {
		throw std::runtime_error("Connection closed");
	}
	socketIO.unlock();
	
	std::string message(msg);
	delete[] msg;

	return message;

}

void ClientSocket::sendMsg(std::string s) {

#ifdef _WIN32
	int error = 0;
	uint32_t length = s.length();
	char* len = nullptr;
	*len = '0' + length;
	socketIO.lock();
	send(_socket, len, sizeof(char), 0);
	if (error == SOCKET_ERROR) {
		socketIO.unlock();
		closesocket(_socket);
		WSACleanup();
		throw std::runtime_error("Send Failed");
	}

	send(_socket, s.c_str(), s.length(), 0);
	if (error == SOCKET_ERROR) {
		socketIO.unlock();
		closesocket(_socket);
		WSACleanup();
		throw std::runtime_error("Second Send Failed");
	}
	socketIO.unlock();
#elif linux
	uint32_t length = htonl(s.length());
	char* len;
	*len = '0' + length;
	socketIO.lock();
	send(_socket, len, sizeof(char), 0);
	send(_socket, s.c_str(), s.length(), 0);
	socketIO.unlock();
#endif
}
