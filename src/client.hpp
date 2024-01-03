#pragma once

#include <iostream>
#include <vector>

class channel;

class client
{
	private:
		int							socket;

		std::vector<int>			clients_socket; // may be deleted
		std::vector<channel>		my_channels;

		std::string					username;
		std::string					nickname;
		std::string					hostname;

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

		void	reply(std::string& message) const;

		bool				valid;
		int					steps;

		client	operator = (const client& cl);
		bool	operator==(const client& cl) const;
};
