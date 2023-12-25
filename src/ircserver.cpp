#include "ircserver.hpp"
#include "socketManager.hpp"


ircserver::ircserver(/* args */)
{
}

ircserver::~ircserver()
{
}

void	ircserver::start(int serverSocket)
{
	int					clientSocket;
	struct sockaddr_in	clientaddr;
	socklen_t			len = sizeof(clientaddr);
	fd_set				currentfds, tmpfds;
	int					maxfd;

	FD_ZERO(&currentfds);
	FD_SET(serverSocket, &currentfds); 
	maxfd = serverSocket;
	while (true)
	{
		tmpfds = currentfds;
		if (select(maxfd + 1, &tmpfds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit (1);
		}

		for (int i = serverSocket; i <= maxfd; i++)
		{
			if (FD_ISSET(i, &tmpfds))
			{
				if (i == serverSocket)
				{
					clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientaddr), &len);
					if (clientSocket == -1)
					{
						perror("Error connecting with client");
						continue ;
					}
					else
					{
						std::cout << "New Connection Has Been Accepted, Client Socket: "<< clientSocket << std::endl;
						FD_SET(clientSocket, &currentfds);
						if (clientSocket > maxfd)
							maxfd = clientSocket;
					}
				}
				else
				{
					char	buffer[1024];
					ssize_t	bufferread = recv(i, buffer, sizeof(buffer) - 1, 0);
					if (bufferread <= 0)
					{
						if (bufferread == 0)
							std::cerr << "Connection closed by client" << std::endl;
						else
							std::cerr << "Error receving" << std::endl;
						close(i);
						FD_CLR(i, &currentfds);
					}
					else
					{
						buffer[bufferread] = '\0';
						std:: cout << "Message Received From client " << i << " " << buffer << std::endl;
						// parse_command();
					}
				}
			}
		}
		
	}
	
}