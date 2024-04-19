#include "Server.hpp"

Server::Server(int	port, std::string passwd) :
_port(port), _passwd(passwd)
{
	_socket_fd = -1;
	_cmds["PASS"] = &Server::pass;
	_cmds["NICK"] = &Server::nick;
	_cmds["USER"] = &Server::user;
	_cmds["PRIVMSG"] = &Server::privmsg;
	_cmds["JOIN"] = &Server::join;
	_cmds["KICK"] = &Server::kick;
	_cmds["INVITE"] = &Server::invite;
	_cmds["TOPIC"] = &Server::topic;
	_cmds["MODE"] = &Server::mode;
	_cmds["PING"] = &Server::pong;
	_cmds["PONG"] = &Server::ping;
	_cmds["QUIT"] = &Server::quit;
}

Server::~Server(){}

int			Server::get_port() const {return _port;}
int			Server::get_socket_fd() const {return _socket_fd;}
std::string	Server::get_passwd() const {return _passwd;}

void	Server::set_port(int port) {_port = port;}
void	Server::set_socket_fd(int socket_fd) {_socket_fd= socket_fd;}
void	Server::set_passwd(std::string &passwd) {_passwd = passwd;}

bool	Server::channel_exists(Channel &ch) const
{
	if (find(_channels.begin(), _channels.end(), ch) == _channels.end())
		return false;
	return true;
}

bool	Server::channel_exists(const std::string &name)
{
	std::vector<Channel>::iterator itr = _channels.begin();
	while (itr != _channels.end())
	{
		if (itr->get_name() == name)
			return (true);
		itr++;
	}
	return (false);
}

Channel		&Server::get_channel_by_name(const std::string &name)
{
	std::vector<Channel>::iterator itr = _channels.begin();
	while (itr != _channels.end())
	{
		if (itr->get_name() == name)
			return (*itr);
		itr++;
	}
	throw (name.c_str());
}

bool		Server::client_exists(Client &c)
{
	if (find(_clients.begin(), _clients.end(), c) == _clients.end())
		return (false);
	return (true);
}

bool		Server::client_exists(const std::string &name)
{
	std::vector<Client>::iterator itr = _clients.begin();
	while (itr != _clients.end())
	{
		if (itr->get_nick_name() == name)
			return (true);
		itr++;
	}
	return (false);
}


Client		&Server::get_client_by_name(const std::string &name)
{
	std::vector<Client>::iterator itr = _clients.begin();
	while (itr != _clients.end())
	{
		if (itr->get_nick_name() == name)
			return (*itr);
		itr++;
	}
	throw ("Client does not exists");
}


Client		&Server::get_client_by_id(int sock_fd)
{
	std::vector<Client>::iterator itr = _clients.begin();
	while (itr != _clients.end())
	{
		if (itr->get_socket_fd() == sock_fd)
			return (*itr);
		itr++;
	}
	throw ("Client does not exists");
}

void	Server::reply(const Client &c, const std::string &msg) const
{
	send(c.get_socket_fd(), msg.c_str(), strlen(msg.c_str()), 0);
}

void	Server::pass(std::string param, Client &c)
{
	//    Command: PASS
	//   Parameters: <password>
	std::string	passwd = get_token(param);
	if (passwd.empty() && !_passwd.empty())
	{
		reply(c, ":irc.bmeek.chat 461 PASS :Not enough parameters\r\n");
		return;
	}
	if (c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 462 " + c.get_nick_name() + " :You may not reregister!\r\n");
		c.set_pass_stat(false);
		return;
	}
	if (passwd != _passwd)
	{
		reply(c, ":irc.bmeek.chat 464 :Password incorrect\r\n");
		return;
	}
	if (!c.get_pass_status())
	{
		c.set_passwd(passwd);
		c.set_pass_status(true);
	}
	if (c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 001 " + c.get_nick_name() + " Welcome to the gulag " + c.get_nick_name() + "@" + c.get_hostname() + "\r\n");
		reply(c, ":irc.bmeek.chat 002 " + c.get_nick_name() + " If you survive you earn your freedom\r\n");
		std::cout << "Client " + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " is athentificated" << std::endl;
	}
}

