#pragma once

#include <iostream>
#include <vector>
#include <algorithm>


class ircserver
{
	private:
		/* data */
	public:
		ircserver(/* args */);
		void	start(int serverSocket);
		~ircserver();
};
