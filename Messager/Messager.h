#pragma once

#include "common.h"
#include "socket.h"

class MessagerListener {
public:
	//TODO: add listeners to log stuff
	inline void onEscapeKey() {
		std::cout << "Escape pressed" << std::endl;
	}
};
class Messager {
public:
	Messager(ClientSocket& sock) : sock(sock) {
		kill = false;
	};
	void manager();
private:

	std::atomic<bool> kill;
	friend class MessagerListener;
	MessagerListener listener;
	ClientSocket& sock;

	void chooseRoom();
	void checkEscapeKeyPress(std::function<void()> func);
	bool validateUsername();
	void startMessaging();

};