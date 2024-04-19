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

		std::cout << " /BOT\\ Login..." << std::endl;

		std::string sendPass = "PASS " + _passwd + "\r\n";
		std::string sendNick = "NICK ircBot\r\n";
		std::string sendUser = "USER ircBot 0 * ircBot\r\n";

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

		std::cout << "hello you can start dud!" << std::endl;
		while (1) {
			memset(buffer, 0, bufferSize);
			int bytesRead = recv(_botSocket, buffer, bufferSize, 0);
			if (bytesRead == 0){
				std::cerr << "Error: connection closed" << std::endl;
				return ;
			}
			else if (bytesRead == -1) {
            	std::cerr << "Error receiving data" << std::endl;
				return ;
			}
			else {
					std::string str, quotes[200];
					std::ifstream ifs;
					int i = 0;
					ifs.open("quotes.txt");
					if (!ifs) 
					{
						std::cout << "File opening failed. exiting..\n";
						return ;
					}
					while (!ifs.eof()) 
					{
						std::getline(ifs, str);
						quotes[i++] = str;
					}
					ifs.close();
					std::string message;
					std::string nick;
					message	= getMessageFromBuffer(buffer);
					nick		= getNickFromBuffer(buffer);
					if (!message.empty())
						botHundler(message, _botSocket, nick, quotes);
				}
		}
}