# ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <vector>
// #include "channel.hpp"

class channel;


typedef struct s_status
{
	bool	password;
	bool	nickname;
	bool	username;
	bool	registered;
}	t_status;

class client
{
	private:
		int							socket;

		std::vector<int>			clients_socket; // may be deleted
		std::vector<channel>		my_channels;

		std::string					username;
		std::string					nickname;
		std::string					hostname;
		std::string					realname;


	public:
		client();
		~client();
		client(int socket, std::vector<int>& clients_socket);

		int			getSocket();
		std::string	getUserName();
		std::string	getNickName();
		std::string	getHostName();


		void		setUserName(std::string s);
		void		setNickName(std::string s);
		void		setHostName(std::string s);

		void	reply(std::string message) const;

		void	registration(std::string line, std::string password, std::vector<client> cls);

		t_status					stat;
		int							steps;

		client	operator = (const client& cl);
		bool	operator==(const client& cl) const;
		// bool	operator!=(const client& cl) const;
		// bool	operator<(const client& cl) const;
		// bool	operator<=(const client& cl) const;
		// bool	operator>=(const client& cl) const;
		// bool	operator>(const client& cl) const;
};

std::vector<std::string> split(const std::string& input, char delimiter);

#endif