void	Server::nick(std::string param, Client &c)
{
	//      Command: NICK
	//   Parameters: <nickname>
	std::string	nick_name = get_token(param);
	if (nick_name.empty())
	{
		reply(c, ":irc.bmeek.chat 431 :No nickname given\r\n");
		return;
	}
	for  (size_t i = 0; i < nick_name.size(); i++)
	{
		if (std::isalnum(nick_name[i]) || nick_name[i] == '_')
			continue;
		else
		{
			reply(c, ":irc.bmeek.chat 432 " + nick_name + " :Erroneous nickname\r\n");
			return;
		}
	}
	std::vector<Client>::iterator it = _clients.begin();
	for (; it != _clients.end(); it++)
	{
		if ((*it).get_nick_name() == nick_name)
		{
			if ((*it) == c)
				return;
			return (reply(c, ":irc.bmeek.chat 433 " + nick_name + " :Nickname is already in use\r\n"));
		}
	}
	if (c.is_athantificated())
	{
		std::string	old_nick_name = c.get_nick_name();
		std::vector<Channel>::iterator itr = _channels.begin();
		while (itr != _channels.end())
		{
			if (itr->is_member(c) >= 0)
			{
				itr->broadcast(":" + old_nick_name + "!~" + c.get_user_name() + "@" + c.get_hostname() + " NICK :" + nick_name + "\r\n", c.get_nick_name());
			}
			itr++;
		}
		reply(c, ":" + old_nick_name + "!~" + c.get_user_name() + "@" + c.get_hostname() + " NICK :" + nick_name + "\r\n");
		c.set_nick_name(nick_name);
	}
	else
	{
		c.set_nick_name(nick_name);
		c.set_nick_status(true);
		if (c.is_athantificated())
		{
			reply(c, ":irc.bmeek.chat 001 " + c.get_nick_name() + " Welcome to the gulag " + c.get_nick_name() + "@" + c.get_hostname() + "\r\n");
			reply(c, ":irc.bmeek.chat 002 " + c.get_nick_name() + " If you survive you earn your freedom\r\n");
			std::cout << "Client " + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " is athentificated" << std::endl;
		}
	}
}

void	Server::user(std::string param, Client &c)
{
	//      Command: USER
	//   Parameters: <username> 0 * <realname>
	std::string	user_name;
	std::string	zero;
	std::string	asterisk;
	std::string	real_name;

	user_name = get_token(param);
	zero = get_token(param);
	asterisk = get_token(param);
	real_name = get_token(param);
	if (c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 462 " + c.get_nick_name() + " :You may not reregister!\r\n");
		return;
	}
	if (user_name.empty() || zero.empty() || asterisk.empty() || real_name.empty())
	{
		reply(c, ":irc.bmeek.chat 461 USER :Not enough parameters\r\n");
		return;
	}
	c.set_user_name(user_name);
	c.set_real_name(real_name);
	c.set_user_status(true);
	if (c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 001 " + c.get_nick_name() + " Welcome to the gulag " + c.get_nick_name() + "@" + c.get_hostname() + "\r\n");
		reply(c, ":irc.bmeek.chat 002 " + c.get_nick_name() + " If you survive you earn your freedom\r\n");
		std::cout << "Client " + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " is athentificated" << std::endl;
	}
}

