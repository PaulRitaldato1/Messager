#pragma once

#include "common.h"
#include "Sockets/ISocket.h"
#include "Threading/ThreadPool.h"
class MessagerListener {
public:
	//TODO: add listeners to log stuff
	inline void onEscapeKey() 
	{
		std::printf("Escape pressed\n");
	}
};

class Messager 
{
public:
	Messager(std::shared_ptr<ISocket> sock) : m_pool(2)
	{
		m_kill = false;
		m_sock = sock;
	};
	void manager();
private:

	std::atomic<bool> m_kill;
	friend class MessagerListener;
	MessagerListener m_listener;
	std::shared_ptr<ISocket> m_sock;
	ThreadPool m_pool;
	void chooseRoom();
	void checkEscapeKeyPress(std::function<void()> func);
	void validateUsername();
	void startMessaging();

};