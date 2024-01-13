#include <iostream>
#include <sstream> 

std::string	get_token(std::string& str)
{
	// std::cout << str << std::endl;
	if (str.empty())
		return ("");
	std::string	ret;
	size_t	valid_c_position = str.find_first_not_of(' ');
	if (valid_c_position != std::string::npos)
	{
		str.erase(0, valid_c_position);
		if (str[0] == ':')
		{
			ret = str.substr(1);
			str.erase();
		}
		else
		{
			size_t	space_position = str.find_first_of(' ');
			if (space_position != std::string::npos)
			{
				ret = str.substr(0, space_position);
				str.erase(0, space_position);
			}
			else
			{
				ret = str;
				str.erase();
			}
		}
		return (ret);
	}
	return ("");
}
int main()
{
	std::string s("join ");
	std::stringstream ss(s);
	std::string str;
	getline(ss, str, ' ');
	if (str.empty())
		std::cout << "empty" << std::endl;
	else
		std::cout << str << std::endl;
	getline(ss, str, ' ');
	if (str.empty())
		std::cout << "empty" << std::endl;
	else
		std::cout << str << std::endl;
	getline(ss, str, ' ');
	if (str.empty())
		std::cout << "empty" << std::endl;
	else
		std::cout << str << std::endl;
	getline(ss, str, ' ');
	if (str.empty())
		std::cout << "empty" << std::endl;
	else
		std::cout << str << std::endl;

}