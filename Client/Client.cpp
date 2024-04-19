#include "Client.hpp"

Client::Client(int sock_fd, std::string hostname): _sock_fd(sock_fd), _hostname (hostname)
{
	_nick_name = "";
	_user_name = "";
	_real_name = "";
	_passwd = "";
	_registred = 0;
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
	_pass_status = false;
	_user_status = false;
	_nick_status = false;
}

int							Client::get_socket_fd() const{return _sock_fd;}
std::string 				Client::get_nick_name() const{return _nick_name;}
std::string 				Client::get_user_name() const{return _user_name;}
std::string 				Client::get_real_name() const{return _real_name;}
std::string 				Client::get_hostname() const{return _hostname;}
std::string 				Client::get_passwd() const{return _passwd;}
std::string					Client::get_line_read() const {return  line;}
int			 				Client::get_status() const{return _registred;}
bool		 				Client::get_pass_stat() const{return _pass_valid;}
bool		 				Client::get_pass_status() const{return _pass_status;}
bool		 				Client::get_user_status() const{return _user_status;}
bool		 				Client::get_nick_status() const{return _nick_status;}

void	Client::set_line(std::string name) {line = name;}
void	Client::set_socket_fd(int sock_fd) {_sock_fd = sock_fd;}
void 	Client::set_nick_name(std::string name) {_nick_name = name;}
void 	Client::set_user_name(std::string name) {_user_name = name;}
void 	Client::set_real_name(std::string name) {_real_name = name;}
void 	Client::set_hostname(std::string name) {_hostname = name;}
void 	Client::set_passwd(std::string name) {_passwd = name;}
void	Client::set_status(int status) {_registred += status;}
void	Client::set_pass_stat(bool status) {_pass_valid = status;}
void	Client::set_pass_status(bool status) {_pass_status = status;}
void	Client::set_user_status(bool status) {_user_status = status;}
void	Client::set_nick_status(bool status) {_nick_status = status;}

bool	Client::is_athantificated() const
{
	return (_pass_status && _user_status && _nick_status);
}

void	Client::close_connection()
{
	close(_sock_fd);
}

bool	Client::operator ==(const Client& cl) const
{
	return (this->_sock_fd == cl.get_socket_fd());
}

bool	Client::operator !=(const Client& cl) const
{
	return (this->_sock_fd != cl.get_socket_fd());
}

bool	Client::operator < (const Client& cl) const
{
	return (this->_sock_fd < cl.get_socket_fd());
}