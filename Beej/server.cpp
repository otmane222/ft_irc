/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obahi <obahi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 15:47:24 by obahi             #+#    #+#             */
/*   Updated: 2024/01/03 11:02:22 by obahi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

typedef struct addrinfo addrinfo;
typedef struct sockaddr_storage sockaddr_storage;
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;

#define BACKLOG 128
#define MAX 10
std::string	read_line(int fd) throw (std::string)
{
	std::string	str;
	std::string	tmp;
	int				count;
	char			buff[100];
	size_t		n;

	n = std::string::npos;
	do
	{
		bzero(buff, 100);
		count = recv(fd, buff, 99, 0);
		if (count > 0)
		{
			tmp = buff;
			size_t	n = tmp.find('\n');
			if (n != std::string::npos)
			{
				str += tmp.substr(0, n);
				break;
			}
			else
				str += tmp;
		}
		else if (count == 0)
		{
			close(fd);
			throw std::string(strerror(errno));
		}
		else
		{
			close(fd);
			throw std::string(strerror(errno));
		}
	} while (n == std::string::npos);
	return str;
}

int	main(int argc, char **argv)
{
	addrinfo	_hints;
	addrinfo	*_res;
	int			ret;
	int			sock_fd;
	char		buff[MAX];
	
	if (argc != 2)
		return 1;
	memset(&_hints, 0, sizeof _hints);
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	ret = getaddrinfo(NULL, *(argv + 1), &_hints, &_res);
	if (ret)
	{
		std::cerr << "failure!!" << std::endl;
		return 1;
	}
	sock_fd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (sock_fd < 0)
	{
		std::cerr << "failure!!!" << std::endl;
		return 1;
	}
	int yes=1;
	if (setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
    perror("setsockopt");
    exit(1);
} 
	if (bind(sock_fd, _res->ai_addr, _res->ai_addrlen) < 0)
	{
		std::cerr << "failure on binding : " << strerror(errno) << std::endl;
		return 1;
	}
	if (listen(sock_fd, BACKLOG) < 0)
	{
		std::cerr << "failure on listening" << std::endl;
		return 1;
	}
	sockaddr_in *server_addr = reinterpret_cast <sockaddr_in *> (_res);
	std::cout << "Server is listening on : " << inet_ntoa(server_addr->sin_addr) << std::endl; 
	sockaddr_in their_addr;
	socklen_t addr_size = sizeof their_addr;
	int	new_fd = accept(sock_fd, reinterpret_cast<sockaddr *> (&their_addr), &addr_size);
	if (new_fd < 0)
	{
		std::cerr << "failure on accepting" << std::endl;
		return 1;
	}
	std::cout << "Connection accepted with : " << inet_ntoa(their_addr.sin_addr) << std::endl;
	while (1)
	{
		bzero(buff, MAX);
		// size_t n = recv(new_fd, buff, MAX, 0);
		// 	std::cout << "from client : " << msg << std::endl;
		try
		{
			// std::cout << "hh" << std::endl;
			std::string  msg = read_line(new_fd);
			std::cout << "from client : " << msg << std::endl;
			int	l = msg.length();
			send(new_fd, &l, sizeof l, 0);
			send(new_fd, "message received succesfully\n", 31, 0);
		}
		catch (std::string &s)
		{
			std::cerr << s << std::endl;
			break;
		}
	}
    close(sock_fd);
    return 0; 
}
