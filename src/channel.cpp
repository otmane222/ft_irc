#include "channel.hpp"
#include "client.hpp"

channel::channel() : password_set(false), invite_only(false)
{
	
}

channel::~channel()
{
	
}

void	channel::setMod(client	moderator)
{
	this->moderator = moderator;
}

void	channel::setSize(int size)
{
	this->size = size;
}

client	channel::getMod()
{
	return (this->moderator);
}

int		channel::getSize()
{
	return (size);
}

void	channel::addClient(client cl)
{
	clients.push_back(cl);
}

void	channel::removeClient(client cl)
{
	std::vector<client>::iterator it = std::find(clients.begin(), clients.end(), cl);
	if (it != clients.end())
		clients.erase(it);
}


channel channel::operator = (const channel& other)
{
	if (this != &other)
	{
		this->clients = other.clients;
		this->channelName = other.channelName;
		this->moderator = other.moderator;
		this->size = other.size;
	}
	return (*this);
}

bool channel::operator==(const channel& other) const
{
	return (this->channelName == other.channelName);
}