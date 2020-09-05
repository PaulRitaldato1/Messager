#pragma once
#include <vector>
#include "Person.h"
#include <algorithm>
#include <execution>
#include <mutex>
#include "Guid.h"

class Chatroom 
{
public:
	Chatroom() : m_chatGuid()
	{
	}

	void addPerson(std::shared_ptr<Person> personToAdd)
	{
		std::scoped_lock<std::mutex> lock(m_socketIO);
		personToAdd->getSocket()->setSocketNonBlocking();
		m_participants.emplace_back(personToAdd);
	}

	inline void sendAll(std::string& message, Guid&& excludedGuid) 
	{
		std::scoped_lock<std::mutex> lock(m_socketIO);
		std::for_each(std::execution::par, m_participants.begin(), m_participants.end(), 
			[=](std::shared_ptr<Person> p) 
			{ 
				if (p->getGuid() == excludedGuid)
				{
					p->send(message);
				}
			});
		//for (Person& p : participants) {
		//	p.send(message);
		//}
	}

	void update()
	{
		for (const auto& participant : m_participants)
		{
			std::string res = participant->read();
			if (!res.empty())
			{
				sendAll(res, participant->getGuid());
			}
		}
	}

	Guid getGuid()
	{
		return m_chatGuid;
	}

private:
	std::mutex m_socketIO;
	Guid m_chatGuid;
	std::vector<std::shared_ptr<Person>> m_participants;
};