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
	// _cmd["PART"] == &Server::part;
	// _cmds["PING"] = &Server::;
	// _cmds["PONG"] = &Server::;
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
	ssize_t	count = send(c.get_socket_fd(), msg.c_str(), strlen(msg.c_str()), 0);
	if (count == -1)
		std::cerr << "failure" << std::endl;
}
//    Command: PASS
//   Parameters: <password>
void	Server::pass(std::string param, Client &c)
{
	std::string	passwd = get_token(param);
	if (passwd.empty())
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
	std::cout << "password accepted" << std::endl;
	if (c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 001 " + c.get_nick_name() + " Welcome to the gulag\r\n");
		reply(c, ":irc.bmeek.chat 002 " + c.get_nick_name() + " If you survive you earn your freedom\r\n");
	}
}

//      Command: NICK
//   Parameters: <nickname>
void	Server::nick(std::string param, Client &c)
{
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
	// xhexk if the nick name already in use
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
				// reply(c, ":" + old_nick_name + "!~" + c.get_user_name() + "@" + c.get_hostname() + " NICK :" + nick_name + "\r\n");
				itr->broadcast(":" + old_nick_name + "!~" + c.get_user_name() + "@" + c.get_hostname() + " NICK :" + nick_name + "\r\n", c.get_nick_name());
				// std::map<Client, int> m = itr->get_members();
				// std::map<Client, int>::iterator m_itr = m.begin();
				// while (m_itr != m.end())
				// {
				// 	if (m_itr->first == c)
				// 		continue;
				// 		// :oho!~obahi@197.230.30.146 NICK :bbbbb
				// 	m_itr++;
				// }
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
			reply(c, ":irc.bmeek.chat 001 " + c.get_nick_name() + " Welcome to the gulag\r\n");
			reply(c, ":irc.bmeek.chat 002 " + c.get_nick_name() + " If you survive you earn your freedom\r\n");
		}
	}
	std:: cout << "nick accepted"<<std::endl;
}

//      Command: USER
//   Parameters: <username> 0 * <realname>
void	Server::user(std::string param, Client &c)
{
	std::string	user_name;
	std::string	zero;
	std::string	asterisk;
	std::string	real_name;

	user_name = get_token(param);
	std::cout << "user name : " << user_name << std::endl;
	zero = get_token(param);
	asterisk = get_token(param);
	real_name = get_token(param);
	std::cout << "real name : " << real_name << std::endl;
	if (c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 462 " + c.get_nick_name() + " :You may not reregister!\r\n");
		return;
	}
	if (user_name.empty() || zero.empty() || asterisk.empty() || real_name.empty())
	{
		reply(c, ":irc.bmeek.chat 461 * USER :Not enough parameters\r\n");
		return;
	} 
	c.set_user_name(user_name);
	c.set_real_name(real_name);
	c.set_user_status(true);
	if (c.is_athantificated())
	{
		reply(c, ":irc.bmeek.chat 001 " + c.get_nick_name() + " Welcome to the gulag\r\n");
		reply(c, ":irc.bmeek.chat 002 " + c.get_nick_name() + " If you survive you earn your freedom\r\n");
	}
	std::cout << "username accepted" << std::endl;
}


