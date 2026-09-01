#include "WinSock.hpp"

#include <winsock2.h>

#include <stdexcept>
#include <string>
#include <print>

#pragma comment(lib, "Ws2_32.lib")

namespace Server {
	WinSock::WinSock(int versionMajor, int versionMinor) {
		WSAData wsaData;
		int wsaerr;
		WORD wVersionRequested = MAKEWORD(versionMajor, versionMinor);
		wsaerr = WSAStartup(wVersionRequested, &wsaData);

		if (wsaerr != 0) {
			throw std::runtime_error("WSAStartup failed with error: " + std::to_string(wsaerr));
		}
		else {
			std::println("Winsock DLL found!");
			std::println("Status: {}", wsaData.szSystemStatus);
		}
	}

	WinSock::~WinSock() {
		cleanup();
	}

	void WinSock::cleanup() {
		int r = WSACleanup();

		if (r == SOCKET_ERROR) throw std::runtime_error("WSACleanup() failed: " + std::to_string(WSAGetLastError()));
		std::println("WSACleanup() successful");
	}
}