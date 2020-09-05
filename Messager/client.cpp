#include "Sockets/ISocket.h"
#include "Sockets/WindowsClientSocket.h"
#include "Sockets/UnixClientSocket.h"
#include "Messager.h"
#include <memory>

//This will be the main for the client
int main() 
{
	std::shared_ptr<ISocket> sock = nullptr;
		
	#ifdef _WIN32
		sock = std::make_shared<WindowsClientSocket>("", "127.0.0.1", "6969");
	#else
		sock = std::make_shared<UnixClientSocket>("", "127.0.0.1", "6969");
	#endif
	
	Messager messager(sock);

	messager.manager();
}