#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#elif linux
#endif

#include <string>
#include <mutex>
#pragma comment (lib, "Ws2_32.lib")

class BaseSocket {
public:
	BaseSocket();
	bool sendMsg(std::string message);
	std::string read();
private:
	uint16_t port = 6969;
#ifdef _WIN32
	struct addrinfo* result = nullptr, hints;
	SOCKET _socket;
#elif linux
#endif
};

class ServerSocket {
public:
	ServerSocket();
	void listen();
private:
	uint16_t port = 6969;
#ifdef _WIN32
	struct addrinfo* result = nullptr, hints;
	SOCKET listener;
#elif linux
#endif
};