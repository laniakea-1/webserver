#include "Server/WinSock.hpp"
#include "Server/Socket.hpp"

#include <iostream>
#include <string>
#include <print>
#include <sstream>
#include <array>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	// Step 1: Load WSA (Windows Sockets API) DLL
	Server::WinSock ws(2, 2);

	// TODO: make socket class
	Server::Socket listen;

/*	bool running = true;
	std::array<SOCKET, 10> connections;*/

	try {
		// Step 2: Create a socket; note that this just creates a socket and does not actually bind to any network address
		listen.create();

		// Step 3-4: Bind the socket and listen
		listen.bindAndListen(55555, 1);

		// Step 5: Accept and connect
		std::array<char, 4096> buffer;
		std::string body;
		std::ifstream file(std::string(SOURCE_DIR) + "/src/website/index.html");

		// Read
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file!");
		}
		
		std::stringstream stream;
		stream << file.rdbuf();
		body = stream.str();

		while (true) {
			Server::Socket accept = listen.accept();
			int bytesReceived = accept.receive(buffer.data(), buffer.size());

			std::println("Received {} bytes:", bytesReceived);
			std::cout.write(buffer.data(), bytesReceived);
			std::cout << '\n';

			// Test response from server
			std::string body = "<!DOCTYPE html><html><head><title>Example</title></head><body><h1>Hello, World!</h1></body></html>";
			std::ostringstream response;
			response << "HTTP/1.1 " << 200 << " " << "OK" << "\r\n";
			response << "Content-Type: " << "text/html" << "\r\n";
			response << "Content-Length: " << body.size() << "\r\n";
			response << "Connection: close\r\n\r\n";
			response << body;

			std::println("Sending response...");
			accept.send(response.str().data(), response.str().length());
			std::println("Response sent");
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}