#include "bot.hpp"
#include "../Server/SocketManager.hpp"

int			ft_atoi(const char * str)
{
	int	sign;
	int	n;

	sign = 1;
	n = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	while (*str >= '0' && *str <= '9')
	{
		n = n * 10 + (*str - '0');
		str++;
		if (n > 65535)
			throw(2);
	}
	if (*str)
		throw (3);
	if (n < 1024)
		throw(1);
	return (sign * n);
}

int	main(int ac, char **av)
{
	if (ac == 4) 
	{
		int port;
		try
		{
			port = ft_atoi(*(av + 2));
		}catch (int i)
		{
			std::cout << "Invalidn port number " << std::endl;
			return (1);
		}
		ircBot ircbot(port, *(av + 3), *(av + 1));

		SocketManager	socketManager(ircbot.get_port(), false);
		ircbot.set_botSocke(socketManager.getSocket());
		ircbot.startBot();
	}
	return (0);
}

