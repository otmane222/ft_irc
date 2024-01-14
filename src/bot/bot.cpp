#include "bot.hpp"

int	main(int ac, char **av)
{
	if (ac == 4) {

		std::string hostname = av[1];
		std::string password = av[2];
		int port = atoi(av[3]);
		int botSocket;
		struct sockaddr_in serverAddress;

		botSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (botSocket == -1) {
		std::cerr << "Error during creation of socket" << std::endl;
		return 1;
		}

		memset(&serverAddress, 0, sizeof(serverAddress));
		serverAddress.sin_family = AF_INET; //AF_INET -> Famille IPV4
		serverAddress.sin_port = htons(port);	
		
		//	Get server address
		struct hostent *host = gethostbyname(hostname.c_str());
		if (host == NULL) {
			std::cerr << "Error during the resolution of server address" << std::endl;
			return 1;
		}
		memcpy(&serverAddress.sin_addr.s_addr, host->h_addr, host->h_length);	

		// Connexion au serveur
		if (connect(botSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
			std::cerr << "Error during connection" << std::endl;
			return 1;
		}

		// The botSocket is connected so we can send identification
		std::cout << " /BOT\\ Login..." << std::endl;

		std::string sendPass = "PASS " + password + "\r\n";
		std::string sendNick = "NICK ircBot\r\n";
		std::string sendUser = "USER ircBot \r\n";

		// We send connection details to log to the server
		usleep(10000);

		send(botSocket, sendPass.c_str(), sendPass.size(), 0);
		usleep(10000);
		send(botSocket, sendNick.c_str(), sendNick.size(), 0);
		usleep(10000);
		send(botSocket, sendUser.c_str(), sendUser.size(), 0);

		std::cout << " /BOT\\ The bot is logged !" << std::endl;

		// GESTION DES COMMANDES avec recv()
		std::string botJoinChannel = "JOIN #bot\r\n";

		usleep(10000);
		if (send(botSocket, botJoinChannel.c_str(), botJoinChannel.size(), 0) <= 0) {
			std::cerr << "Error: send failed to join channel" << std::endl;
		}
		
		int		bufferSize = 512;
		char	buffer[bufferSize];	

		std::cout << "Ready to receive data!" << std::endl;
		while (1) {
			memset(buffer, 0, bufferSize);
			int bytesRead = recv(botSocket, buffer, bufferSize, 0);
			if (bytesRead == 0){
				std::cerr << "Error: connection closed" << std::endl;
				return 1;
			}
			else if (bytesRead < 0) {
				std::cerr << "Error: recv() failed" << std::endl;
				return 1;
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
						botBehavior(message, botSocket, nick);
				}
		}
	}
}

std::string	retreiveNickFromBuffer(char *buffer)
{
    size_t pos;
    std::vector<std::string>	stringElements = splitString(buffer, ":");

    pos = stringElements[0].find("!");
    std::string nick = stringElements[0].substr(0, pos);
    return (nick);
}

std::string	retreiveMessageFromBuffer(char *buffer) {

	std::vector<std::string>	stringElements = splitString(buffer, ":");
	int				id = stringElements.size() - 1;

	rebuildMessage(stringElements, id);
	return stringElements[id];
}

void tokenize(std::string const &str, const char* delim, std::vector<std::string> &out)
{
	char *token = strtok(const_cast<char*>(str.c_str()), delim);
	while (token != NULL)
	{
		out.push_back(std::string(token));
		token = strtok(NULL, delim);
	}
}

std::vector<std::string> splitString(std::string str, const char *delim)
{
	std::vector<std::string> out;
	tokenize(str, delim, out);
	return (out);
}

void rebuildMessage(std::vector<std::string> &vector, int index) {

	size_t i = index + 1;
	while (i < vector.size()) {
		vector[index].append(" ");
		vector[index].append(vector[i]);
		i++;
	}
	vector[index].append("\0");
	return ;
}

int 	botBehavior(std::string message, int botSocket, std::string nick)
{
	const char* channelName = "#bot";
	std::string response = std::string("PRIVMSG") + " " + channelName + " :";
	if (message.find("PING") != std::string::npos)
	{
		// Répondre au PING du serveur pour éviter d'être déconnecté
	 	std::string pongCommand = response.append("PONG\r\n");
		send(botSocket, pongCommand.c_str(), pongCommand.length(), 0);
	}
	else if (message.find("bonjour") != std::string::npos)
	{
	    std::string helloCommand = response.append("Bonjour ") + nick + ", comment allez vous ?" + std::string("\r\n");
	    send(botSocket, helloCommand.c_str(), helloCommand.size(), 0);
	}
	else if (message.find("bien") != std::string::npos)
	{
	    std::string goodCommand = response.append("Cela me rempli de joie ") + nick + " !\r\n";
	    send(botSocket, goodCommand.c_str(), goodCommand.size(), 0);
	}
	else if (message.find("mal") != std::string::npos || message.find("pas bien") != std::string::npos)
	{
	    std::string badCommand = response.append("Cela m'attriste pour vous ") + nick + " ...\r\n";
	    send(botSocket, badCommand.c_str(), badCommand.size(), 0);
	}
	else if (message.find("quoi") != std::string::npos)
	{
	    srand(time(0));
	    int nb = rand();
	    std::string feurCommand;
	    if (nb % 2 == 0)
		 feurCommand = response.append("Feur !!\r\n");
	    else
		 feurCommand = response.append("coubeh !!\r\n");
	    send(botSocket, feurCommand.c_str(), feurCommand.size(), 0);
	}
	else if (message.find("hein") != std::string::npos || message.find("1") != std::string::npos)
	{
	    std::string heinCommand = response.append("apanyan !! apanyan !\r\n");
	    send(botSocket, heinCommand.c_str(), heinCommand.size(), 0);
	}
	else if (message.find("Fils de pendu") != std::string::npos || message.find("orchidoclaste") != std::string::npos || message.find("nigaud") != std::string::npos || message.find("flagorneur") != std::string::npos)
	{
	    std::string kickCommand = std::string("KICK") + " ircserv " + channelName + " :" + nick + " :You have been kicked for bad behavior, watch your mouth. " + "\r\n";
	    send(botSocket, kickCommand.c_str(), kickCommand.size(), 0);
	}
	return 0;
}
