#ifndef BOT_HPP
# define BOT_HPP
# include <string>

# define LOCAL_HOST "127.0.0.1"
# define READ_SIZE	512 

using namespace::std;

typedef struct	s_bot
{
	int					fd_socket;
	int					port;
	string				server_reply;
	string				result;
	string 				password;
	string				channel;
	struct sockaddr_in	addr_socket;
}						t_bot;

#endif