//      Command: PRIVMSG
//   Parameters: <target>{,<target>} <text to be sent>
void	Server::privmsg(std::string param, Client &c)
{
	std::cout << "privmsg is being executed" << std::endl;
	std::string	s_targets;
	std::string	s_msg;
	std::vector<std::string> processed_targets;
	std::map<std::string, int> targets;
	s_targets = get_token(param);
	s_msg = get_token(param);
	if (s_targets.empty())
	{
		std::cout << "target is empty" << std::endl;
		// reply with  ERR_NEEDMOREPARAMS (461)
		reply(c, ":irc.bmeek.chat 411 " + c.get_nick_name() + " :No recipient given (<PRIVMSG>)\r\n");
		// reply(c, ":irc.bmeek.chat 461 * USER :Not enough parameters\r\n");
		// reply(c, ERR_NEEDMOREPARAMS(c.get_nick_name(), c.get_hostname()));
		return;
	}
	if (s_msg.empty())
	{
		std::cout << "msg is empty" << std::endl;
		reply(c, ":irc.bmeek.chat 412 " + c.get_nick_name() + ":No text to send\r\n");
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
				Channel ch = get_channel_by_name(itr->first);
				if (ch.is_member(c) != -1)
				{
					std::cout << c.get_nick_name() << " is a member of " << ch.get_name() << std::endl;
					Channel ch = get_channel_by_name(itr->first);
					ch.broadcast(":" + c.get_nick_name() + "!~" + c.get_user_name() + "@" + c.get_hostname() + " PRIVMSG " + itr->first + " :" + s_msg + "\r\n", c.get_nick_name());
				}
				else
				{
					reply(c, ":irc.bmeek.chat 404 " + c.get_nick_name() + " " + itr->first + " :Cannot send to nick/channel\r\n");
					std::cout << "cannot sent to channel : " << itr->first << std::endl;
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
				reply(c2, ":" + c.get_nick_name() + "!~" + c.get_user_name() + "@127.0.0.1" + " PRIVMSG " + c2.get_nick_name() + " :" + s_msg + "\r\n");
				// reply(c2, RPL_PRIVMSG(get_source(c.get_nick_name(), c.get_real_name(), c.get_hostname()) , itr->first, s_msg));
			}
			else
			{
				reply(c, ":irc.bmeek.chat 401 " + c.get_nick_name() + " " + itr->first + " :No such nick/channel\r\n");
			}
		}
		itr++;
	}
}

//      Command: JOIN
//   Parameters: <channel>{,<channel>} [<key>{,<key>}]
//   Alt Params: 0
void	Server::join(std::string param, Client & c)
{
	if (!c.is_athantificated())
		return ;
	std::cout << "join" << std::endl;
	std::string	s_channels;
	std::string	s_keys;
	std::map<std::string, std::string> channels;
	s_channels = get_token(param);
	s_keys = get_token(param);
	if (s_channels.empty() || s_channels == "#")
	{
		std::cout << "no channel name is being provided" << std::endl;
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
			// reply(c, ERR_BADCHANNELNAME(c.get_nick_name(), c.get_hostname(), channel));
			// std::cout << ERR_BADCHANNELNAME(c.get_nick_name(), c.get_hostname(), channel) << std::endl;
			continue;
		}
		getline(keys_stream, key, ',');
		if (channel_exists(channel))
		{
			std::cout << "channel exists :" << channel << std::endl;
			Channel &ch = get_channel_by_name(channel);
			std::cout << "###############Befor###############" << std::endl;
			std::cout << "number of members : " << ch.get_nbr_members() << std::endl;
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
						reply(c, ":irc.bmeek.chat 471 " + c.get_nick_name() + " " + channel + " :Cannot join channel (+i) - you must be invited\r\n");
						break;
					case 4 :
						reply(c, ":irc.bmeek.chat 475 " + c.get_nick_name() + " " + channel + " :Cannot join channel (+k) - bad key\r\n");
				}
				return;
			}
			reply(c, ":irc.bmeek.chat 311 " + c.get_nick_name() + " " + channel + " :no topic is set\r\n");
			// reply(c, c.get_nick_name() + "!~" + c.get_user_name() + "@" + "127.0.0.1" + " JOIN " +channel + "\r\n");
			reply(c, ":" + c.get_nick_name() + "!" + c.get_user_name() + "@127.0.0.1" + " JOIN " + channel + "\r\n");
			ch.broadcast(c.get_nick_name() + "!~" + c.get_user_name() + "@" + "127.0.0.1" + " JOIN " +channel + "\r\n", c.get_nick_name());
			reply(c, ":irc.bmeel.chat 353 " + c.get_nick_name() + " @ " + "" + channel + ":" + ch.get_list_of_names() +"\r\n");
			reply(c, ":irc.bmeel.chat 366 " +c.get_nick_name() + " " + channel + " :End of /NAMES list.\r\n");
			std::cout << "###############After###############" << std::endl;
			std::cout << "number of members : " << ch.get_nbr_members() << std::endl;
		}
		else
		{
			std::cout << "channel does not exists :" << channel << std::endl;
			Channel	ch(channel, key);
			if (!key.empty())
				ch.enable_mode(CH_KEY);
			ch.add_operator(c);
			_channels.push_back(ch);

			// reply(c, ":irc.bmeel.chat MODE " + channel + " +Cnst\r\n");
			// :server1 331 bmeek #c1 :No topic is set
			reply(c, ":irc.bmeek.chat 311 " + c.get_nick_name() + " " + channel + " :no topic is set\r\n");
			// reply(c, c.get_nick_name() + "!~" + c.get_user_name() + "@" + "127.0.0.1" + " JOIN " +channel + "\r\n");
			reply(c, ":" + c.get_nick_name() + "!" + c.get_user_name() + "@127.0.0.1" + " JOIN " + channel + "\r\n");
			reply(c, ":irc.bmeel.chat 353 " + c.get_nick_name() + " @ " + channel + ":" + ch.get_list_of_names() +"\r\n");
			reply(c, ":irc.bmeel.chat 366 " +c.get_nick_name() + " " + channel + " :End of /NAMES list.\r\n");
			
			// reply
		}
	}
}

