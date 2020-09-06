#pragma once
#include "chatroom.h"
#include "../Messager/Threading/ThreadPool.h"
#include "Person.h"

#ifdef _WIN32
#include "WindowsServerSocket.h"
#endif
class Server 
{
public:
	Server();
	void start();
	void handleNewConnection(std::shared_ptr<Person> newConnection);
	int createChatRoom();
private:
	std::atomic<bool> m_active;
	ThreadPool m_pool;
	std::shared_ptr<IServerSocket> m_listener;
	std::vector<std::shared_ptr<Chatroom>> m_rooms;
	std::vector<std::shared_ptr<Person>> m_peopleConnected;
};