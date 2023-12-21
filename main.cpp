// #include "ircserver.hpp"
// #include "socket.hpp"

// int main(int argc, char *argv[])
// {
// 	if (argc != 3) {
// 		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
// 		return (1);
// 	}

// 	int port = std::stoi(argv[1]);
// 	std::string password = argv[2];

// 	// Create and initialize your IRC server logic here
// 	// Initialize server socket, set it to non-blocking mode, and bind it to the specified port
// 	socket	socketManager(port);

// 	// Your IRC server logic here

// 	return 0;
// }
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

const int PORT = 6666; // Default IRC port

void *handleClient(void *c) {
	char buffer[1024];

	int *a = static_cast<int *>(c);
	int	clientSocket = *a;
	while (true)
	{
		memset(buffer, 0, sizeof(buffer));

		// Read data from the client
		ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
		if (bytesRead <= 0)
		{
			perror("Error reading from client");
			break;
		}

		// Process the received data (IRC protocol handling)
		// This is a placeholder; you'll need to implement proper IRC protocol parsing and response logic.

		std::cout << "Received message from client: " << buffer << std::endl;

		// Example: Respond to PING with PONG
		if (strncmp(buffer, "PING", 4) == 0)
		{
			const char* pongMessage = "PONG\r\n";
			send(clientSocket, pongMessage, strlen(pongMessage), 0);
		}

		// Example: Quit the connection on QUIT command
		if (strncmp(buffer, "QUIT", 4) == 0)
		{
			const char* quitMessage = "ERROR :Closing Link\r\n";
			send(clientSocket, quitMessage, strlen(quitMessage), 0);
			break;
		}
	}

	// Close the client socket
	close(clientSocket);
	return (NULL);
}

int main()
{
	int serverSocket, clientSocket;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientLen = sizeof(clientAddr);
	pthread_t	thread;

	// Create socket
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}

	// Setup server address structure
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	// Bind socket to address
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
	{
		perror("Error binding");
		exit(EXIT_FAILURE);
	}

	// Listen for connections
	if (listen(serverSocket, 5) == -1)
	{
		perror("Error listening");
		exit(EXIT_FAILURE);
	}

	std::cout << "IRC Server listening on port " << PORT << std::endl;

	// Accept connections
	while (true)
	{
		if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen)) == -1)
		{
			perror("Error accepting connection");
			continue;
		}

		// Create a new process or thread to handle the client connection
		if (pthread_create(&thread, NULL, handleClient, (void*)&clientSocket) != 0) 
		{
			perror("Error creating thread");
			close(clientSocket);
		}
	}

	// Close the server socket (this code will not be reached in the parent process if using fork)
	close(serverSocket);

	return 0;
}