void	Server::privmsg(std::string param, Client &c)
{
	//      Command: PRIVMSG
	//   Parameters: <target>{,<target>} <text to be sent>
	std::string	s_targets;
	std::string	s_msg;
	std::vector<std::string> processed_targets;
	std::map<std::string, int> targets;
	if (!c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 451 " + c.get_nick_name() + " :You have not registered !\r\n");
		return;
	}
	s_targets = get_token(param);
	s_msg = get_token(param);
	if (s_targets.empty())
	{
		reply(c, ":irc.bmeek.chat 411 " + c.get_nick_name() + " :No recipient given (PRIVMSG)!\r\n");
		return;
	}
	if (s_msg.empty())
	{
		reply(c, ":irc.bmeek.chat 412 " + c.get_nick_name() + " :No text to send\r\n");
		// reply(c, ":irc.bmeek.chat 461 " + c.get_nick_name() + " :No text to send\r\n");
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
		if (find(processed_targets.begin(), processed_targets.end(), itr->first) != processed_targets.end())
		{
			itr++;
			continue;
		}else
		{
			processed_targets.push_back(itr->first);
		}
		if (itr->second == 1) // target is a channel
		{
			if (channel_exists(itr->first))
			{
				Channel & ch = get_channel_by_name(itr->first);
				if (ch.is_member(c) != -1)
				{
					ch.broadcast(":" + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " PRIVMSG " + itr->first + " :" + s_msg + "\r\n", c.get_nick_name());
				}
				else
				{
					reply(c, ":irc.bmeek.chat 404 " + c.get_nick_name() + " " + itr->first + " :Cannot send to nick/channel\r\n");
				}
			}
			else
			{
				reply(c, ":irc.bmeek.chat 401 " + c.get_nick_name() + " " + itr->first + " :No such nick/channel\r\n");
			}
		}
		else // target is a client
		{
			if (client_exists(itr->first))
			{
				Client c2 = get_client_by_name(itr->first);
				reply(c2, ":" + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " PRIVMSG " + c2.get_nick_name() + " :" + s_msg + "\r\n");
			}
			else
			{
				reply(c, ":irc.bmeek.chat 401 " + c.get_nick_name() + " " + itr->first + " :No such nick/channel\r\n");
			}
		}
		itr++;
	}
}

void	Server::join(std::string param, Client & c)
{
	//      Command: JOIN
	//   Parameters: <channel>{,<channel>} [<key>{,<key>}]
	if (!c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 451 " + c.get_nick_name() + " :You have not registered !\r\n");
		return;
	}
	std::string	s_channels;
	std::string	s_keys;
	std::map<std::string, std::string> channels;
	s_channels = get_token(param);
	s_keys = get_token(param);
	if (s_channels.empty() || s_channels == "#")
	{
		reply(c, ":irc.bmeek.chat 461 " + c.get_nick_name() + " JOIN :Not enough parameters\r\n");
		return;
	}
	std::stringstream channels_stream(s_channels);
	std::stringstream keys_stream(s_keys);
	std::string channel;
	std::string key;
	while (!channels_stream.eof())
	{
		getline(channels_stream, channel, ',');
		if (channel[0] != '#')
		{
			reply(c, ":irc.bmeek.chat 476 " + c.get_nick_name() + " " + channel + " :Invalid channel name.\r\n");
			continue;
		}
		getline(keys_stream, key, ',');
		if (channel_exists(channel))
		{
			Channel &ch = get_channel_by_name(channel);
			try
			{
				ch.add_member(c, key);
			}
			catch(const int & i)
			{
				switch (i)
				{
					case 1 :
						break;
					case 2 :
						reply(c, ":irc.bmeek.chat 471 " + c.get_nick_name() + " " + channel + " :Cannot join channel (+l) - channel is full, try again later\r\n");
						break;
					case 3 :
						reply(c, ":irc.bmeek.chat 473 " + c.get_nick_name() + " " + channel + " :Cannot join channel (+i) - you must be invited\r\n");
						break;
					case 4 :
						reply(c, ":irc.bmeek.chat 475 " + c.get_nick_name() + " " + channel + " :Cannot join channel (+k) - bad key\r\n");
				}
				return;
			}
			reply(c, ":irc.bmeek.chat 311 " + c.get_nick_name() + " " + channel + " :" + ch.get_topic() + "\r\n");
			reply(c, ":" + c.get_nick_name() + "!" + c.get_user_name() + "@" + c.get_hostname() + " JOIN " + channel + " " + ch.get_mode() +  "\r\n");
			ch.broadcast(":" + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " JOIN " +channel + " " + ch.get_mode() + "\r\n", c.get_nick_name());
			reply(c, ":irc.bmeel.chat 353 " + c.get_nick_name() + " @ " + channel + " :" + ch.get_list_of_names() +"\r\n");
			reply(c, ":irc.bmeel.chat 366 " +c.get_nick_name() + " " + channel + " :End of /NAMES list.\r\n");
		}
		else
		{
			Channel	ch(channel, key);
			if (!key.empty())
				ch.enable_mode(CH_KEY);
			ch.add_operator(c);
			_channels.push_back(ch);
			reply(c, ":irc.bmeek.chat 311 " + c.get_nick_name() + " " + channel + " :no topic is set\r\n");
			reply(c, ":" + c.get_nick_name() + "!" + c.get_user_name() + "@" + c.get_hostname() + " JOIN " + " " + ch.get_mode() + channel + "\r\n");
			reply(c, ":irc.bmeel.chat 353 " + c.get_nick_name() + " @ " + channel + " :" + ch.get_list_of_names() +"\r\n");
			reply(c, ":irc.bmeel.chat 366 " +c.get_nick_name() + " " + channel + " :End of /NAMES list.\r\n");
		}
	}
}

