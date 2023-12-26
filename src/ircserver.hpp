#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <stdlib.h>
#include "client.hpp"


class ircserver
{
	private:
		/* data */
	public:
		ircserver(/* args */);
		void	start(int serverSocket);
		~ircserver();
};
