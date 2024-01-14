#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

class SocketManager
{
	private:
		int	serverSocket;
	public:
		SocketManager(int port);
		int	getSocket();
		~SocketManager();
};
