#include <iostream>
#include <fcntl.h>

void f(int = 0, std::string = "default", double = 3.14);
int	main()
{
	f(5, "hello", 12);

}

void f(int i, std::string s, double f)
{
	std::cout << i << std::endl;
	std::cout << s << std::endl;
	std::cout << f << std::endl;
}