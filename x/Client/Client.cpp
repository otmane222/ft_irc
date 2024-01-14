#include "Client.hpp"

Client::Client(int sock_fd):_sock_fd(sock_fd)
{
	_nick_name = "";
	_user_name = "";
	_real_name = "";
	_hostname = "127.0.0.1";
	_passwd = "";
	_registred = false;
	_pass_valid = false;
}

Client::~Client()
{
	_nick_name = "";
	_user_name = "";
	_real_name = "";
	_hostname = "127.0.0.1";
	_passwd = "";
	_registred = false;
	_pass_valid = false;
}

int							Client::get_socket_fd() const{return _sock_fd;}
std::string 				Client::get_nick_name() const{return _nick_name;}
std::string 				Client::get_user_name() const{return _user_name;}
std::string 				Client::get_real_name() const{return _real_name;}
std::string 				Client::get_hostname() const{return _hostname;}
std::string 				Client::get_passwd() const{return _hostname;}
bool		 				Client::get_status() const{return _registred;}
bool		 				Client::get_pass_stat() const{return _pass_valid;}

void	Client::set_socket_fd(int sock_fd) {_sock_fd = sock_fd;}
void 	Client::set_nick_name(std::string name) {_nick_name = name;}
void 	Client::set_user_name(std::string name) {_user_name = name;}
void 	Client::set_real_name(std::string name) {_real_name = name;}
void 	Client::set_hostname(std::string name) {_hostname = name;}
void 	Client::set_passwd(std::string name) {_hostname = name;}
void	Client::set_status(bool status) {_registred = status;}
void	Client::set_pass_stat(bool status) {_pass_valid = status;}

void	Client::close_connection()
{
	close(_sock_fd);
}

bool	Client::operator ==(const Client& cl) const
{
	return (this->_sock_fd == cl.get_socket_fd());
}

bool	Client::operator < (const Client& cl) const
{
	return (this->_sock_fd < cl.get_socket_fd());
}