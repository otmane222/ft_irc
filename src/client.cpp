#include "client.hpp"

client::client(/* args */)
{
}

client::client(int socket,std::vector<int>& clientsocket) : socket(socket) , clientsocket(clientsocket) {}

int client::getSocket()
{
	return (socket);
}

client::~client()
{
}
