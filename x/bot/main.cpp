#include "bot.hpp"
#include "../Server/SocketManager.hpp"

int	main(int ac, char **av)
{
	if (ac == 4) {
	ircBot ircbot(std::atoi(*(av + 2)), *(av + 3), *(av + 1));

	// Create and initialize your IRC server logic here
	// Initialize server socket, set it to non-blocking mode, and bind it to the specified port
		SocketManager	socketManager(ircbot.get_port(), false);
		ircbot.set_botSocke(socketManager.getSocket());
		ircbot.startBot();
	}
	return (0);
}

