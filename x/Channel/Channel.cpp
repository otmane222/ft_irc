#include "Channel.hpp"

Channel::Channel(std::string name, std::string passwd): _name(name), _passwd(passwd)
{
	_topic = "";
	_nbr_members = 0;
	_max_members = 0;
	_mode = 0;
}

Channel::~Channel(){}

// getters .
std::string	Channel::get_name() const {return _name;}
std::string	Channel::get_passwd() const {return _passwd;}
std::string	Channel::get_topic() const {return _topic;}
size_t		Channel::get_nbr_members() const {return _nbr_members;}
size_t		Channel::get_max_members() const {return _max_members;}

// setters
void	Channel::set_name(std::string & name) {_name = name;}
void	Channel::set_passwd(std::string & passwd) {_passwd = passwd;}
void	Channel::set_topic(std::string & topic) {_topic = topic;}
void	Channel::set_nbr_members(size_t nbr_members) {_nbr_members = nbr_members;}
void	Channel::set_max_members(size_t max_members) {_max_members = max_members;}

// mode controle
void	Channel::enable_mode(mode_t mode) {_mode |= mode;}
void	Channel::disable_mode(mode_t mode) {_mode = _mode & (~mode);}


// members methode
void	Channel::add_member(Client &c, std::string passwd)
{
	if (_members.find(c) != _members.end())
	{
		//reply with something
		std::cout << "member already exists" << std::endl;
		throw (1);
	}
	if ((_mode & CH_CLIENT_LIMIT) && _nbr_members >= _max_members)
	{
		//reply with ERR_CHANNELISFULL (471)
		std::cout << "channel is full" << std::endl;
		throw (2);
	}
	if ((_mode & CH_INVITE_ONLY) && find(_invited.begin(), _invited.end(), c) == _invited.end())
	{
		//reply with ERR_INVITEONLYCHAN (473)
		std::cout << "user is not invited" << std::endl;
		throw (3);
	}
	if ((_mode & CH_KEY) == CH_KEY && passwd != _passwd)
	{
		// reply with ERR_BADCHANNELKEY (475)
		std::cout << "wrong key" << std::endl;
		throw (4);
	}
	_members[c] = 0;
	std::cout << "memeber added succesfully" << std::endl;
	// reply(c, RPL_JOIN(c.get_nick_name(), c.get_user_name(), channel, "127.0.0.1"));
	// reply(c, RPL_TOPICDISPLAY(c.get_hostname(), c.get_nick_name(), channel, ch.get_topic()))
	//no topic
	//send reply
}

void	Channel::add_operator(Client &c)
{
	if (_members.find(c) != _members.end())
	{
		//reply with something
		std::cout << "member already exists" << std::endl;
		return;
	}
	_members[c] = 1;
}

void	Channel::remove_member(Client & c)
{
	std::map<Client, int>::iterator itr = _members.find(c);
	if (itr != _members.end())
	{
		_members.erase(itr);
		std::cout << "member removed" << std::endl;
	}
}

void	Channel::promote_member(Client &c)
{
	if (_members.find(c) != _members.end())
		_members[c] = 1;
}

void	Channel::unpromote_member(Client &c)
{
	if (_members.find(c) != _members.end())
		_members[c] = 0;
}

void	Channel::invite_member(Client & c)
{
	if (_members.find(c) == _members.end())
	{
		if (find(_invited.begin(), _invited.end(), c) != _invited.end())
		{
			_invited.push_back(c);
			std::cout << "member invited" << std::endl;
		}
		else
		{
			std::cout << "member already have an invitation" << std::endl;
		}
	}
	else
	{
		// ERR_USERONCHANNEL (443)
		std::cout << "member in channel" << std::endl; 
	}
}

int	Channel::is_member(Client &c)
{
	std::map<Client, int>::iterator itr = _members.find(c);
	if (itr == _members.end())
		return (-1);
	return (itr->second);
}

int	Channel::is_member(std::string &name)
{
	std::map<Client, int>::iterator itr = _members.begin();
	while (itr != _members.end())
	{
		if (itr->first.get_nick_name() == name)
			return (itr->second);
		itr++;
	}
	return (-1);
}

const Client	&Channel::get_member_by_name(std::string &name)
{
	std::map<Client, int>::iterator itr = _members.begin();
	while (itr != _members.end())
	{
		if (itr->first.get_nick_name() == name)
			return (itr->first);
		itr++;
	}
	throw ("Client not found");
}

void	Channel::broadcast(const std::string &msg)
{
	std::map<Client, int>::iterator itr = _members.begin();
	while (itr != _members.end())
	{
		ssize_t	count = send(itr->first.get_socket_fd(), msg.c_str(), strlen(msg.c_str()), 0);
		if (count == -1)
		{
			std :: cout << "Error sending" << std::endl;
		}
		itr++;
	}
}

std::string	Channel::get_list_of_names()
{
	std::string	names;
	std::map<Client, int>::iterator itr = _members.begin();
	while (itr != _members.end())
	{
		if (itr->second == 1)
			names += "@" + itr->first.get_nick_name() + " ";
		else
			names += itr->first.get_nick_name() + " ";
		itr++;
	}
	return names;
}

std::map <Client, int> Channel::get_members() const {return _members;}
bool	Channel::operator==(const Channel& cl) const
{
	return (this->_name == cl.get_name());
}