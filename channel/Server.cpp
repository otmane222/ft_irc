// Server.cpp file
#include "Server.hpp"

Server::Server(int	port, std::string passwd) :
_port(port), _passwd(passwd){}

Server::~Server(){}

int			Server::get_port() const {return _port;}
std::string	Server::get_passwd() const {return _passwd;}

void	Server::set_port(int port) {_port = port;}
void	Server::set_passwd(std::string &passwd) {_passwd = passwd;}

// void	Server::pass(std::string param)
// {
// 	std::stringstream	strstream(param);
// 	std::string			pass;
// 	getline(strstream, pass, ' ');

// }

// void	Server::nick(std::string param)
// {
// 	std::stringstream	strstream(param);
// 	std::string			nick;
// 	getline(strstream, nick, ' ');

// }

// void	Server::user(std::string param)
// {
	
// }

void	Server::privmsg(std::string param, Client &c)
{
	std::stringstream			strstream(param);
	std::map<std::string, int>	targets;
	std::string					msg;
	std::string					target;
	getline(strstream, target, ' ');
	getline(strstream, msg, '\r');
	std::stringstream			strstream2(target);
	while(!strstream2.eof())
	{
		std::string	str;
		getline(strstream2, str, ',');
		if (str[0] == '#')
			targets[str] = 1;
		else
			targets[str] = 0; 
	}
	if (msg[0] != ':')
	{
		size_t found = msg.find(" ");
		if (found!=std::string::npos)
			msg = msg.substr(0, found);
	}
	std::map<std::string, int>::iterator itr = targets.begin();
	while (itr != targets.end())
	{
		if (itr->second == 1) // target is a channel
		{
			if (channel_exists(itr->first))
			{
				Channel ch = get_channel_by_name(itr->first);
				if (ch.is_member(c))
				{
					// broadcast REP_AWAY
				}
				else
				{
					// reply with ERR_CANNOTSENDTOCHAN (404)
				}
			}
			else
			{
				// reply with ERR_NOSUCHCHANNEL (403)
			}
		}else // target is a client
		{
			if (client_exists(itr->first))
			{
				// reply with REP_AWAY
			}
			else
			{
				// replyERR_NOSUCHNICK (401) 
			}
		}
	}
}
void	Server::join(std::string param, Client & c)
{
	std::stringstream			strstream(param);
	std::vector<std::string>	v_channels;
	std::vector<std::string>	v_keys;
	std::string					s_channels;
	std::string					s_keys;
	getline(strstream, s_channels, ' ');
	getline(strstream, s_keys, ' ');
	std::stringstream			channel_stream(s_channels);
	std::stringstream			key_stream(s_keys);
	while(channel_stream.eof())
	{
		std::string	ch;
		getline(channel_stream, ch, ',');
		v_channels.push_back(ch);
	}
	while(key_stream.eof())
	{
		std::string	k;
		getline(key_stream, k, ',');
		v_channels.push_back(k);
	}
}
void	Server::kick(std::string param)
{
	std::string					channel;
	std::vector<std::string>	targets;
	std::string					reason;
	std::stringstream			str_stream(param);
	std::string					s_target;
	std::string					s_reason;
	getline(str_stream, channel, ' ');
	getline(str_stream, s_target, ' ');
	getline(str_stream, s_reason, '\r');
	std::stringstream			target_stream(s_target);
	while(!target_stream.eof())
	{
		std::string	t;
		getline(target_stream, t, ',');
		targets.push_back(t);
	}
	if (s_reason[0] != ':')
	{
		size_t found = s_reason.find(" ");
		if (found!=std::string::npos)
			s_reason = s_reason.substr(0, found);
	}
}
void	Server::invite(std::string param)
{
	std::string			nich_name;
	std::string			channel;
	std::stringstream	str_stream(param);
	getline(str_stream, nich_name, ' ');
	getline(str_stream, channel, ' ');

}
void	Server::topic(std::string param)
{
	std::stringstream	str_stream(param);
	std::string			channel;
	std::string			topic;
	getline(str_stream, channel, ' ');
	getline(str_stream, topic, '\r');
	if (topic[0] != ':')
	{
		size_t found = topic.find(" ");
		if (found!=std::string::npos)
			topic = topic.substr(0, found);
	}
}
void	Server::mode(std::string param)
{
	
}