void	Server::kick(std::string param, Client &c)
{
	//       Command: KICK
	//    Parameters: <channel> <user> *( "," <user> ) [<comment>]
	if (!c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 451 " + c.get_nick_name() + " :You have not registered !\r\n");
		return;
	}
	std::string	channel;
	std::string	s_users;
	std::string	comment;
	channel = get_token(param);
	s_users = get_token(param);
	if (channel.empty() || s_users.empty())
	{
		reply(c, ":irc.bmeek.chat 461 " + c.get_nick_name() + " KICK :Not enough parameters\r\n");
		return;
	}
	if (!channel_exists(channel))
	{
		reply(c, ":irc.bmeek.chat 403 " + c.get_nick_name() + " " + channel + " :No such channel\r\n");
		return;
	}
	Channel	&ch = get_channel_by_name(channel);
	int i = ch.is_member(c);
	if (i == 0)
	{
		reply(c, ":irc.bmeek.chat 482 " + c.get_nick_name() + " " + channel + " :You're not channel operator\r\n");
		return;
	}
	if (i == -1)
	{
		reply(c, "irc.bmeek.chat 442 " + c.get_nick_name() + " " + channel + " You're not on that channl\r\n");
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
		try
		{
			Client& cl = get_client_by_name(user);
			if (ch.is_member(cl) >= 0)
			{
				ch.broadcast(":" + c.get_nick_name() + "!" + c.get_user_name() + "@" + c.get_hostname() + " KICK " + channel + " " + cl.get_nick_name() + " :" + comment + "\r\n", c.get_nick_name());
				reply(c, ":" + c.get_nick_name() + "!" + c.get_user_name() + "@" + c.get_hostname() + " KICK " + channel + " " + cl.get_nick_name() + " :" + comment + "\r\n");
				ch.remove_member(cl);
				ch.remove_member_from_invited_list(cl);
				if (ch.get_nbr_members() <= 0)
				{
					remove_channel(ch);
				}
			}
			else
			{
				reply(c, ":irc.bmeek.chat 441 " + c.get_nick_name() + " " + cl.get_nick_name() + " " + channel + " :They aren't on that channel\r\n");
			}
		}
		catch (const char *)
		{
			reply(c, ":irc.bmeek.chat 401 " + c.get_nick_name() + " " + user + " :No such nick/channel\r\n");
		}
	}
}

void	Server::invite(std::string param, Client &c)
{
	//      Command: INVITE
	//   Parameters: <nickname> <channel>
	if (!c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 451 " + c.get_nick_name() + " :You have not registered !\r\n");
		return;
	}
	std::string	user = get_token(param);
	std::string	channel = get_token(param);
	if (channel.empty() || user.empty())
	{
		reply(c, ":irc.bmeek.chat 461 " + c.get_nick_name() + " INVITE " + " :Not enough parameters\r\n");
		return;
	}
	if (!client_exists(user))
	{
		reply(c, ":irc.bmeek.chat 401 " + c.get_nick_name() + " " + user + " :No such nick/channel\r\n");
		return;
	}
	if (!channel_exists(channel))
	{
		reply(c, ":irc.bmeek.chat 403 " + c.get_nick_name() + " " + channel + " :No such channel\r\n");
		return;
	}
	Channel& ch = get_channel_by_name(channel);
	if (ch.is_member(c) == 0)
	{
		reply (c, ":irc.bmeek.chat 482 " + c.get_nick_name() + " " + channel + ":You're not a channel operator\r\n");
		return;	
	}
	if (ch.is_member(c) == -1)
	{
		reply (c, ":irc.bmeek.chat 442 " + c.get_nick_name() + " " + channel + ":You're not on that channel\r\n");
		return;	
	}
	Client& cl = get_client_by_name(user);
	if (ch.is_member(cl) != -1)
	{
		reply(c, ":irc.bmeek.chat 443 " + c.get_nick_name() + " " + user + " " + channel + " :is already on channel\r\n");
		return;
	}
	ch.invite_member(cl);
	reply(c, ":irc.bmeek.chat 341 " + c.get_nick_name() + " " + user + " " + channel + "\r\n");
	reply(cl, ":" + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " INVITE " + user + " :" + channel + "\r\n");
	return;
}


