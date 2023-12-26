#include "src/ircserver.hpp"
#include "src/socketManager.hpp"
#include "src/client.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return (1);
	}

	int			port = std::stoi(av[1]);
	std::string	password = av[2];


	// Create and initialize your IRC server logic here
	// Initialize server socket, set it to non-blocking mode, and bind it to the specified port
	socketManager	socketManagerr(port);
	ircserver		server;

	server.start(socketManagerr.getSocket());

	return 0;
}

