#include "src/ircserver.hpp"
#include "src/socketManager.hpp"
#include "src/client.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return (1);
	}

	int			port = std::atoi(av[1]);
	std::string	password = av[2];


	// Create and initialize your IRC server logic here
	// Initialize server socket, set it to non-blocking mode, and bind it to the specified port
	socketManager	socketManagerr(port);
	ircserver		server;

	try
	{
		server.start(socketManagerr.getSocket(), password);
	}
	catch(std::exception& e)
	{
		std	:: cout <<
					e.what()
					<<
		std ::		endl;
	}

	return 0;
}


// v2
// #include <iostream>
// #include <cstdlib>
// #include <cstring>
// #include <cerrno>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <pthread.h>

// const int PORT = 6666; // Default IRC port

// void *handleClient(void *c) {
// 	char buffer[1024];

// 	int *a = static_cast<int *>(c);
// 	int	clientSocket = *a;
// 	while (true)
// 	{
// 		memset(buffer, 0, sizeof(buffer));

// 		// Read data from the client
// 		ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
// 		if (bytesRead <= 0)
// 		{
// 			perror("Error reading from client");
// 			break;
// 		}

// 		// Process the received data (IRC protocol handling)
// 		// This is a placeholder; you'll need to implement proper IRC protocol parsing and response logic.

// 		std::cout << "Received message from client: " << buffer << std::endl;

// 		// Example: Respond to PING with PONG
// 		if (strncmp(buffer, "PING", 4) == 0)
// 		{
// 			const char* pongMessage = "PONG\r\n";
// 			send(clientSocket, pongMessage, strlen(pongMessage), 0);
// 		}

// 		// Example: Quit the connection on QUIT command
// 		if (strncmp(buffer, "QUIT", 4) == 0)
// 		{
// 			const char* quitMessage = "ERROR :Closing Link\r\n";
// 			send(clientSocket, quitMessage, strlen(quitMessage), 0);
// 			break;
// 		}
// 	}

// 	// Close the client socket
// 	close(clientSocket);
// 	return (NULL);
// }

// int main()
// {
// 	int serverSocket, clientSocket;
// 	struct sockaddr_in serverAddr, clientAddr;
// 	socklen_t clientLen = sizeof(clientAddr);
// 	pthread_t	thread;

// 	// Create socket
// 	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
// 	{
// 		perror("Error creating socket");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Setup server address structure
// 	serverAddr.sin_family = AF_INET;
// 	serverAddr.sin_port = htons(PORT);
// 	serverAddr.sin_addr.s_addr = INADDR_ANY;

// 	// Bind socket to address
// 	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
// 	{
// 		perror("Error binding");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Listen for connections
// 	if (listen(serverSocket, 5) == -1)
// 	{
// 		perror("Error listening");
// 		exit(EXIT_FAILURE);
// 	}

// 	std::cout << "IRC Server listening on port " << PORT << std::endl;

// 	// Accept connections
// 	while (true)
// 	{
// 		if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen)) == -1)
// 		{
// 			perror("Error accepting connection");
// 			continue;
// 		}

// 		// Create a new process or thread to handle the client connection
// 		if (pthread_create(&thread, NULL, handleClient, (void*)&clientSocket) != 0) 
// 		{
// 			perror("Error creating thread");
// 			close(clientSocket);
// 		}
// 	}

// 	// Close the server socket (this code will not be reached in the parent process if using fork)
// 	close(serverSocket);

// 	return 0;
// }



// v3
// #include <iostream>
// #include <cstdlib>
// #include <cstring>
// #include <cerrno>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <sys/time.h>

// const int PORT = 6667;
// // const int MAX_CLIENTS = 10;

// int main() {
// 	int serverSocket, clientSocket;
// 	struct sockaddr_in serverAddr, clientAddr;
// 	socklen_t clientLen = sizeof(clientAddr);

// 	fd_set readfds;
// 	int maxfd;

// 	// Create socket
// 	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
// 		perror("Error creating socket");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Setup server address structure
// 	serverAddr.sin_family = AF_INET;
// 	serverAddr.sin_port = htons(PORT);
// 	serverAddr.sin_addr.s_addr = INADDR_ANY;

// 	// Bind socket to address
// 	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
// 		perror("Error binding");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Listen for connections
// 	if (listen(serverSocket, 5) == -1)
// {
// 		perror("Error listening");
// 		exit(EXIT_FAILURE);
// 	}

// 	std::cout << "IRC Server listening on port " << PORT << std::endl;

// 	// Initialize the set of active sockets
// 	FD_ZERO(&readfds);
// 	FD_SET(serverSocket, &readfds);
// 	maxfd = serverSocket;

