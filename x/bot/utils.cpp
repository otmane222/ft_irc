#include "bot.hpp"

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
		std::string str, quotes[200];
		std::ifstream ifs;
		int i = 0;
		ifs.open("quotes.txt");
		if (!ifs) 
		{
			std::cout << "File opening failed. exiting..\n";
			return 1;
		}
		while (!ifs.eof()) 
		{
			std::getline(ifs, str);
			quotes[i++] = str;
		}
		ifs.close();
	std::string response = std::string("PRIVMSG") + " " + nick + " :";
	if (message.find("PING") != std::string::npos)
	{
		std::string pongCommand = response.append("PONG\r\n");
		send(botSocket, pongCommand.c_str(), pongCommand.length(), 0);
	}
	else if (message.find("hello") != std::string::npos)
	{
	    std::string helloCommand = response.append("hello ") + nick + ", how are you ?" + std::string("\r\n");
	    send(botSocket, helloCommand.c_str(), helloCommand.size(), 0);
	}
	else if (message.find("good") != std::string::npos)
	{
	    std::string goodCommand = response.append("nice ") + nick + " !\r\n";
	    send(botSocket, goodCommand.c_str(), goodCommand.size(), 0);
	}
	else if (message.find("bad") != std::string::npos || message.find("not good") != std::string::npos)
	{
	    std::string badCommand = response.append("sad to here that 😢") + nick + " hope to be well soon" + " ...\r\n";
	    send(botSocket, badCommand.c_str(), badCommand.size(), 0);
	}
	else if (message.find("give me quote") != std::string::npos || message.find("qoute pls") != std::string::npos)
	{
    	int random_number = rand() % 200;
		// std::cout << "***hello  " << random_number <<"    "<< quotes[random_number] <<std::endl;
	    std::string badCommand = response.append(quotes[random_number]) + " ...\r\n";
	    send(botSocket, badCommand.c_str(), badCommand.size(), 0);
	}
	// else if (message.find("man") != std::string::npos || message.find("manual") != std::string::npos)
	// {
    // 	std::string manCommand = "just command here is availabe and you can you use it ...\r\n";
	// 								// manCommand.append("PING : you can use this command \r\n");
	// 								// manCommand.append("hello : you can use this command to greeting bot \r\n");
	// 								// manCommand.append("good : this command you can use it to reply the bot is ask about youself and want to answer you're good \r\n");
	// 								// manCommand.append("bad or not good : you can use one of these to command to reply the bot is ask about youself and want to answer you're not good \r\n");
	// 								// manCommand.append("give me quote or quote pls : you can use this command to ask our bot to give you a quote \r\n"); 

	//     send(botSocket, manCommand.c_str(), manCommand.size(), 0);
	// }
	// else
	// {
	// 	std::string wrongCommand = "hello hello! your command " + message + "is not availabe please try tab : man or manual \r\n have good day dud 😉!";
	// 	send(botSocket, wrongCommand.c_str(), wrongCommand.size(), 0);
	// }
	return (0);
}