void	Server::topic(std::string param, Client &c)
{
	//      Command: TOPIC
	//   Parameters: <channel> [<topic>]
	if (!c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 451 " + c.get_nick_name() + " :You have not registered !\r\n");
		return;
	}
	std::string			channel = get_token(param);
	std::string			topic = get_token(param);
	if (channel.empty())
	{
		reply(c, ":irc.libera.chat 461 " + c.get_nick_name() + " TOPIC :Not enough parameters\r\n");
		return;
	}
	if (!channel_exists(channel))
	{
		reply(c, ":irc.bmeek.chat 403 " + c.get_nick_name() + " " + channel + " :No suck channel\r\n");
		return;
	}
	Channel& ch = get_channel_by_name(channel);
	int i = ch.is_member(c);
	if (i < 0)
	{
		reply(c, ":irc.bmeek.chat 442 " + c.get_nick_name() + " " + channel + " :You're not on that channel\r\n");
		return;
	}
	if (!topic.empty())
	{
		if (i == 0 && ch.is_mode_enabled(CH_PROTECTED_TOPIC))
		{
			reply(c, ":irc.bmeek.chat 482 " + c.get_nick_name() + " " + channel + " :You're not a channel operator\r\n");
			return;
		}
		ch.set_topic(topic);
		reply(c, ":" + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " TOPIC " + channel + " :" + topic + "\r\n");
		ch.broadcast(":" + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " TOPIC " + channel + " :" + topic + "\r\n", c.get_nick_name());
		return;
	}
	else
	{
		topic = ch.get_topic();
		if (topic.empty())
			topic = "no topic set";
		reply(c, ":irc.bmeek.chat 332 " + c.get_nick_name() + " " + channel + " :" + topic + "\r\n");
		return;
	}
}


