#include "socket.h"
#include "Messager.h"


//This will be the main for the client
int main() {
	ClientSocket sock("127.0.0.1", "6969");
	Messager messager(sock);

	messager.manager();
}