/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obahi <obahi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 09:15:29 by obahi             #+#    #+#             */
/*   Updated: 2023/12/31 09:55:44 by obahi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

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
		count = read(fd, buff, 99);
		if (count > 0)
		{
			tmp = buff;
			size_t	n = tmp.find('\n');
			if (n != std::string::npos)
				str += tmp.substr(0, n);
			else
				str += tmp;
		}
		else if (count == 0)
			break;
		else
		{
			if (errno == EINTR) /* Interrupted --> restart read() */
				continue;
			else
				throw std::string(strerror(errno));
		}
	} while (n == std::string::npos);
	return str;
}