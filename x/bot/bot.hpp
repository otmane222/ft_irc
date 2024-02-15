#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fstream>
#include <random>


class	ircBot
{
	public:
		ircBot(int = 0, std::string = "", std::string = "");
		~ircBot();

		int		get_port() const;
		int		get_botSocke() const;
		std::string get_passwd() const;

		void	set_passwd(std::string &);
		void	set_port(int);
		void	set_botSocke(int);

		void		startBot();
	private :
		int						_botSocket;
		int						_port;
		std::string 			_passwd;
		std::string 			_hostname;

};


std::string			retreiveMessageFromBuffer(char *buffer);
std::string			retreiveNickFromBuffer(char *buffer);
std::vector<std::string>	splitString(std::string str, const char *delim);
int				botBehavior(std::string message, int botSocket, std::string nick);
void				rebuildMessage(std::vector<std::string> &vector, int index);

#endif