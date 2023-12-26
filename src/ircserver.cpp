#include "ircserver.hpp"
#include "socketManager.hpp"


ircserver::ircserver(/* args */)
{
}

ircserver::~ircserver()
{
}

void	ircserver::start(int _serverSocket)
{
	std::map<std::string, client>			clients;
	std::map<std::string, std::set<int> >	channels;
	int										_clientSocket;
	struct sockaddr_in						clientaddr;
	socklen_t								len = sizeof(clientaddr);
	fd_set									currentfds, tmpfds;
	std::vector<int>						fds;
	int										maxfd;

	fds.push_back(_serverSocket);
	FD_ZERO(&currentfds);
	FD_SET(_serverSocket, &currentfds);
	maxfd = _serverSocket;
	while (true)
	{
		tmpfds = currentfds;
		if (select(maxfd + 1, &tmpfds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit (1);
		}

		for (int i = _serverSocket; i <= maxfd; i++)
		{
			if (FD_ISSET(i, &tmpfds))
			{
				if (i == _serverSocket)
				{
					_clientSocket = accept(_serverSocket, reinterpret_cast<struct sockaddr*>(&clientaddr), &len);
					if (_clientSocket == -1)
					{
						perror("Error connecting with client");
						continue ;
					}
					else
					{
						std::cout << "New Connection Has Been Accepted, Client Socket: "<< _clientSocket << std::endl;
						FD_SET(_clientSocket, &currentfds);

						fds.push_back(_clientSocket);
						client	a(_clientSocket, fds);
						std::string	clientname = "client ";

						clientname = std::to_string(_clientSocket - 3);
						clients[clientname] = a;

						if (_clientSocket > maxfd)
							maxfd = _clientSocket;
					}
				}
				else
				{
					char	buffer[10000];
					ssize_t	bufferread = recv(i, buffer, sizeof(buffer) - 1, 0);
					if (bufferread <= 0)
					{
						if (bufferread == 0)
							std::cerr << "Connection closed by client" << std::endl;
						else
							std::cerr << "Error receving" << std::endl;
						close(i);
						FD_CLR(i, &currentfds);

						std::vector<int>::iterator	it = std::find(fds.begin(), fds.end(), i);
						if(it != fds.end())
							fds.erase(it);
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
