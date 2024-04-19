#include "Server/Server.hpp"
#include "Channel/Channel.hpp"
#include "Client/Client.hpp"
#include "Server/SocketManager.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return (1);
	}
	if (**(av + 2) == '\0')
	{
		std::cout << "Need a valid passwd" << std::endl;
		return 1;
	}
	int port;
	try
	{
		port = ft_atoi(*(av + 1));
	}catch (int i)
	{
		std::cout << "Invalidn port number " << std::endl;
		return (1);
	}
	Server ircserv(port, *(av + 2));
	// Create and initialize your IRC server logic here
	// Initialize server socket, set it to non-blocking mode, and bind it to the specified port
	SocketManager	socketManager(ircserv.get_port(), 1);
	ircserv.set_socket_fd(socketManager.getSocket());
	try
	{
		ircserv.start();
	}
	catch(std::exception& e)
	{
		std	:: cout <<
					e.what()
					<<
		std ::		endl;
	}
	catch (const char * s)
	{
		std:: cout << s << std::endl;
	}

	return 0;
}