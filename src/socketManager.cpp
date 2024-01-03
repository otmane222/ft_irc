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

	/*	This In Case The Port Was Used And Wants To Reuse It  */

	// int yes=1;
	// //char yes='1'; // Solaris people use this

	// // lose the pesky "Address already in use" error message
	// if (setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
	// 	perror("setsockopt");
	// 	exit(1);
	// } 

	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fcntl");
		exit (1);
	}
	serverAddr.sin_port = htons(port);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1)
	{
		perror("bind");
		exit(1);
	}

	if (listen(serverSocket, SOMAXCONN) == -1)
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