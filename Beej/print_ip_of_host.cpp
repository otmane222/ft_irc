/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ip_of_host.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obahi <obahi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 09:11:39 by obahi             #+#    #+#             */
/*   Updated: 2023/12/30 10:04:46 by obahi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

int	main(int argc, char **argv)
{
	addrinfo 	hints;
	addrinfo 	*res;
	int			ret;

	if (argc != 2)
		return 1;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	ret = getaddrinfo(*(argv + 1), NULL, &hints, &res);
	if (ret)
		std::cerr << "failure" << std::endl;
	addrinfo *i;
	i = res;
	while (i)
	{
		if (i->ai_family == AF_INET)
		{
			sockaddr_in *ai_addr = reinterpret_cast<sockaddr_in *>(i->ai_addr);
			// char ÷ip4[INET_ADDRSTRLEN];
			char	*ip4 = inet_ntoa(ai_addr->sin_addr);
			std::cout << "ipv4 = " << ip4 << std::endl;
			// free(ip4);
		}
		i = i->ai_next;
	}
	freeaddrinfo(res);
}