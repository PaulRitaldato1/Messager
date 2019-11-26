#pragma once
#include "ServerSocket.h"

class Person {
public:
	inline bool send(std::string message) {
		_socket.sendMsg(message);
	}
private:
	BaseSocket _socket;

};