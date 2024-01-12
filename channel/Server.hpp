// Server.hpp file
#pragma once
#include <iostream>
#include "client.hpp"
#include "Channel.hpp"
#include <vector>
#include <sstream>
#include <map>

class	Server
{
	public:
		Server(int = 0, std::string = "");
		~Server();

		int		get_port() const;
		void	set_port(int);

		std::string get_passwd() const;
		void		set_passwd(std::string &);

		// commands
		void		pass(std::string);
		void		nick(std::string);
		void		user(std::string);
		void		privmsg(std::string);
		void		join(std::string);
		void		kick(std::string);
		void		invite(std::string);
		void		topic(std::string);
		void		mode(std::string);

		void		execute_cmd();
	private :
		int			_port;
		std::string _passwd;
		std::vector<Client>		_clients;
		std::vector<Channel>	_channels;
};