//       Command: KICK
//    Parameters: <channel> <user> *( "," <user> ) [<comment>]
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
		// :server1 461 bmeek KICK :Not enough parameters
		reply(c, ":irc.bmeek.chat 461 " + c.get_nick_name() + " KICK :Not enough parameters\r\n");
		return;
	}
	if (!channel_exists(channel))
	{
		// reply with ERR_NOSUCHCHANNEL (403)
		// :server1 403 bmeek #CC :No such channel
		reply(c, ":irc.bmeek.chat 403 " + c.get_nick_name() + " " + channel + " :No such channel\r\n");
		return;
	}
	Channel	&ch = get_channel_by_name(channel);
	int i = ch.is_member(c);
	if (i == 0)
	{
		// :server1 482 loki #c ::You're not channel operator
		reply(c, ":irc.bmeek.chat 482 " + c.get_nick_name() + " " + channel + " :You're not channel operator\r\n");
		return;
	}
	if (i == -1)
	{
		//:erbium.libera.chat 442 hulk #clcl :You're not on that channel
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
				//:lokii!~loki@197.230.30.146 KICK #clcl hiba :jdfks skvhfs
				ch.broadcast(":" + c.get_nick_name() + "!" + c.get_user_name() + "@127.0.0.1" + " KICK " + channel + " " + cl.get_nick_name() + " :" + comment + "\r\n", c.get_nick_name());
				reply(c, ":" + c.get_nick_name() + "!" + c.get_user_name() + "@127.0.0.1" + " KICK " + channel + " " + cl.get_nick_name() + " :" + comment + "\r\n");
				ch.remove_member(cl);
				ch.remove_member_from_invited_list(cl);
			}
			else
			{
				//:molybdenum.libera.chat 441 lokii ho #clcl :They aren't on that channel
				//:molybdenum.libera.chat 401 lokii jfhifhd :No such nick/channel
				// reply with  ERR_USERNOTINCHANNEL (441) 
				//:platinum.libera.chat 401 lokki ssssssssssss :No such nick/channel
				reply(c, ":irc.bmeek.chat 441 " + c.get_nick_name() + " " + cl.get_nick_name() + " " + channel + " :They aren't on that channel\r\n");
			}
		}
		catch (const std::string &)
		{
			//:platinum.libera.chat 401 lokki ssssssssssss :No such nick/channel
			reply(c, ":irc.bmeek.chat 401 " + c.get_nick_name() + " " + user + " :No such nick/channel\r\n");
		}
	}
}

//      Command: INVITE
//   Parameters: <nickname> <channel>
void	Server::invite(std::string param, Client &c)
{
	std::string	user = get_token(param);
	std::string	channel = get_token(param);
	if (channel.empty() || user.empty())
	{
		//:erbium.libera.chat 461 domy INVITE :Not enough parameters
		reply(c, ":irc.bmeek.chat 461 " + c.get_nick_name() + " INVITE " + " :Not enough parameters\r\n");
		return;
	}
	if (!client_exists(user))
	{
		// :mercury.libera.chat 401 lloki dhssd :No such nick/channel
		reply(c, ":irc.bmeek.chat 401 " + c.get_nick_name() + " " + user + " :No such nick/channel\r\n");
		return;
	}
	if (!channel_exists(channel))
	{
		// :mercury.libera.chat 403 lloki #ccccc :No such channel
		reply(c, ":irc.bmeek.chat 403 " + c.get_nick_name() + " " + channel + " :No such channel\r\n");
		return;
	}
	Channel& ch = get_channel_by_name(channel);
	if (ch.is_member(c) == 0)
	{
		// :platinum.libera.chat 482 hhulk #clcl :You're not a channel operator
		reply (c, ":irc.bmeek.chat 482 " + c.get_nick_name() + " " + channel + ":You're not a channel operator\r\n");
		return;	
	}
	if (ch.is_member(c) == -1)
	{
		// :erbium.libera.chat 442 mr #clcl :You're not on that channel
		reply (c, ":irc.bmeek.chat 442 " + c.get_nick_name() + " " + channel + ":You're not on that channel\r\n");
		return;	
	}
	Client& cl = get_client_by_name(user);
	if (ch.is_member(cl) != -1)
	{
		//:mercury.libera.chat 443 lloki hhulk #clcl :is already on channel
		reply(c, ":irc.bmeek.chat 443 " + c.get_nick_name() + " " + user + " " + channel + " :is already on channel\r\n");
		return;
	}
	ch.invite_member(c);
	//:mercury.libera.chat 341 lloki mr #clcl
	reply(c, ":irc.bmeek.chat 341 " + c.get_nick_name() + " " + user + " " + channel + "\r\n");
	// :lloki!~loki@197.230.30.146 INVITE mr :#clcl
	reply(cl, ":" + c.get_nick_name() + "!~" + c.get_user_name() + "@127.0.0.1" + " INVITE " + user + ":" + channel + "\r\n");
	return;
}


