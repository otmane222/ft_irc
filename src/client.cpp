#include "client.hpp"
#include "channel.hpp"
#include "socketManager.hpp"
#include "ircserver.hpp"

client::client()
{
}

client::~client()
{
	// close(socket);
}

client::client(int socket) : socket(socket), steps(0)
{
	username        = "";
	nickname        = "";
	hostname        = "localhost";
	stat.password   = false;
	stat.nickname   = false;
	stat.username   = false;
	stat.registered = false;
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

void	client::reply(std::string message) const
{
	ssize_t bytes_sent = send(socket, message.c_str(), strlen(message.c_str()), 0);
	if (bytes_sent == -1)
	{
		std :: cerr << "Error sending" << std::endl;
		throw("Error sending");
	}
}

void pass(client& cl, std::vector<std::string> cmd, std::string password)
{
	std::cout<< "pass" <<std::endl;
	if (cl.stat.registered)
		return (cl.reply(ERR_ALREADYREGISTERED(cl.getNickName(), cl.getHostName())));
	if (cl.stat.nickname)
		return ;
	if (cmd.size() != 2) 
		return (cl.stat.password = false, cl.reply(ERR_NEEDMOREPARAMS(cl.getNickName(), cl.getHostName())));
	if (cmd[1] != password)
		return (cl.stat.password = false, cl.reply(ERR_PASSWDMISMATCH(cl.getNickName(), cl.getHostName())));
	cl.stat.password = true;
	std::cout << "password accepted" << std::endl;
}

void nick(client& cl, std::vector<std::string> cmd, std::vector<client> cls)
{
	std::cout<< "nick" <<std::endl;
	if (cl.stat.registered)
		return (cl.reply(ERR_ALREADYREGISTERED(cl.getNickName(), cl.getHostName())));
	if (!cl.stat.password || cl.stat.username)
		return ;
	if (cmd.size() == 1)
		return (cl.reply(ERR_NONICKNAMEGIVEN(cl.getNickName(), cl.getHostName())));
	std::vector<client>::iterator it = cls.begin();
	for (; it != cls.end(); it++)
	{
		if ((*it).getNickName() != "" && (*it).getNickName() == cmd[1])
			return (cl.stat.nickname = false, cl.reply(ERR_NICKNAMEINUSE(cl.getNickName(), cl.getHostName())));
	} 
	const char* search = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
	if (std::isdigit(cmd[1][0]))
		return (cl.stat.nickname = false, cl.reply(ERR_ERRONEUSNICKNAME(cl.getNickName(), cl.getHostName())));
	size_t i = 0;
	if (cmd[1][0] == ':')
		i++;
	for (; cmd[1][i] != '\0'; i++)
	{
		std :: cout << "'" << cmd[1][i] << "'" << std::endl;
		if (!std::strchr(search, cmd[1][i]))
			return (cl.stat.nickname = false, cl.reply(ERR_ERRONEUSNICKNAME(cl.getNickName(), cl.getHostName())));

	}
	if (cl.getNickName() != "")
		cl.reply(RPL_NICKCHANGE(cl.getNickName(), cmd[1], cl.getHostName()));
	cl.setNickName(cmd[1]);
	cl.stat.nickname = true;
	std::cout << "nickname accepted" << std::endl;
}

std::string extractRealname(const std::vector<std::string>& params)
{
	// Extract the realname parameter, which may contain spaces
	std::string realname;
	for (size_t i = 4; i < params.size(); ++i)
	{
		realname += params[i];
		if (i != params.size() - 1)
			realname += " ";
	}
	// Check if the realname starts with a colon, and remove it
	if (!realname.empty() && realname[0] == ':')
		realname = realname.substr(1);
	return (realname);
}

void user(client& cl, std::vector<std::string> cmd)
{
	std::cout << "user" << std::endl;
	if (cl.stat.registered)
		return (cl.reply(ERR_ALREADYREGISTERED(cl.getNickName(), cl.getHostName())));
	if (cmd.size() < 5)
		return (cl.reply(ERR_NEEDMOREPARAMS(cl.getNickName(), cl.getHostName())));
	if (!cl.stat.nickname)
		return ;
	if (cmd[2] != "0" || cmd[3] != "*")
		return ;
	const char* search = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789([ ]{})\'|";
	std::string realname = extractRealname(cmd);
	std::string username = cmd[1];
	if (username.empty() || username.length() > USERLEN || username.length() < 1)
		return ;
	for (size_t i = 1; cmd[1][i] != '\0'; i++)
	{
		if (!std::strchr(search, username[i]))
			return ;
	}
	cl.setUserName(username);
	cl.stat.username = true;
	cl.stat.registered = true;
	cl.reply(RPL_WELCOME(cl.getNickName(), cl.getHostName()));
	std::cout << "username accepted" << std::endl;
}

void	client::registration(std::string line, std::string password, std::vector<client> cls)
{
	if (line.empty())
		return ;
	if (line.back() == '\n')
	{
		line.pop_back();
		if (line.back() == '\r')
			line.pop_back();
	}
	std::vector<std::string> cmd = split(line, ' ');
	if (cmd.size() == 0)
	{
		std :: cout << "empty line" << std::endl;
		return ;
	}
	if (cmd[0] == "PASS")
	{
		pass(*this, cmd, password);
		return ;
	}
	if (cmd[0] == "NICK")
	{
		nick(*this, cmd, cls);
		return ;
	}
	if (cmd[0] == "USER")
	{
		user(*this, cmd);
		return ;
	}
}

void	client::close_connection(std::vector<pollfd>& pollfds, std::map<int, client>& clients, int i)
{
	std::cerr << "Connection closed by client" << std::endl;
	close(socket);
	pollfds.erase(pollfds.begin() + i);
	clients.erase(pollfds[i].fd);
}

client	client::operator = (const client& other)
{
	if (this != &other)
	{
		this->socket = other.socket;
		this->clients_socket = other.clients_socket;
		// this->my_channels = other.my_channels;
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
