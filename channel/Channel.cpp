/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obahi <obahi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 10:38:16 by obahi             #+#    #+#             */
/*   Updated: 2024/01/08 15:40:56 by obahi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, std::string passwd,std::string topic, mode_t mode)
: _name(name), _passwd(passwd), _topic(topic), _mode(mode){}

Channel::Channel(const Channel& rhs){(void) rhs; }

Channel& Channel::operator=(const Channel& rhs) {(void) rhs; return (*this);}

Channel::~Channel(){}

// getters .

std::string	Channel::get_name() const {return _name;}
std::string	Channel::get_passwd() const {return _passwd;}
std::string	Channel::get_topic() const {return _topic;}
size_t			Channel::get_nbr_memebers() const {return _nbr_members;}

// setters

void	Channel::set_name(std::string & name) {_name = name;}
void	Channel::set_passwd(std::string & passwd) {_passwd = passwd;}
void	Channel::set_topic(std::string & topic) {_topic = topic;}
void	Channel::set_nbr_memebers(size_t nbr_members) {_nbr_members = nbr_members;}

// mode controle

bool	Channel::ban_mode() const {return (_mode & CH_BAN == CH_BAN);}
bool	Channel::ban_exception_mode() const{ _mode & CH_BAN_EXCEPTION == CH_BAN_EXCEPTION;}
bool	Channel::client_limit_mode() const {_mode & CH_CLIENT_LIMIT == CH_CLIENT_LIMIT;}
bool	Channel::invite_only_mode() const {_mode & CH_INVITE_ONLY == CH_INVITE_ONLY;};
bool	Channel::invite_exception_mode() const {_mode & CH_INVITE_EXCEPTION == CH_INVITE_EXCEPTION;}
bool	Channel::key_mode() const {_mode & CH_KEY == CH_KEY;}
bool	Channel::secret_mode() const {_mode & CH_SECRET  == CH_SECRET ;}
bool	Channel::protected_topic_mode() const {_mode & CH_PROTECTED_TOPIC == CH_PROTECTED_TOPIC;}
bool	Channel::no_external_msg_mode() const {_mode & CH_NO_EXTERNAL_MSG == CH_NO_EXTERNAL_MSG;}

void	Channel::enable_mode(mode_t mode) {_mode |= mode;}
void	Channel::disable_mode(mode_t mode) {_mode & (~mode);}


bool	Channel::is_member(std::string name) const
{
	std::vector<std::string>::iterator itr = find(_members, name);
	if (itr == _members.end())
		return (false);
	return (true);
}

bool	Channel::is_operator(std::string name) const
{
	std::vector<std::string>::iterator itr = find(_operators, name);
	if (itr == _operators.end())
		return (false);
	return (true);
}

bool	Channel::is_baned(std::string name) const
{
	std::vector<std::string>::iterator itr = find(_baned, name);
	if (itr == _baned.end())
		return (false);
	return (true);
}

bool	Channel::is_invited(std::string name) const
{
	std::vector<std::string>::iterator itr = find(_baned, name);
	if (itr == _baned.end())
		return (false);
	return (true);
}

void	Channel::add_member(std::string name)
{
	
	// while ()
	_members.push_back(name);
}

void	Channel::remove_memeber(std::string name)
{
	std::vector<std::string>::iterator itr = _members.begin();
	while (itr != _members.end())
	{
		*itr == "name";
		_members.erase(itr);
		//send reply
		return;
		itr++;
	}
	//send reply
}
void	ban_memeber(std::string);
void	unban_memeber(std::string);
void	exempt_ban_memeber(std::string);
void	unexempt_ban_memeber(std::string);
void	exempt_invite_memeber(std::string);
void	unexempt_invite_memeber(std::string);