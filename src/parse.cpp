#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "ircserver.hpp"
#include "socketManager.hpp"

std::vector<std::string> split(const std::string& input, char delimiter)
{
	std::vector<std::string>	result;
	std::istringstream			stream(input);
	std::string					token;

	while (std::getline(stream, token, delimiter))
		result.push_back(token);

	return (result);
}

// // std::cout
// // <<
// // s.size()
// // <<
// // std::endl
// // ;

// int	passwordcheck(std::string line, std::string password)
// {
// 	std::vector<std::string>	s = split(line, ' ');
// 	if ((s.size() != 2) || (s[0] != "PASS") || (s[1] != password))
// 		return (0);
// 	return (1);
// }

// int	nickname(std::string line)
// {
// 	std::vector<std::string>	s = split(line, ' ');
// 	if ((s.size() != 2) || (s[0] != "NICK"))
// 		return (0);
// 	return (1);
// }

// int	usr(std::string line)
// {
// 	std::vector<std::string>	s = split(line, ' ');
// 	if ((s.size() != 5) || (s[0] != "USER"))
// 		return (0);
// 	return (1);
// }

// void	parse_command(char* l, std::map<int, int>& steps, int i, std::string password)
// {
// 	std::string line = l;

// 	switch (steps[i])
// 	{
// 		case (0):
// 			if (passwordcheck(line, password))
// 				steps[i]++;
// 			break;
// 		case (1):
// 			if (nickname(line))
// 				steps[i]++;
// 			break;
// 		case (2):
// 			if (usr(line))
// 				steps[i]++;
// 			break;
// 		default:
// 			// command();
// 			break;
// 	}
// }

void	join(std::vector<std::string> param, client client, std::map<std::string, std::set<int> >& channels)
{
	(void)param;
	(void)client;
	(void)channels;
	/* std::string rpl[5] = {RPL_JOIN(nick, hostname, channelName, "10.11.4.1"),\
							RPL_TOPIC(hostname, nick, channelName, topicsetter,topicsetter), \
							RPL_TOPICWHOTIME(channelName, time, nick, hostname, channelName),\
							RPL_NAMREPLY(hostname, client, channelName, nick),\
							RPL_ENDOFNAMES(hostname, nick, channelName)};
	*/
}

void	parse_command(std::string line, std::map<int, client>& clients, std::vector<pollfd>& pollfds, std::vector<int>& fds, int i, std::map<std::string, std::set<int> >& channels)
{
	(void)line;
	(void)clients;
	(void)channels;

	std::vector<std::string>	v = split(line, ' ');
	if (v[0] == "QUIT\n")
	{
		std:: cout << clients[pollfds[i].fd].getUserName() << " has quit" << std::endl;
		close(pollfds[i].fd);
		pollfds.erase(pollfds.begin() + i);
		clients.erase(pollfds[i].fd);
		fds.erase(fds.begin() + i);
	}
	if (v[0] == "JOIN")
	{
		// join(v, clients[pollfds[i].fd], channels);
		// std:: cout << clients[pollfds[i].fd].getHostName() << " has join " << v[1] << "channel" << std::endl;
		// channels[v[1]].insert(pollfds[i].fd);
		// std::string name[4] = {"RPL_TOPIC", "RPL_TOPICWHOTIME", "RPL_NAMREPLY", "RPL_ENDOFNAMES"};
		// std::string hostname = "mgggg";
		// std::string nick = "myname";
		// std::string channelName = "c1";
		// std::string client = "1";
		// std::string topicsetter = "none";
		// std::string time = "202201010000";
		
		// std::string rpl[5] = {RPL_JOIN(nick, hostname, channelName, "10.11.4.1"), RPL_TOPIC(hostname, nick, channelName, topicsetter,topicsetter), RPL_TOPICWHOTIME(channelName, time, nick, hostname, channelName), RPL_NAMREPLY(hostname, client, channelName, nick), RPL_ENDOFNAMES(hostname, nick, channelName)};
	
		// // std::string serverResponse = RPL_TOPICWHOTIME("setter123", "202201010000", "user1", "irc.example.com", "#example_channel");
		// // const char *serverResponse = RPL_NAMREPLY("irc.example.com", "@user1 +user2 user3", "#example_channel", "nick123");
		// // std::string rpl[1] = {RPL_JOIN(nick, hostname, channelName, "10.11.4.1")};
	
		// for (size_t j = 0; j < 5; j++)
		// {
		// 	ssize_t bytes_sent = send(pollfds[i].fd, rpl[j].c_str(), strlen(rpl[j].c_str()), 0);
		// 	if (bytes_sent == -1)
		// 	{
		// 		perror("send");
		// 	}
		// }
	}
}
