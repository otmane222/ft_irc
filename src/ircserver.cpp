#include "ircserver.hpp"
#include "socketManager.hpp"



ircserver::ircserver(/* args */)
{
}

ircserver::~ircserver()
{
}

void close_fds(std::vector<int> fds)
{
	std::vector<int>::iterator it = fds.begin();
	for (size_t i = 0; it != fds.end(); it++, i++)
	{
		std::cout << "closed: "<< fds[i] << std::endl;
		close(fds[i]);
	}
	
}

std::string	recving(int fd, std::vector<int> fds)
{
	char	buffer[512];
	std::string line;
	std::string str;
	ssize_t	bufferr;
	while (1)
	{
		bufferr = recv(fd, buffer, sizeof(buffer) - 1, 0);
		if (bufferr == -1)
		{
			close_fds(fds);
			throw std::runtime_error("Error reciving");
		}
		buffer[bufferr] = '\0';
		line.append(buffer);
		if (std::strchr(buffer, '\n'))
			break;
	}
	// size_t i = 0;
	// while (i < line.size())
	// {
	// 	write(1, ":", 1);
	// 	if (line[i] != '\r')
	// 		write(1, &line[i], 1);
	// 	write(1, ":", 1);
	// 	write(1, "\n", 1);
	// 	i++;
	// }
	// exit (0);
	return (line);
}

void	ircserver::start(int _serverSocket, std::string password)
{
	std::map<int, client>					clients;
	std::map<std::string, std::set<int> >	channels;
	std::vector<client>						cls;
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
			throw std::runtime_error("Error polling");
		for (size_t i = 0; i <= pollfds.size(); i++) // EAGAIN is an erno 
		{
			if ((pollfds[i].revents & POLLIN) == POLLIN)
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

						client	a(_clientSocket);
						clients[_clientSocket] = a;
						cls.push_back(a);
					}
					break ;
				}
				else if ((pollfds[i].revents & POLLHUP) == POLLHUP)
				{
					clients[pollfds[i].fd].close_connection(pollfds, clients, i);
					break ;
				}
				else
				{
					std::string line = recving(pollfds[i].fd, fds);
					std:: cout << "Message Received From client " << i << " " << line << std::endl;
					clients[pollfds[i].fd].registration(line, password, cls);
					// (void)password;
				}
			}
		}
	}
}

// PASS jaja
// NICK niko
// USER us 0 * realname