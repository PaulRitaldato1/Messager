#include "Messager.h"

void Messager::manager() {
	std::cout << "Hello! Welcome to Paul's ananymous messaging CLI client.\n\n";

	bool validUsername = false;
	do {	
		validUsername = validateUsername();

		do {
			checkEscapeKeyPress([]() { std::cout << "Escape key pressed. Exiting messager...\n\n"; exit(0); });
			chooseRoom();
			startMessaging();

		} while (true);
	} while (!validUsername);
}

bool Messager::validateUsername() {

	std::string command = "v";

	bool isValid = false;

	do {
		std::cout << "Please create a username to use while messaging (no spaces allowed): ";

		std::string username = "";
		std::cin >> username;

		sock.sendMsg(command + " " + username);
		std::string response = sock.read();

		if (response == "y")
			isValid = true;
		else {
			std::cout << "Invalid username, it is taken." << std::endl;
		}

	} while (!isValid);

}

void Messager::chooseRoom() {
	std::string  command = "c";
	sock.sendMsg(command);

	std::cout << sock.read();

	int response = -1;
	std::cin >> response;
	sock.sendMsg(std::to_string(response));
	
	std::cout << sock.read();
}

void Messager::checkEscapeKeyPress(std::function<void()> func) {

	if (GetAsyncKeyState(VK_ESCAPE)) {
		listener.onEscapeKey();
		func();
	}

}

bool Messager::startMessaging() {

	std::async([this]() {
		while (!kill) {
			std::cout << sock.read();
		}
	});

	while (!kill) {
		std::string message;
		getline(std::cin, message);
	}


}