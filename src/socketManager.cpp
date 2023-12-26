#include "socketManager.hpp"

socketManager::socketManager(int port)
{
	struct sockaddr_in serverAddr;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		perror("socket");
		exit(1);
	}

	serverAddr.sin_port = htons(port);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY );

	if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1)
	{
		perror("bind");
		exit(1);
	}

	if (listen(serverSocket, 10) == -1)
	{
		perror("listen");
		exit(1);
	}
}

int	socketManager::getSocket()
{
	return (serverSocket);
}

socketManager::~socketManager()
{
	close(serverSocket);
}
