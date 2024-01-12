// Msg.cpp file
#include "Msg.hpp" 

Msg::Msg(std::string msg)
{
	_msg = msg;
	cmds.push_back("PING");
	cmds.push_back("PONG");
	cmds.push_back("PASS");
	cmds.push_back("USER");
	cmds.push_back("NICK");
	cmds.push_back("JOIN");
	cmds.push_back("PRIVMSG");
	cmds.push_back("KICK");
	cmds.push_back("INVITE");
	cmds.push_back("TOPIC");
	cmds.push_back("MODE");
}

Msg::~Msg()
{

}

std::string	Msg::get_msg() const {return _msg;}
void	Msg::set_msg(std::string & msg) { _msg = msg;}

bool	Msg::is_valid() const
{
	std::stringstream ss(_msg);
	std::word

}

Cmd&	Msg::get_cmd() const
{


}
