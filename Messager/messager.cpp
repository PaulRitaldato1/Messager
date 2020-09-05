#include "Messager.h"

void Messager::manager() {
	std::cout << "Hello! Welcome to Paul's anonymous messaging CLI client.\n\n";

	validateUsername();
	chooseRoom();
	startMessaging();
}

void Messager::validateUsername() {

	std::string command = "v";

	do {
		std::cout << "Please create a username to use while messaging (no spaces allowed): ";

		std::string username = "";
		std::cin >> username;

		std::string msg = command + " " + username;
		m_sock->sendSocket(msg);
		std::string response = m_sock->readSocket();

		if (response == "y")
			break;
		else {
			std::cout << "Invalid username, it is taken or wrong." << std::endl;
		}

	} while (true);
}

void Messager::chooseRoom() 
{
	std::string  command = "c";
	m_sock->sendSocket(command);

	std::cout << m_sock->readSocket();

	int response = -1;
	std::cin >> response;
	std::string res = std::to_string(response);
	m_sock->sendSocket(res);
	
	std::cout << m_sock->readSocket();
}

void Messager::checkEscapeKeyPress(std::function<void()> func) 
{
	
	//if (GetAsyncKeyState(VK_ESCAPE)) 
	//{
	//	m_listener.onEscapeKey();
	//	func();
	//}

}

void Messager::startMessaging() {

	//read messages constantly
	m_pool.enqueue([this] {
		while (!m_kill) 
		{
			std::cout << m_sock->readSocket();
		}
	});
	
	//check for escape key press
	m_pool.enqueue([this]() {
		while (!m_kill) {
			checkEscapeKeyPress([this]() {std::cout << "Escape key pressed, leaving chat room." << std::endl; m_kill = true; });
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	});

	while (!m_kill) {
		std::string message = "";
		getline(std::cin, message);
		m_sock->sendSocket(message);
	}


}