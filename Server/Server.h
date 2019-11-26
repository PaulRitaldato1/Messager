#pragma once
#include "ServerSocket.h"
#include "chatroom.h"

class Server {
public:
	Server();
private:
	ServerSocket _listener;
	std::vector<Chatroom> rooms;

};