/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obahi <obahi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 10:38:16 by obahi             #+#    #+#             */
/*   Updated: 2024/01/13 11:31:17 by obahi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, std::string passwd,std::string topic, mode_t mode, size_t max_members)
: _name(name), _passwd(passwd), _topic(topic), _mode(mode), _max_members(max_members){}

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



bool	Channel::client_limit_mode() const {_mode & CH_CLIENT_LIMIT == CH_CLIENT_LIMIT;}
bool	Channel::invite_only_mode() const {_mode & CH_INVITE_ONLY == CH_INVITE_ONLY;};
bool	Channel::key_mode() const {_mode & CH_KEY == CH_KEY;}
bool	Channel::protected_topic_mode() const {_mode & CH_PROTECTED_TOPIC == CH_PROTECTED_TOPIC;}

void	Channel::enable_mode(mode_t mode) {_mode |= mode;}
void	Channel::disable_mode(mode_t mode) {_mode & (~mode);}


// bool	Channel::is_invited(Client &c) const
// {
// 	f
// 	if (_invited.find(c) == _invited.end())
// 		return false;
// 	return true;
// }

// bool	Channel::is_member(Client &c) const
// {
// 	if (_members.find(c) == _members.end())
// 		return false;
// 	return true;
// }

void	Channel::add_operator(Client &c)
{
	if (_members.find(c) == _members.end())
	{
		//reply with something
		return;
	}
	_members[c] = 1;
}
void	Channel::add_member(Client &c, std::string passwd)
{
		
	if (_members.find(c) == _members.end())
	{
		//reply with something
		return;
	}
	if (_mode & CH_CLIENT_LIMIT && _nbr_members >= _max_members)
	{
		//reply with ERR_CHANNELISFULL (471)
		return;
	}
	if (_mode & CH_INVITE_ONLY && find(_invited.begin(), _invited.end(), c) == _invited.end())
	{
		//reply with ERR_INVITEONLYCHAN (473)
		return;
	}
	if (_mode & CH_KEY == CH_KEY && passwd != _passwd)
	{
		// reply with ERR_BADCHANNELKEY (475)
		return;
	}
	_members[c] = 0;
	//send reply
}

void	Channel::remove_member(Client & c)
{
	std::map<Client, int>::iterator itr = _members.find(c);
	if (itr != _members.end())
		_members.erase(itr);
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
	if (find(_invited.begin(), _invited.end(), c) != _invited.end())
		_invited.push_back(c);
	else
		// ERR_USERONCHANNEL (443)
}

int	Channel::is_member(Client &c) const
{
	std::map<std::string, int>::iterator itr = _members.find(c);
	if (itr == _members.end())
		return (-1);
	return (itr->second);
}