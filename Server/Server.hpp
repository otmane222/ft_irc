#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"
#define MAX_LEN 512

class	Server
{
	public:
		Server(int = 0, std::string = "");
		~Server();

		int		get_port() const;
		int		get_socket_fd() const;
		std::string get_passwd() const;


		void	set_passwd(std::string &);
		void	set_port(int);
		void	set_socket_fd(int);

		bool		channel_exists(Channel &) const;
		bool		channel_exists(const std::string &);
		void		remove_channel(const Channel &);
		Channel		&get_channel_by_name(const std::string &);
		bool		client_exists(Client &);
		bool		client_exists(const std::string &);
		Client		&get_client_by_name(const std::string &);
		Client		&get_client_by_id(int sock_id);
		void		reply(const Client &, const std::string &) const;
		// commands
		void		pass(std::string, Client &);
		void		nick(std::string, Client &);
		void		user(std::string, Client &);
		void		privmsg(std::string, Client &);
		void		join(std::string, Client &);
		void		kick(std::string, Client &);
		void		invite(std::string, Client &);
		void		topic(std::string, Client &);
		void		mode(std::string, Client &);
		void		ping(std::string, Client &);
		void		pong(std::string, Client &);
		void		quit(std::string, Client &);
		// void		part(std::string, Client &);

		void		execute_cmd(std::string, Client &);

		void		start();
	private :
		int						_socket_fd;
		int						_port;
		std::string 			_passwd;
		std::vector<Client>		_clients;
		std::vector<Channel>	_channels;
		std::map<std::string, void (Server::*)(std::string, Client&)> _cmds;
		std::vector<pollfd>		pollfds;
		// std::map<int, Client>		_clients;
};

std::string	get_token(std::string&);
std::string	recving(int fd, std::vector<int> fds);
std::string	get_source(std::string, std::string, std::string);
int			ft_atoi(const char *);
