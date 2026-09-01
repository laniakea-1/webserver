#pragma once

namespace Server {
	class Socket {
	public:
		Socket();
		~Socket();

		// Copyable????

		// Move ctor????
	private:
		void cleanup();
	};
}