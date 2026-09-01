#pragma once

namespace Server {
	class WinSock {
	public:
		WinSock(int versionMajor, int versionMinor);
		~WinSock();

		// Non-copyable
		WinSock(const WinSock& o) = delete;
		WinSock& operator=(const WinSock& o) = delete;

		// Movable
		WinSock(WinSock&& o) = default;
		WinSock& operator=(WinSock&& o) = default;
	private:
		void cleanup();
	};
}