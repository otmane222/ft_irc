
CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ircbot

SRC = $(wildcard *.cpp)

OBJ = $(SRC:.cpp=.o)

HEADER = $(wildcard %.hpp)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o : %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 
	
clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re

