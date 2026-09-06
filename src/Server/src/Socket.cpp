#include "Socket.hpp"

#include <stdexcept>
#include <string>
#include <print>
#include <iostream>

namespace Server {
	// -struction
	Socket::Socket() : handle_(INVALID_SOCKET) {
		
	}

	Socket::Socket(SOCKET handle) : handle_(handle) {
		
	}

	Socket::~Socket() {
		cleanup();
	}

	//
	void Socket::create() {
		cleanup();
		handle_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (handle_ == INVALID_SOCKET) {
			throw std::runtime_error("Socket failed with error: " + std::to_string(WSAGetLastError()));
		}
		else {
			std::println("Socket created successfully!");
		}
	}

	void Socket::bindAndListen(u_short port, int backlog) {
		// BIND
		SOCKADDR_IN listenAddr; // sockaddr_in = "Sock Address Internet" (specifically, this is information describing the network address and port that should be associated with the socket)
		listenAddr.sin_family = AF_INET; // Address family (AF_INET = IPv4)
		//InetPton(listenAddr.sin_family, _T("127.0.0.1"), &listenAddr.sin_addr.s_addr); // InetPton = Internet Presentation to Network (converts IP address); converts IP string to binary form and writes to service
		listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		listenAddr.sin_port = htons(port);

		if (::bind(handle_, (SOCKADDR*)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR) {
			throw std::runtime_error("Failed to bind socket: " + std::to_string(WSAGetLastError()));
		}
		else std::println("Socket bound successfully to port {}!", port);

		// LISTEN
		if (::listen(handle_, backlog) == SOCKET_ERROR) {
			throw std::runtime_error("Failed to listen on port " + std::to_string(port));
		}
		else {
			std::println("Listening on port {} ...", port);
		}
	}

	Socket Socket::accept() {
		SOCKADDR_IN addr{};
		int len = sizeof(addr);
		SOCKET accept = INVALID_SOCKET;
		accept = ::accept(handle_, (SOCKADDR*)&addr, &len);
		if (accept == INVALID_SOCKET) {
			throw std::runtime_error("Failed to accept: " + std::to_string(WSAGetLastError()));
		}
		else {
			std::println("Accepted connection on port {}", ntohs(addr.sin_port));
		}

		return Socket(accept);
	}

	int Socket::receive(char* pBuffer, int size) {
		int bytesReceived = recv(handle_, pBuffer, size, 0);

		if (bytesReceived == SOCKET_ERROR) {
			throw std::runtime_error("Failed to receive data: " + std::to_string(WSAGetLastError()));
		}

		return bytesReceived;
	}

	void Socket::send(char* data, int size) {
		::send(handle_, data, size, 0); // TODO: flags??
	}

	void Socket::cleanup() {
		closesocket(handle_);
	}
}