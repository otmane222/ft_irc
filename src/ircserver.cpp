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
	std::vector<client>						clss;
	std::vector<int>						fds;
	std::vector<pollfd>						pollfds;
	int										_clientSocket;
	struct sockaddr_in						clientaddr;
	socklen_t								len = sizeof(clientaddr);
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
						// cls.push_back(a);
					}
				}
				else
				{
					char	buffer[100];
					std::string line = "";
					while (1)
					{
						ssize_t	bufferr;
						bufferr = recv(pollfds[i].fd, buffer, sizeof(buffer) - 1, 0); // Recv In A While Until You Find A '\n'
						if (std::strchr(buffer, '\n'))
						{
							if (bufferr != -1)
								buffer[bufferr] = '\0';
							line.append(buffer);
							break;
						}
						if (bufferr <= 0)
						{
							if (bufferr == 0)
								std::cerr << "Connection closed by client" << std::endl;
							else
								std::cerr << "Error receving" << std::endl;
							close(pollfds[i].fd);
							pollfds.erase(pollfds.begin() + i);
							clients.erase(pollfds[i].fd);
							fds.erase(fds.begin() + i);
							break ;
						}
						line.append(buffer);

					}
					if (line == "")
						continue ;
					std:: cout << "Message Received From client " << i << " " << line << std::endl;
					if (!clients[pollfds[i].fd].stat.registered)
						clients[pollfds[i].fd].registration(line, password, clss);
					parse_command(line, clients, pollfds, fds, i, channels);
					(void)password;
				}
			}
		}
	}
}
