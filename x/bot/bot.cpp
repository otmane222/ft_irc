#include "bot.hpp"

ircBot::ircBot(int	port, std::string passwd, std::string hostname) :
_port(port), _passwd(passwd), _hostname(hostname)
{
	_botSocket = -1;
}

ircBot::~ircBot(){}

int			ircBot::get_port() const {return _port;}
int			ircBot::get_botSocke() const {return _botSocket;}
std::string	ircBot::get_passwd() const {return _passwd;}

void	ircBot::set_port(int port) {_port = port;}
void	ircBot::set_botSocke(int botSocket) {_botSocket = botSocket;}
void	ircBot::set_passwd(std::string &passwd) {_passwd = passwd;}

void	ircBot::startBot()
{

		// The _botSocket is connected so we can send identification
		std::cout << " /BOT\\ Login..." << std::endl;

		std::string sendPass = "PASS " + _passwd + "\r\n";
		std::string sendNick = "NICK ircBot\r\n";
		std::string sendUser = "USER ircBot 0 * ircBot\r\n";

		// We send connection details to log to the server
		usleep(10000);
		send(_botSocket, sendPass.c_str(), sendPass.size(), 0);
		usleep(10000);
		send(_botSocket, sendNick.c_str(), sendNick.size(), 0);
		usleep(10000);
		send(_botSocket, sendUser.c_str(), sendUser.size(), 0);

		std::cout << " /BOT\\ The bot is logged !" << std::endl;

		std::string botJoinChannel = "JOIN #bot\r\n";

		usleep(10000);
		if (send(_botSocket, botJoinChannel.c_str(), botJoinChannel.size(), 0) <= 0) {
			std::cerr << "Error: send failed to join channel" << std::endl;
		}
		
		int		bufferSize = 512;
		char	buffer[bufferSize];	

		std::cout << "Ready to receive data!" << std::endl;
		while (1) {
			memset(buffer, 0, bufferSize);
			int bytesRead = recv(_botSocket, buffer, bufferSize, 0);
			// std::cout << "********" << buffer << bufferSize<<std::endl;
			if (bytesRead == 0){
				std::cerr << "Error: connection closed" << std::endl;
				return ;
			}
			else if (bytesRead == -1) {
            	std::cerr << "Error receiving data" << std::endl;
				return ;
			}
			else {
					std::string message;
					std::string nick;
					// We handle the message here
					// First we need to parse the buffer so we retreive only the message
					message	= retreiveMessageFromBuffer(buffer);
					nick		= retreiveNickFromBuffer(buffer);
					// Then we can proceed the bot reponses
					if (!message.empty())
					botBehavior(message, _botSocket, nick);
				}
		}
}