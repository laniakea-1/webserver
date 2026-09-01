add_library(Server
	# Source
	src/WinSock.cpp
	src/Socket.cpp

	# Header
	include/Server/WinSock.hpp
	include/Server/Socket.hpp
)

target_include_directories(Server PUBLIC
	include
)

target_include_directories(Server PRIVATE
	include/Server
)

target_link_libraries(Server PUBLIC)