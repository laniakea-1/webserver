#pragma once

#include <winsock2.h>

namespace Server {
	// Note that, for now, the Socket class refers only to TCP sockets; more abstraction may be done in future.
	class Socket {
	public:
		Socket();
		explicit Socket(SOCKET handle);
		~Socket();

		// Copy
		Socket(const Socket& o) = delete;
		Socket& operator=(const Socket& o) = delete;

		// TODO: Move ctor????
		Socket(Socket&& o) = default;
		Socket& operator=(Socket&& o) = default;

		// Listening
		void create();
		void bindAndListen(u_short port, int backlog);
		Socket accept();

		// Connected
		int receive(char* pBuffer, int size);
		void send(char* data, int size);
	private:
		void cleanup();

		SOCKET handle_;
	};
}