//      Command: TOPIC
//   Parameters: <channel> [<topic>]
void	Server::topic(std::string param, Client &c)
{
	std::string			channel = get_token(param);
	std::string			topic = get_token(param);
	if (channel.empty())
	{
		// :copper.libera.chat 461 lloki TOPIC :Not enough parameters
		reply(c, ":irc.libera.chat 461 " + c.get_nick_name() + " TOPIC :Not enough parameters\r\n");
		return;
	}
	if (!channel_exists(channel))
	{
		// :copper.libera.chat 403 lloki #ccclclclclc :No such channe)
		reply(c, ":irc.bmeek.chat 403 " + c.get_nick_name() + " " + channel + " :No suck channel\r\n");
		return;
	}
	Channel& ch = get_channel_by_name(channel);
	int i = ch.is_member(c);
	if (i < 0)
	{
		//  ERR_NOTONCHANNEL (442) 
		//:osmium.libera.chat 442 flash #clcl :You're not on that channel
		reply(c, ":irc.bmeek.chat 442 " + c.get_nick_name() + " " + channel + " :You're not on that channel\r\n");
		return;
	}
	if (topic.empty())
	{
		if (i == 0)
		{
			//:tungsten.libera.chat 482 hhulk #clcl :You're not a channel operator
			reply(c, ":irc.bmeek.chat 482 " + c.get_nick_name() + " " + channel + " :You're not a channel operator\r\n");
			return;
		}
		ch.set_topic(topic);
		// :lloki!~l@197.230.30.146 TOPIC #clcl :meating (broadcast)
		reply(c, ":" + c.get_nick_name() + "!~" + c.get_user_name() + "@127.0.0.1" + " TOPIC " + channel + " :" + topic + "\r\n");
		ch.broadcast(":" + c.get_nick_name() + "!~" + c.get_user_name() + "@127.0.0.1" + " TOPIC " + channel + " :" + topic + "\r\n", c.get_nick_name());
		return;
	}
	else
	{
		topic = ch.get_topic();
		if (!topic.empty())
			topic = "no topic set";
		reply(c, ":irc.bmeek.chat 332 " + c.get_nick_name() + " " + channel + " :" + topic + "\r\n");
		//:copper.libera.chat 332 lloki #clcl :meating
		//:copper.libera.chat 333 lloki #clcl lloki!~l@197.230.30.146 1707662897
		return;
	}
}

//      Command: MODE
//   Parameters: <target> [<modestring> [<mode arguments>...]]

