#pragma once
#include "common.h"

class ClientSocket {
public:
	ClientSocket(std::string hostname, std::string ip);
	inline ~ClientSocket() { closesocket(_socket); };
	
	std::string read();
	void sendMsg(std::string s);

private:
	std::mutex socketIO;
	int _socket;
	struct sockaddr_in addrress, serverAddress;
	int resolveHostname(const char* hostname, std::string& ip);
	
};