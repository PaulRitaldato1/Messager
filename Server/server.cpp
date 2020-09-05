#include "Server.h"
#include "WindowsServerSocket.h"
#include "../Messager/Sockets/WindowsClientSocket.h"
#include <iostream>
#include <string>
#include <sstream>

Server::Server() : m_pool(16)
{
	m_active = true;
#ifdef _WIN32
	m_listener = std::make_shared<WindowsServerSocket>();
#else
#endif
}

void Server::start()
{
	m_pool.enqueue([this] {
		do
		{
			auto newClient = m_listener->startConnectionListening();
			std::shared_ptr<Person> newPerson = std::make_shared<Person>(newClient);
			
			m_pool.enqueue([this, &newPerson] {	handleNewConnection(newPerson); });
		} while (m_active); 
	});

	for (auto room : m_rooms)
	{
		m_pool.enqueue([this, room]
		{
			do
			{
				room->update();
			} while (m_active);
		});
	}

	int kill = 0;
	std::cin >> kill;
	if (kill == 1)
		m_active = false;
}

void Server::handleNewConnection(std::shared_ptr<Person> newConnection)
{
	std::stringstream commandStream(newConnection->read());

	std::string current = "";
	commandStream >> current;
	if (current == "v")
	{
		commandStream >> current;
		newConnection->setName(current);
		for (const std::shared_ptr<Person>& person : m_peopleConnected)
		{
			if (*person == *newConnection)
			{
				newConnection->setName("");
				newConnection->send("n");
				handleNewConnection(newConnection);
			}
		}
		newConnection->send("y");
	}
	else
	{
		newConnection->send("You did something the server didnt like asshole. Disconnecting you.\n");
		return;
	}

	auto swapCommandStream = [](std::stringstream& ss, std::string string) {
		std::stringstream temp(string);
		ss.swap(temp);
	};
	swapCommandStream(commandStream, newConnection->read());
	commandStream >> current;
	if (current == "c")
	{
		if (m_rooms.size() == 0)
		{
			newConnection->send("There are no chat rooms, do you want to create one?(y/n)");
			swapCommandStream(commandStream, newConnection->read());
			commandStream >> current;
			if (current == "y")
			{
				int newRoomIndex = createChatRoom();
				std::string toSend = "You created and joined chat room: " + std::to_string(newRoomIndex + 1) + "\n";
				m_rooms[newRoomIndex]->addPerson(newConnection);
				newConnection->send(toSend);
				return;
			}
			else
			{
				newConnection->send("You chose not to create a chatroom so im disconnecting your ass.\n");
				return;
			}
		}
		else
		{
			std::string toSend = "Enter the room number you wish to join: \n";
			
			for (std::size_t i = 0; i !=m_rooms.size(); ++i)
			{
				toSend += "Chatroom " + std::to_string(i + 1) + "\n";
			}

			newConnection->send(toSend);

			swapCommandStream(commandStream, newConnection->read());
			
			int choice = 0;
			commandStream >> choice;
			if ((choice - 1) >= 0 && choice <= m_rooms.size())
			{
				m_rooms[choice - 1]->addPerson(newConnection);
				std::string res = "Successfully joined chatroom " + std::to_string(choice) + "\n";
				newConnection->send(res);
			}
			else
			{
				newConnection->send("Fuck you dude put in a real index, im closing you out\n");
			}
		}
		m_peopleConnected.push_back(newConnection);
	}
	else
	{
		newConnection->send("You did something the server didnt like asshole. Disconnecting you.\n");
		return;
	}
}

int Server::createChatRoom()
{
	m_rooms.emplace_back(std::move<std::shared_ptr<Chatroom>>(std::make_shared<Chatroom>()));
	return (int)(m_rooms.size() - 1);
}