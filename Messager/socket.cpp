#include "socket.h"

ClientSocket::ClientSocket(std::string hostname, std::string ip) {

	//create socket
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
}

int ClientSocket::resolveHostname(const char* hostname, std::string& ip) {
	
	//addrinfo hints, * result = NULL;
	//hints.ai_flags = AF_INET;
	//hints.ai_protocol = SOCK_STREAM;
	
	hostent* host = gethostbyname(hostname);
	if (host == NULL) {
		return -1;
	}

	in_addr* address = (in_addr*)host->h_addr;
	ip = inet_ntoa(*address);
	return 0;

}
std::string ClientSocket::read() {
	
	char* tmpLen;
	socketIO.lock();
	int readBytes = recv(_socket, tmpLen, sizeof(char), 0);
	if (readBytes <= 0) {
		throw new std::runtime_error("Connection closed");
	}
	int size = *tmpLen - '0';
	//extract the size, create a buffer the appropriate size
	char* msg;
	msg = new(std::nothrow) char[size + 1]();

	if (!msg) {
		std::cerr << "ClientSocket::read(): Failed to allocate memory for message\n";
		return;
	}
	msg[size] = '\0';
	readBytes = recv(_socket, msg, size, 0);
	if (readBytes <= 0) {
		throw new std::runtime_error("Connection closed");
	}
	socketIO.unlock();
	
	std::string message(msg);
	delete[] msg;

	return message;

}

void ClientSocket::sendMsg(std::string s) {
	uint32_t length = htonl(s.length());
	char* len;
	*len = '0' + length;
	socketIO.lock();
	send(_socket, len, sizeof(char), 0);
	send(_socket, s.c_str(), s.length(), 0);
	socketIO.unlock();
}
