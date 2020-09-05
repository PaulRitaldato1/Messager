#pragma once
#ifdef _WIN32
#include <combaseapi.h>
#else
//todo for linux
#endif

class Guid
{
public:
	Guid()
	{
		#ifdef _WIN32
		CoCreateGuid(&m_guid);
		#else
		//todo linux
		#endif
	}

	bool operator!=(const Guid& rhs) const
	{
		return !(m_guid == rhs.m_guid);
	}

	bool operator==(const Guid& rhs) const
	{
		return m_guid == rhs.m_guid;
	}

private:


	#ifdef _WIN32
	GUID m_guid;
	#else
	//todo linux
	#endif
};