void	Server::mode(std::string param, Client &c)
{
	try
	{
		/* code */
	
		std::string	target;
		std::string	modestring;
		std::string	made_arg;

		target = get_token(param);
		Channel&	ch = get_channel_by_name(target);
		modestring = get_token(param);
		if (modestring.empty())
		{
			//:osmium.libera.chat 324 bmeek #clcl +Cnst
			// :osmium.libera.chat 329 bmeek #clcl 1707680144
			//:osmium.libera.chat 461 bmeek MODE :Not enough parameters
			reply(c, ":irc.bmeek.chat 461 " + c.get_nick_name() + " MODE :Not enough parameters\r\n");
		}
		else
		{
			if (modestring == "+i")
				ch.enable_mode(CH_INVITE_ONLY);
			else if (modestring == "-i")
				ch.disable_mode(CH_INVITE_ONLY);
			else if (modestring == "+k")
			{
				std::string pass = get_token(param);
				if (!pass.empty())
				{
					ch.enable_mode(CH_KEY);
					ch.set_passwd(pass);
				}
			}
			else if (modestring == "-k")
			{
				ch.disable_mode(CH_KEY);
				ch.set_passwd("");
			}
		}
	}
	catch(const char * s)
	{
		reply(c, ERR_NOSUCHCHANNEL(c.get_hostname(), s, c.get_nick_name()));
		// std :: cout << s << std :: endl;
	}
	//:tungsten.libera.chat 472 dr * :is an unknown mode char to me
	// :tungsten.libera.chat 461 dr MODE :Not enough parameters

}

// PART

void		Server::execute_cmd(std::string msg, Client &c)
{
	std::cout << ">>>>> cmd "<< msg << std::endl;
	if (msg.back() == '\n')
		msg.pop_back();
	if (msg.back() == '\r')
		msg.pop_back();
	std::string cmd = get_token(msg);
	if (cmd.empty() || cmd == "\r\n")
		return;
	for (size_t i = 0; i < cmd.size(); i++)
		cmd[i] = toupper(cmd[i]);
	if (this->_cmds.find(cmd) != this->_cmds.end())
		(this->*_cmds[cmd])(msg, c);
	else
	{
		// reply(c, ERR_UNKNOWNCOMMAND(c.get_nick_name(), c.get_hostname(), cmd));
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
	char	buffer[4];
	std::string line = "";
	ssize_t	bufferr;

	std::memset(buffer, 0, sizeof(buffer));
	bufferr = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bufferr == -1)
	{
		if (errno == EAGAIN)
		{
			std:: cout << "roro" << std::endl;
			return (line);
		}
		throw std::runtime_error("Error reciving");
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
	std::vector<pollfd>						pollfds;
	int										_clientSocket;
	struct sockaddr_in						clientaddr;
	socklen_t								len = sizeof(clientaddr);
	pollfd Pollfd;

	Pollfd.fd = _socket_fd;
	Pollfd.events = POLLSTANDARD;
	Pollfd.revents = 0;

	pollfds.push_back(Pollfd);
	while (true)
	{
		if (poll(pollfds.data(), pollfds.size(), -1) == -1)
			throw std::runtime_error("Error polling");
		for (size_t i = 0; i < pollfds.size(); i++) // EAGAIN is an erno 
		{
			if ((pollfds[i].revents & POLLHUP) == POLLHUP)
			{
				std::cout << "Client Disconnected" << std::endl;
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
					{
						if (errno == EMFILE)
						{
							std::cout << "Maximum file descriptors reached" << std::endl;
							continue;
						}
						else
						{
							perror("Error connecting with client");
							continue;
						}
					}
					else
					{
						if (fcntl(_clientSocket, F_SETFL, O_NONBLOCK) == -1)
							perror("fcntl");
						std::cout << "New Connection Has Been Accepted, Client Socket: "<< _clientSocket - 3 << std::endl;
						Pollfd.fd = _clientSocket;
						pollfds.push_back(Pollfd);


						Client	a(_clientSocket);
						_clients.push_back(a);
					}
					break ;
				}
				else
				{
					Client&		 cl = get_client_by_id(pollfds[i].fd);
					std::string cl_line = cl.get_line_read();
				

					std::string	line = recving(pollfds[i].fd);

					cl_line.append(line.c_str());
					cl.set_line(cl_line);
					if (std::strstr(cl_line.c_str(), "\r\n"))
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

std::string	get_source(std::string nick, std::string user, std::string host)
{
	return nick + "!" + user + "@" + host;
	//nick>!<user>@<host>
}
/*def count_multiprime(a, b):
    def has_multiprime(n):
        if n < 2: return False
        div, pf = 2, set()
        while n > 1:
            while n % div == 0:
                n //= div
                pf.add(div)
            div += 1
            if div * div > n:
                if n > 1: pf.add(n)
                break
        return len(pf) > 1
    
    return sum(has_multiprime(x) for x in range(a, b + 1))

n = int(input())
for _ in range(n):
    a, b = map(int, input().split())
    print(count_multiprime(a, b))*/
// :tungsten.libera.chat 421 dr modscs :Unknown command