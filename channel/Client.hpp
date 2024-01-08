// Client.hpp file
#pragma once
#include <iostream>
#include <vector>

class	Client
{
	public:
		Client(int = -1, std::string = "", std::string = "");∏
		Client(const Client&);
		Client& operator = (const Client&);
		bool	operator == (const Client&);
		~Client();

		// geters
		int							get_socket_fd() const;
		std::string 				get_nick_name() const;
		std::string 				get_name() const;
		std::vector<std::string>	get_invites() const;

		// seters
		int					set_socket_fd(int);
		void 				set_nick_name(std::string);
		void 				set_name(std::string);
		void				set_invites(std::vector<std::string> &);
		
		
	private :
		int	_sock_fd;
		std::string 				_nick_name;
		std::string 				_name;
		std::vector<std::string>	_invites;
};