void	Server::mode(std::string param, Client &c)
{
	//      Command: MODE
	//   Parameters: <target> [<modestring> [<mode arguments>...]]
	//	modestring i t k o l
	if (!c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 451 " + c.get_nick_name() + " :You have not registered !\r\n");
		return;
	}
	std::string	target;
	std::string	modestring;
	std::string	made_arg;
	std::map<mode_t, char> modes;
	target = get_token(param);
	if (target.empty())
	{
		reply(c, ":irc.bmeek.chat 461 " + c.get_nick_name() + " MODE :Not enough parameters\r\n");
	}
	else
	{
		try
		{
			Channel &ch = get_channel_by_name(target);
			modestring = get_token(param);
			if (modestring.empty())
			{
				reply(c, ":irc.bmeek.chat 324 " + c.get_nick_name() + " " + target + " " + ch.get_mode() + "\r\n");
			}
			else
			{
				int	flag = 1;// o : disable , 1 : enable
				unsigned int size = modestring.size();
				for (unsigned int  i = 0; i < size; i++)
				{
					if (strchr("+-itkol", modestring[i]) == NULL)
					{
						reply(c, ":irc.bmeek.chat 472 " + c.get_nick_name() + " " + modestring[i] + " :is an unknown mode char to me\r\n");
						return;
					}
					else
					{
						int i_c = ch.is_member(c);
						if (i_c != 1)
						{
							reply(c, ":irc.bmeek.chat 482 " + c.get_nick_name() + " " + target + " :You're not a channel operator\r\n");
							return;
						}
						if (modestring[i] == '+')
						{
							flag = 1;
						}
						else if (modestring[i] == '-')
						{
							flag = 0;
						}
						else if (modestring[i] == 'i')
						{
							if (flag == 1)
							{
								ch.enable_mode(CH_INVITE_ONLY);
							}else
							{
								ch.disable_mode(CH_INVITE_ONLY);
							}
						}
						else if (modestring[i] == 't')
						{
							if (flag == 1)
							{
								ch.enable_mode(CH_PROTECTED_TOPIC);
							}else
							{
								ch.disable_mode(CH_PROTECTED_TOPIC);
							}
						}
						else if (modestring[i] == 'k')
						{
							if (flag == 1)
							{
								std::string key = get_token(param);
								if (key.empty())
								{
									reply(c, ":irc.bmeek.chat 461 " + c.get_user_name() + " MODE :Not enough parameters\r\n");
									return;
								}
								ch.enable_mode(CH_KEY);
								ch.set_passwd(key);
							}else
							{
								ch.disable_mode(CH_KEY);
							}
						}
						else if (modestring[i] == 'o')
						{
							std::string c_name = get_token(param);
							if (c_name.empty())
							{
								reply(c, ":irc.bmeek.chat 461 " + c.get_user_name() + " MODE :Not enough parameters\r\n");
								return;
							}
							if (flag == 1)
							{
								ch.promote_member(c_name);
							}else
							{
								ch.unpromote_member(c_name);
							}
						}
						else // l
						{
							if (flag == 1)
							{
								std::string limit = get_token(param);
								if (limit.empty())
								{
									reply(c, ":irc.bmeek.chat 461 " + c.get_user_name() + " MODE :Not enough parameters\r\n");
									return;
								}
								int l = atoi(limit.c_str());
								if (l > 0)
								{
									ch.enable_mode(CH_CLIENT_LIMIT);
									ch.set_max_members(l);
								}
							}else
							{
								ch.disable_mode(CH_CLIENT_LIMIT);
							}
						}
					}
				}
				reply(c, ":" + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " MODE " + target + " " + ch.get_mode() + "\r\n");
				ch.broadcast(":" + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() +  " MODE " + target + " " + ch.get_mode() + "\r\n", c.get_nick_name());
			}
		}
		catch (const char *)
		{
			reply(c, ":irc.bmeek.chat 401 " + c.get_nick_name() + " " + target + " :No such nick/channel\r\n");
		}
	}
}

void		Server::ping(std::string param, Client &c){(void) param; (void) c;}
void		Server::pong(std::string param, Client &c){(void) param; (void) c;}
void		Server::quit(std::string param, Client &c)
{
	//     Command: QUIT
	(void) param;
	reply(c,":" + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " QUIT :Client Quit\r\n");
	std::vector<Channel>::iterator itr = _channels.begin();
	while (itr != _channels.end())
	{
		if (itr->is_member(c) >= 0)
		{
			itr->remove_member(c);
			if (itr->get_nbr_members() == 0)
			{
				_channels.erase(itr);
				continue;
			}
		}
		itr++;
	}
	close(c.get_socket_fd());
	std::vector<Client>::iterator it = _clients.begin();
	int i = 0;
	for (; it != _clients.end(); it++)
	{
		if (_clients[i] == c)
			break ;
		i++;
	}
	if (it != _clients.end())
	{
		std::cout << "Client " << c.get_socket_fd() - 3 << " Disconnected" << std::endl;
		_clients.erase(_clients.begin() + i);
		pollfds.erase(pollfds.begin() + i + 1);
	}
}