// 	// Accept connections and handle communication
// 	while (true) {
// 		fd_set tmpfds = readfds;
// 		if (select(maxfd + 1, &tmpfds, NULL, NULL, NULL) == -1) {
// 			perror("Error in select");
// 			exit(EXIT_FAILURE);
// 		}

// 		// Check if there is a new connection request
// 		if (FD_ISSET(serverSocket, &tmpfds))
// 		{
// 			if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen)) == -1)
// 			{
// 				perror("Error accepting connection");
// 				continue ;
// 			}
// 			else
// 			{
// 				std::cout << "New connection accepted. Client socket: " << clientSocket << std::endl;

// 				FD_SET(clientSocket, &readfds);
// 				if (clientSocket > maxfd)
// 					maxfd = clientSocket;
// 			}
// 		}

// 		// Check existing clients for data
// 		for (int i = serverSocket + 1; i <= maxfd; ++i)
// 		{
// 			if (FD_ISSET(i, &tmpfds))
// 			{
// 				char buffer[1024];
// 				ssize_t bytesRead = recv(i, buffer, sizeof(buffer) - 1, 0);

// 				if (bytesRead <= 0)
// 				{
// 					if (bytesRead == 0)
// 					{
// 						std::cout << "Connection closed by client. Socket: " << i << std::endl;
// 					}
// 					else
// 					{
// 						perror("Error reading from client");
// 					}

// 					close(i);
// 					FD_CLR(i, &readfds);
// 				}
// 				else
// 				{
// 					buffer[bytesRead] = '\0';
// 					std::cout << "Received message from client " << i << ": " << buffer << std::endl;

// 					// Process the received data (IRC protocol handling)
// 					// This is a placeholder; you'll need to implement proper IRC protocol parsing and response logic.

// 					// Example: Respond to PING with PONG
// 					if (strncmp(buffer, "PING", 4) == 0) {
// 						const char* pongMessage = "PONG\r\n";
// 						send(i, pongMessage, strlen(pongMessage), 0);
// 					}

// 					// Example: Quit the connection on QUIT command
// 					if (strncmp(buffer, "QUIT", 4) == 0) {
// 						const char* quitMessage = "ERROR :Closing Link\r\n";
// 						send(i, quitMessage, strlen(quitMessage), 0);
// 						close(i);
// 						FD_CLR(i, &readfds);
// 					}
// 				}
// 			}
// 		}
// 	}

// 	// Close the server socket (this code will not be reached)
// 	close(serverSocket);

// 	return 0;
// }













// v4
// #include <iostream>
// #include <cstring>
// #include <cstdlib>
// #include <unistd.h>
// #include <arpa/inet.h>

// // Constants for simplicity
// const int PORT = 12345;
// const int BUFFER_SIZE = 1024;

// int main() {
//     // Step 1: Create a socket (AF_INET for IPv4, SOCK_STREAM for TCP)
//     int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1) {
//         std::cerr << "Error creating socket" << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     // Step 2: Configure server address
//     sockaddr_in serverAddress;
//     memset(&serverAddress, 0, sizeof(serverAddress));
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_addr.s_addr = INADDR_ANY;
//     serverAddress.sin_port = htons(PORT);

//     // Step 3: Bind the socket to the server address
//     if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
//         std::cerr << "Error binding socket" << std::endl;
//         close(serverSocket);
//         exit(EXIT_FAILURE);
//     }

//     // Step 4: Set the socket to listen for incoming connections
//     if (listen(serverSocket, 10) == -1) {
//         std::cerr << "Error listening on socket" << std::endl;
//         close(serverSocket);
//         exit(EXIT_FAILURE);
//     }

//     std::cout << "Server listening on port " << PORT << "..." << std::endl;

//     // Step 5: Accept incoming connections
//     sockaddr_in clientAddress;
//     socklen_t clientAddressLength = sizeof(clientAddress);
//     int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddressLength);
//     if (clientSocket == -1) {
//         std::cerr << "Error accepting connection" << std::endl;
//         close(serverSocket);
//         exit(EXIT_FAILURE);
//     }

//     std::cout << "Connection accepted from " << inet_ntoa(clientAddress.sin_addr) << std::endl;

//     // Step 6: Use the clientSocket for communication (send/receive data)
//     char buffer[BUFFER_SIZE];
//     ssize_t bytesRead;

//     while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
//         // Process received data
//         std::cout << "Received: " << std::string(buffer, bytesRead) << std::endl;

//         // Echo the data back to the client
//         // send(clientSocket, buffer, bytesRead, 0);
//     }

//     // Step 7: Close the sockets when done
//     close(clientSocket);
//     close(serverSocket);

//     return 0;
// }
