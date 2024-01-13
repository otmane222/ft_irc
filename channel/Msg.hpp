// Msg.hpp file
#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include "Cmd.hpp"

class	Msg
{
	public:
		Msg(std::string = "");
		~Msg();

		std::string get_msg() const;
		void		set_msg(std::string &);
		bool		is_valid() const;
		Cmd&		get_cmd() const;
	
	private :
		std::string _msg;
		std::vector<std::string> cmds;
};

