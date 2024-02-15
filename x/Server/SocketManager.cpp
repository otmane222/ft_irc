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



	struct addrinfo hints, *result;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int status;
	if ((status = getaddrinfo(hostname, NULL, &hints, &result)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	char* ip_address = NULL;

	void *addr;
	if (result->ai_family == AF_INET)
	{
		addr = &((struct sockaddr_in *)result->ai_addr)->sin_addr;
	}
	else
	{
		addr = &((struct sockaddr_in6 *)result->ai_addr)->sin6_addr;
	}
	ip_address = inet_ntoa(*((struct in_addr*) addr));
	std:: cout << hostname << std::endl;
	std:: cout << ip_address << std::endl;
	// exit (0);

	freeaddrinfo(result);


	in_addr_t	addrr;

	addrr = inet_addr(ip_address);
	serverAddr.sin_addr.s_addr = addrr;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_family = AF_UNSPEC;

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
