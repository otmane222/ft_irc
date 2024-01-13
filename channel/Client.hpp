
#pragma once
#include <iostream>
#include <vector>

class	Client
{
	public:
		Client(int = -1, std::string = "", std::string = "", bool = false);
		~Client();

		// geters
		int							get_socket_fd() const;
		std::string 				get_nick_name() const;
		std::string 				get_user_name() const;
		std::string 				get_real_name() const;
		std::vector<std::string>	get_invites() const;

		// seters
		void				set_socket_fd(int);
		void 				set_nick_name(std::string);
		void 				set_user_name(std::string);
		void 				set_real_name(std::string);
		void				set_invites(std::vector<std::string> &);
		

	private :
		int							_sock_fd;
		std::string 				_nick_name;
		std::string 				_user_name;
		std::string					_real_name;
		std::string					_hostname;
		bool						_registred;
};

