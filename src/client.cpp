#include "client.hpp"
#include "channel.hpp"
#include "socketManager.hpp"

client::client()
{
}

client::~client()
{
	// close(socket);
}

client::client(int socket,std::vector<int>& clients_socket) : socket(socket) , clients_socket(clients_socket), valid(false), steps(0)
{
	std::string	cname = "client ";
	cname += std::to_string(socket - 3);

	this->username = cname;
}

int client::getSocket()
{
	return (socket);
}

std::string client::getUserName()
{
	return (this->username);
}

std::string client::getNickName()
{
	return (this->nickname);
}

std::string client::getHostName()
{
	return (this->hostname);
}

void	client::setUserName(std::string str)
{
	this->username = str;
}

void	client::setNickName(std::string str)
{
	this->nickname = str;
}

void	client::setHostName(std::string str)
{
	this->hostname = str;
}

void	client::reply(std::string& message) const
{
	ssize_t bytes_sent = send(socket, message.c_str(), strlen(message.c_str()), 0);
	if (bytes_sent == -1)
	{
		std :: cerr << "Error sending" << std::endl;
		throw("Error sending");
	}
}

client	client::operator = (const client& other)
{
	if (this != &other)
	{
		this->socket = other.socket;
		this->clients_socket = other.clients_socket;
		this->my_channels = other.my_channels;
		this->username = other.username;
		this->nickname = other.nickname;
		this->hostname = other.hostname;
	}
	return (*this);
}

bool client::operator==(const client& cl) const
{
	return (this->socket == cl.socket);
}