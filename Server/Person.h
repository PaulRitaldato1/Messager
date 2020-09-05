#pragma once
#include "../Messager/Sockets/ISocket.h"
#include "Guid.h"
#include <memory>

class Person 
{
public:
	Person(std::shared_ptr<ISocket> socket) : m_personGuid()
	{
		m_socket = socket;
	}

	//bool send(std::string&& message) 
	//{
	//	m_socket->sendSocket(message);
	//}
	
	void send(std::string message)
	{
		m_socket->sendSocket(message);
	}

	std::string read()
	{
		return m_socket->readSocket();
	}

	void setName(std::string name)
	{
		m_name = name;
	}

	Guid getGuid()
	{
		return m_personGuid;
	}

	std::shared_ptr<ISocket> getSocket()
	{
		return m_socket;
	}

	bool operator==(const Person& rhs) const
	{
		return m_name == m_name;
	}

private:
	Guid m_personGuid;
	std::string m_name;
	std::shared_ptr<ISocket> m_socket;

};
//
//
//bool operator==(const Person& lhs, const Person& rhs)
//{
//	return lhs.m_name == rhs.m_name;
//}