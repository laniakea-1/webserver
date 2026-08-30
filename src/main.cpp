#include <ws2tcpip.h>
#include <winsock2.h>
#include <tchar.h>

#include <iostream>
#include <string>
#include <print>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	// Step 1: Load WSA (Windows Sockets API) DLL
	WSAData wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr != 0) {
		throw std::runtime_error("WSAStartup failed with error: " + std::to_string(wsaerr));
	}
	else {
		std::println("Winsock DLL found!");
		std::println("Status: {}", wsaData.szSystemStatus);
	}

	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET acceptSocket = INVALID_SOCKET;

/*	bool running = true;
	std::array<SOCKET, 10> connections;*/

	try {
		// Step 2: Create a socket; note that this just creates a socket and does not actually bind to any network address
		listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Creates a socket bound to TCP
		if (listenSocket == INVALID_SOCKET) {
			throw std::runtime_error("Socket failed with error: " + std::to_string(WSAGetLastError()));
		}
		else {
			std::println("Socket created successfully!");
		}

		// Step 3: Bind the socket
		u_short port = 55555;
		
		SOCKADDR_IN listenAddr; // sockaddr_in = "Sock Address Internet" (specifically, this is information describing the network address and port that should be associated with the socket)
		listenAddr.sin_family = AF_INET; // Address family (AF_INET = IPv4)
		InetPton(listenAddr.sin_family, _T("127.0.0.1"), &listenAddr.sin_addr.s_addr); // InetPton = Internet Presentation to Network (converts IP address); converts IP string to binary form and writes to service
		listenAddr.sin_port = htons(port);

		if (bind(listenSocket, (SOCKADDR*)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR) {
			throw std::runtime_error("Failed to bind socket: " + std::to_string(WSAGetLastError()));
		}
		else std::println("Socket bound successfully to port {}!", port);

		// Step 4: Listen!
		if (listen(listenSocket, 1) == SOCKET_ERROR) {
			throw std::runtime_error("Failed to listen on port " + std::to_string(port));
		}
		else {
			std::println("Listening on port {} ...", port);
		}

		// Step 5: Accept and connect
		SOCKADDR_IN acceptAddr{};
		int acceptLen = sizeof(acceptAddr);
		acceptSocket = accept(listenSocket, (SOCKADDR*)&acceptAddr, &acceptLen);
		if (acceptSocket == INVALID_SOCKET) {
			throw std::runtime_error("Failed to accept: " + std::to_string(WSAGetLastError()));
		}
		else {
			std::println("Accepted connection on port {}", ntohs(acceptAddr.sin_port));
		}

		char buffer[4096];

		int bytesReceived = recv(acceptSocket, buffer, sizeof(buffer), 0);

		if (bytesReceived == SOCKET_ERROR) {
			throw std::runtime_error("Failed to receive data: " + std::to_string(WSAGetLastError()));
		}

		std::println("Received {} bytes:", bytesReceived);
		std::cout.write(buffer, bytesReceived);
		std::cout << '\n';

		// Test response from server
		std::string body = "<!DOCTYPE html><html><head><title>Example</title></head><body><h1>Bri is incredibly sexy and beautiful mwehehe</h1></body></html>";
		std::ostringstream response;
		response << "HTTP/1.1 " << 200 << " " << "OK" << "\r\n";
		response << "Content-Type: " << "text/html" << "\r\n";
		response << "Content-Length: " << body.size() << "\r\n";
		response << "Connection: close\r\n\r\n";
		response << body;
		
		std::println("Sending response...");
		send(acceptSocket, response.str().data(), response.str().length(), 0);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cout << "Closing socket and unloading DLL..." << std::endl;
		closesocket(listenSocket);
		WSACleanup(); // Deregister our app from Winsock, should we have a failure
		return EXIT_FAILURE;
	}
	
	WSACleanup(); // Deregister our app from Winsock
	return EXIT_SUCCESS;
}
