#pragma once
#include <vector>
#include "Person.h"

class Chatroom {
public:
	Chatroom();
	void startChatroom();
	bool addPerson(Person& personToAdd);
	inline bool sendAll(std::string message) {
		for (Person p : participants) {
			p.send(message);
		}
	}

	int getID() {
		return chatID;
	}
private:
	std::mutex socketIO;
	static int chatID;
	BaseSocket chatRoomSock;
	std::vector<Person> participants;
};