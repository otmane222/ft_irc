/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obahi <obahi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:40:56 by obahi             #+#    #+#             */
/*   Updated: 2024/01/08 18:18:07 by obahi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int sock_fd, std::string  nick_name, std::string name):
_sock_fd(sock_fd), _nick_name(nick_name), _name(name) {}

Client::Client(const Client& rhs)
{
	_sock_fd = rhs.get_socket_fd();
	_nick_name = rhs.get_nick_name();
	_name = rhs.get_name();
	_invites = rhs.get_invites();
}

Client& Client::operator=(const Client& rhs)
{
	if (this != &rhs)
	{
		_sock_fd = rhs.get_socket_fd();
		_nick_name = rhs.get_nick_name();
		_name = rhs.get_name();
		_invites = rhs.get_invites();
	}
	return (*this);
}

Client::~Client(){}

