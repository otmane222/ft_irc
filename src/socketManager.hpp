#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

class socketManager
{
	private:
		int	serverSocket;
	public:
		socketManager(int port);
		int	getSocket();
		~socketManager();
};
