#pragma once

#include <iostream>
#include <vector>

class client
{
	private:
		int					socket;
		std::vector<int>	clientsocket;
		std::vector<int>	channels;
	public:
		client();
		client(int socket, std::vector<int>& clientsocket);
		int getSocket();
		~client();
};
