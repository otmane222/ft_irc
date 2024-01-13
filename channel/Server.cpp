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
	std::string	s_targets;
	std::string	s_msg;
	std::map<std::string, int> targets;

	s_targets = get_token(param);
	s_msg = get_token(param);
	if (s_targets.empty() || s_msg.empty())
	{
		// reply with  ERR_NEEDMOREPARAMS (461)
		return;
	}
	std::stringstream str_stream(s_targets);
	std::string	target;
	while(!str_stream.eof())
	{
		getline(str_stream, target, ',');
		if (target[0] == '#')
			targets[target] = 1;
		else
			targets[target] = 0;
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
					// send private msg to all memeber
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
		}
		else // target is a client
		{
			if (client_exists(itr->first))
				// send private msg
			else
				// replyERR_NOSUCHNICK (401) 
		}
	}
}

void	Server::join(std::string param, Client & c)
{
	std::string	s_channels;
	std::string	s_keys;
	std::map<std::string, std::string> channels;
	s_channels = get_token(param);
	s_keys = get_token(param);
	std::stringstream channels_stream(s_channels);
	std::stringstream keys_stream(s_keys);
	std::string channel;
	std::string key;
	while (!channels_stream.eof())
	{
		getline(channels_stream, channel, ',');
		if (channel[0] != '#')
		{
			// reply with ERR_BADCHANMASK (476)
			continue;
		}
		getline(keys_stream, key, ',');
		if (channel_exists(channel))
		{
			Channel ch = get_channel_by_name(channel);
			ch.add_member(c, key);
		}
		else
		{
			Channel	ch(channel, key);
			if (!key.empty())
				ch.enable_mode(CH_KEY);
			ch.add_operator(c);
			_channels.push_back(ch);
			// reply
		}
	}
}

void	Server::kick(std::string param, Client &c)
{
	std::string	channel;
	std::string	s_users;
	std::string	comment;
	channel = get_token(param);
	s_users = get_token(param);
	if (channel.empty() || s_users.empty())
	{
		// send  ERR_NEEDMOREPARAMS (461)
		return;
	}
	if (!channel_exists(channel))
	{
		// reply with ERR_NOSUCHCHANNEL (403)
		return;
	}
	Channel	ch = get_channel_by_name(channel);
	int i = ch.is_member(c);
	if (i <= 0)
	{
		// reply with  ERR_CHANOPRIVSNEEDED (482) 
		return;
	}
	comment = get_token(param);
	if (comment.empty())
		comment = "apparently someone hates you";
	std::stringstream users_stream(s_users);
	std::string user;
	while (!users_stream.eof())
	{
		getline(users_stream, user, ',');
		Client cl = get_client_by_name(user);
		if (ch.is_member(cl) >= 0)
		{
			// send reply to client
			ch.remove_member(cl);
		}
		else
		{
			// reply with  ERR_USERNOTINCHANNEL (441) 

		}
	}
}
void	Server::invite(std::string param, Client &c)
{
	std::string	user = get_token(param);
	std::string	channel = get_token(param);
	if (channel.empty() || user.empty())
	{
		// send  ERR_NEEDMOREPARAMS (461)
		return;
	}
	if (!channel_exists(channel))
	{
		//  ERR_NOSUCHCHANNEL (403) 
		return;
	}
	Channel ch = get_channel_by_name(channel);
	if (ch.is_member(c) <= 0)
	{
		//  ERR_CHANOPRIVSNEEDED (482) 
		return;
	}
	ch.invite_member(c);
	//send reply
}
void	Server::topic(std::string param, Client &c)
{
	std::string			channel = get_token(param);
	std::string			topic = get_token(param);
	if (!channel_exists(channel))
	{
		// ERR_NOSUCHCHANNEL (403) 
		return;
	}
	Channel ch = get_channel_by_name(channel);
	int i = ch.is_member(c);
	if (i < 0)
	{
		//  ERR_NOTONCHANNEL (442) 
		return;
	}
	if (topic.empty())
	{
		if (i == 0)
		{
			//  ERR_CHANOPRIVSNEEDED (482) 
			return;
		}
		ch.set_topic(topic);
		// reply
	}
	else
	{
		topic = ch.get_topic();
		// reply
	}
}
void	Server::mode(std::string param, Client &c)
{
	
}

std::string	get_token(std::string& str)
{
	if (str.empty())
		return ("");
	std::string	ret;
	size_t	valid_c_position = str.find_first_not_of(' ');
	if (valid_c_position != std::string::npos)
	{
		str.erase(0, valid_c_position);
		if (str[0] == ':')
		{
			ret = str.substr(1);
			str.erase();
		}
		else
		{
			size_t	space_position = str.find_first_of(' ');
			if (space_position != std::string::npos)
			{
				ret = str.substr(0, space_position);
				str.erase(0, space_position);
			}
			else
			{
				ret = str;
				str.erase();
			}
		}
		return (ret);
	}
	return ("");
}

//hello 
//hello
//