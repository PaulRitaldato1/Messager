#include "UnixClientSocket.h"

#ifdef linux
UnixClientSocket::UnixClientSocket(std::string hostname, std::string ip)
{
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket < 0)
	{
		std::cerr << "Failed to create socket!" << std::endl;
	}

	memset(&serverAddress, '0', sizeof(serverAddress));
	unsigned short PORT = 1337;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);

	char* port;
	*port = PORT + '0';

	if (ip.empty())
	{
		if (resolveHostname(hostname.c_str(), ip) < 0)
		{
			throw new std::runtime_error("Could no resolve hostname: " + hostname);
		}
	}
	//convert ipv4 to binary data
	if (InetPton(AF_INET, ip.c_str(), &serverAddress.sin_addr) < 0)
	{
		throw new std::runtime_error("Error ClientSocket::ClientSocket(std::string, std::string): Could not use IP address provided");
	}

	if (connect(m_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		throw new std::runtime_error("Error: Could not connect to server.");
	}
}

void UnixClientSocket::sendSocket(std::string& s)
{
	uint32_t length = htonl(s.length());
	char* len;
	*len = '0' + length;
	
	{
		std::scoped_lock<std::mutex> lock(socketIO);
		send(m_socket, len, sizeof(char), 0);
		send(m_socket, s.c_str(), s.length(), 0);
	}
}

std::string UnixClientSocket::readSocket()
{

	char* tmpLen = nullptr;
	char* msg = nullptr;
	
	{
		std::scoped_lock<std::mutex> lock(socketIO);
		
		int readBytes = recv(m_socket, tmpLen, sizeof(char), 0);
		if (readBytes <= 0)
		{
			throw std::runtime_error("Connection closed");
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

long UnixClientSocket::resolveHostname(const char* hostname, std::string& ip)
{

	hostent* host = gethostbyname(hostname);
	if (host == NULL) 
	{
		return -1;
	}

	in_addr* address = (in_addr*)host->h_addr;
	ip = inet_ntoa(*address);
	return 0;
}

#endif