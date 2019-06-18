#pragma once

#include "common.h"
#include "socket.h"

class MessagerListener {
public:
	//TODO: add listeners to log stuff
	void onEscapeKey();
};
class Messager {
	Messager(ClientSocket& sock) : sock(sock) { 
		kill = false; 
	};
public:

	void manager();
	bool startMessaging();
	bool validateUsername();

private:

	std::atomic<bool> kill;
	friend class MessagerListener;
	MessagerListener listener;
	ClientSocket& sock;

	void chooseRoom();
	void checkEscapeKeyPress(std::function<void()> func);
};