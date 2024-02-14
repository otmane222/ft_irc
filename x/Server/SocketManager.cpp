#include "SocketManager.hpp"

SocketManager::SocketManager(int port)
{
	struct sockaddr_in serverAddr;

	std::memset(&serverAddr, 0, sizeof(sockaddr_in));
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		perror("socket");
		exit(1);
	}

	/*	This In Case The Port Was Used And Wants To Reuse It  */

	int yes = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	} 

	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fcntl");
		exit (1);
	}

	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == -1) {
		std::cout << "Error getting hostname" << std::endl;
		exit (1);
	}

	struct hostent* host;
	if ((host = gethostbyname(hostname)) == NULL) {
		std::cout << "Error getting hostent" << std::endl;
		exit (1);
	}

	char* ip_address = ç(*((struct in_addr*) host->h_addr_list[0]));

	std :: cout << ip_address << std::endl;

	in_addr_t	addr = inet_addr(ip_address);

	serverAddr.sin_port = htons(port);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;

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

int	SocketManager::getSocket()
{
	return (serverSocket);
}

SocketManager::~SocketManager()
{
	close(serverSocket);
} 
