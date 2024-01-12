#include <iostream>
#include <sstream> 

int main(int argc, char ** argv)
{
	std::string s(*(argv + 1));
	std::stringstream ss(s);
	std::string world;
	while (!ss.eof())
	{
		getline(ss, world, ':');
		std::cout << "#" <<world << "#" << std::endl;
	}
}