#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

std::string			retreiveMessageFromBuffer(char *buffer);
std::string			retreiveNickFromBuffer(char *buffer);
std::vector<std::string>	splitString(std::string str, const char *delim);
int				botBehavior(std::string message, int botSocket, std::string nick);
void				rebuildMessage(std::vector<std::string> &vector, int index);

#endif