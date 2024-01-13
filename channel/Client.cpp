/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obahi <obahi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:40:56 by obahi             #+#    #+#             */
/*   Updated: 2024/01/12 10:35:26 by obahi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int sock_fd, std::string  nick_name, std::string name, bool registred):
_sock_fd(sock_fd), _nick_name(nick_name), _real_name(name), _registred(registred) {}

Client::~Client()
{
	// close(_sock_fd);
}

int							Client::get_socket_fd() const{return _sock_fd;}
std::string 				Client::get_nick_name() const{return _nick_name;}
std::string 				Client::get_user_name() const{return _user_name;}
std::string 				Client::get_real_name() const{return _real_name;}

void	Client::set_socket_fd(int sock_fd) {_sock_fd = sock_fd;}
void 	Client::set_nick_name(std::string nick_name) {_nick_name = nick_name;}
void 	Client::set_user_name(std::string name) {_user_name = name;}
void 	Client::set_real_name(std::string name) {_real_name = name;}