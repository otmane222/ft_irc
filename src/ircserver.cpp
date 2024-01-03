#include "ircserver.hpp"
#include "socketManager.hpp"



ircserver::ircserver(/* args */)
{
}

ircserver::~ircserver()
{
}

void	ircserver::start(int _serverSocket, std::string password)
{
	std::map<int, client>					clients;
	std::map<std::string, std::set<int> >	channels;
	int										_clientSocket;
	struct sockaddr_in						clientaddr;
	socklen_t								len = sizeof(clientaddr);
	std::vector<int>						fds;
	std::vector<pollfd>						pollfds;
	pollfd Pollfd;

	Pollfd.fd = _serverSocket;
	Pollfd.events = POLLIN;
	Pollfd.revents = 0;

	fds.push_back(_serverSocket);

	pollfds.push_back(Pollfd);
	while (true)
	{
		if (poll(pollfds.data(), pollfds.size(), -1) == -1)
		{
			perror("select");
			exit (1); // throw exception ;
		}

		for (size_t i = 0; i <= pollfds.size(); i++) // EAGAIN is an erno 
		{
			if (pollfds[i].revents & POLLIN)
			{
				if (i == 0)
				{
					_clientSocket = accept(_serverSocket, reinterpret_cast<struct sockaddr*>(&clientaddr), &len);
					if (_clientSocket == -1)
					{
						perror("Error connecting with client");
						continue ;
					}
					else
					{
						std::cout << "New Connection Has Been Accepted, Client Socket: "<< _clientSocket - 3 << std::endl;
						Pollfd.fd = _clientSocket;
						pollfds.push_back(Pollfd);

						fds.push_back(_clientSocket);

						client	a(_clientSocket, fds);
						clients[_clientSocket] = a;
					}
				}
				else
				{
					char	buffer[1024];
					ssize_t	bufferread = recv(pollfds[i].fd, buffer, sizeof(buffer) - 1, 0); // Recv In A While Until You Find A '\n'
					if (bufferread <= 0)
					{
						if (bufferread == 0)
							std::cerr << "Connection closed by client" << std::endl;
						else
							std::cerr << "Error receving" << std::endl;
						close(pollfds[i].fd);
						pollfds.erase(pollfds.begin() + i);
						clients.erase(pollfds[i].fd);
						fds.erase(fds.begin() + i);
					}
					else
					{
						buffer[bufferread] = '\0';
						std:: cout << "Message Received From client " << i << " " << buffer << std::endl;
						parse_command(buffer, clients, pollfds, fds, i, channels);
						(void)password;
					}
				}
			}
		}
	}
}