void		Server::execute_cmd(std::string msg, Client &c)
{
	if (msg.back() == '\n')
		msg.pop_back();
	if (msg.back() == '\r')
		msg.pop_back();
	if (msg.size() > MAX_LEN)
	{
		reply(c, ":irc.bmeek.chat 417 " + c.get_nick_name() + " :Input line was too long !\r\n");
		return;
	}
	std::string cmd = get_token(msg);
	if (cmd.empty() || cmd == "\r\n")
		return;
	for (size_t i = 0; i < cmd.size(); i++)
		cmd[i] = toupper(cmd[i]);
	if (this->_cmds.find(cmd) != this->_cmds.end())
		(this->*_cmds[cmd])(msg, c);
	else
	{
		reply(c, ":irc.bmeek.chat 421 " + c.get_nick_name() + " " + cmd + " :Unknown command\r\n");
		return;
	}
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

std::string	recving(int fd)
{
	char	buffer[512];
	std::string line = "";
	ssize_t	bufferr;

	std::memset(buffer, 0, sizeof(buffer));
	bufferr = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bufferr == -1)
	{
		return ("");
	}
	line.append(buffer);
	return (line);
}

void eraseSubstring(std::string& str)
{
	size_t pos = str.find("\r\n");
	if (pos != std::string::npos) {
		str.erase(0, pos + 2);
	}
}

void eraseSubstring2(std::string& str)
{
	size_t pos = str.find("\r\n");
	if (pos != std::string::npos) {
		str.erase(pos + 2, str.length());
	}
}

void		Server::start()
{
	int										_clientSocket;
	struct sockaddr_in						clientaddr;
	socklen_t								len = sizeof(clientaddr);
	pollfd Pollfd;

	Pollfd.fd = _socket_fd;
	Pollfd.events = POLLHUP|POLLIN;
	Pollfd.revents = 0;

	pollfds.push_back(Pollfd);
	while (true)
	{
		if (poll(pollfds.data(), pollfds.size(), -1) == -1)
			throw "Error polling";
		for (size_t i = 0; i < pollfds.size(); i++) // EAGAIN is an erno 
		{
			if ((pollfds[i].revents & POLLHUP) == POLLHUP)
			{
				std::cout << "Client " << pollfds[i].fd - 3 << " disconnected" << std::endl;
				get_client_by_id(pollfds[i].fd).close_connection();
				pollfds.erase(pollfds.begin() + i);
				_clients.erase(_clients.begin() + i - 1);
				break ;
			}
			if ((pollfds[i].revents & POLLIN) == POLLIN)
			{
				if (i == 0)
				{
					_clientSocket = accept(_socket_fd, reinterpret_cast<struct sockaddr*>(&clientaddr), &len);
					if (_clientSocket == -1)
						continue ;
					else
					{
						char* ip_address = inet_ntoa(clientaddr.sin_addr);

						if (fcntl(_clientSocket, F_SETFL, O_NONBLOCK) == -1)
						{
							perror("fcntl");
							exit (1);
						}
						std::cout << "Client " << _clientSocket - 3 << " is connected" << std::endl;
						// std::cout << "New Connection Has Been Accepted, Client Socket: "<< _clientSocket - 3 << std::endl;
						Pollfd.fd = _clientSocket;
						pollfds.push_back(Pollfd);


						Client	a(_clientSocket, ip_address);
						_clients.push_back(a);
					}
				}
				else
				{
					Client&		 cl = get_client_by_id(pollfds[i].fd);
					std::string cl_line = cl.get_line_read();
				

					std::string	line = recving(pollfds[i].fd);

					cl_line.append(line.c_str());
					cl.set_line(cl_line);

					while (std::strstr(cl_line.c_str(), "\r\n"))
					{

						std::string kaka = cl_line;

						eraseSubstring2(kaka);

						execute_cmd(kaka, cl);

						eraseSubstring(cl_line);

						cl.set_line(cl_line);
					}
				}
			}
		}
	}
}

int			ft_atoi(const char * str)
{
	int	sign;
	int	n;

	sign = 1;
	n = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	while (*str >= '0' && *str <= '9')
	{
		n = n * 10 + (*str - '0');
		str++;
		if (n > 65535)
			throw(2);
	}
	if (*str)
		throw (3);
	if (n < 1024)
		throw(1);
	return (sign * n);
}


void	Server::remove_channel(const Channel & ch)
{
	std::vector<Channel>::iterator itr = find(_channels.begin(), _channels.end(), ch);
	if (itr != _channels.end())
	{
		_channels.erase(itr);
	}
}