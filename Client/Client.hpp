
#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>

// typedef struct s_status
// {
// 	bool	password;
// 	bool	nickname;
// 	bool	username;
// 	bool	registered;
// }	t_status;

class	Client
{
	std::string line;
	public:
		Client(int = -1, std::string hostname = "127.0.0.1");
		~Client();

		// geters
		int							get_socket_fd() const;
		std::string 				get_nick_name() const;
		std::string 				get_user_name() const;
		std::string 				get_real_name() const;
		std::string					get_hostname() const;
		std::string					get_passwd() const;
		std::string					get_line_read() const;
		int							get_status() const;
		bool						get_pass_stat() const;
		bool						get_pass_status() const;
		bool						get_user_status() const;
		bool						get_nick_status() const;

		// seters
		void				set_socket_fd(int);
		void 				set_nick_name(std::string);
		void 				set_user_name(std::string);
		void 				set_real_name(std::string);
		void				set_hostname(std::string);
		void				set_passwd(std::string);
		void				set_line(std::string);
		void				set_status(int);
		void				set_pass_stat(bool);
		void				set_pass_status(bool);
		void				set_user_status(bool);
		void				set_nick_status(bool);

		bool				is_athantificated() const;

		void				close_connection();

		bool				operator==(const Client& cl) const;
		bool				operator!=(const Client& cl) const;
		bool				operator < (const Client& cl) const;

	private :
		int					_sock_fd;
		std::string			_nick_name;
		std::string			_user_name;
		std::string			_real_name;
		std::string			_hostname;
		std::string			_passwd;
		int					_registred;
		bool				_pass_valid;
		bool				_pass_status;
		bool				_user_status;
		bool				_nick_status;
		
};

