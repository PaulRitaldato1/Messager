#pragma once
#include "ServerSocket.h"
#include "chatroom.h"

class Server {
public:
	Server();
	void createChatRoom();
private:
	static int chatIDs;
	ServerSocket _listener;
	std::vector<Chatroom> rooms;

};