// Server.hpp file
#pragma once
#include <iostream>
#include "client.hpp"
#include "Channel.hpp"
#include <vector>
#include <sstream>
#include <map>
#include <utility> 

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
		void		pass(std::string, Client &);
		void		nick(std::string, Client &);
		void		user(std::string, Client &);
		void		privmsg(std::string, Client &);
		void		join(std::string, Client &);
		void		kick(std::string, Client &);
		void		invite(std::string, Client &);
		void		topic(std::string, Client &);
		void		mode(std::string, Client &);
		bool		channel_exists(Channel &) const;
		bool		channel_exists(const std::string &) const;
		Channel		&get_channel_by_name(const std::string &) const;
		bool		client_exists(const std::string &) const;

		void		execute_cmd();
	private :
		int			_port;
		std::string _passwd;
		std::vector<Client>		_clients;
		std::vector<Channel>	_channels;
};


std::string	get_token(std::string&);

