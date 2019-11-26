#pragma once
#include "ServerSocket.h"

class Person {
public:
	Person(BaseSocket& socket) : _socket(socket) {}
	inline bool send(std::string message) {
		_socket.sendMsg(message);
	}
private:
	BaseSocket _socket;

};