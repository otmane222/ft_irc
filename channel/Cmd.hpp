// Cmd.hpp file
#pragma once
#include <iostream>
#include <vector>

class	Cmd
{
	public:
		Cmd(std::string	= "");
		
		~Cmd();
	private :
		std::string _cmd;
		std::string _params;
};

