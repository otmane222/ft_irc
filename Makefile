NAME = ircserv

SRC = 	main.cpp \
		src/ircserver.cpp \
		src/command.cpp \
		src/socket.cpp \

OBJS = $(SRC:.cpp=.o)

FLAGS = -Werror -Wextra -Wall -std=c++98

CC = c++

RM = rm -rf

HEADERS = src/ircserver.hpp src/command.hpp src/socket.hpp

all: $(NAME) $(OBJS) $(HEADERS)

%.o: %.cpp $(HEADERS)
	$(CC) -c $(FLAGS) $< -o $@

$(NAME): $(OBJS) $(HEADERS)
	$(CC)  $(FLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean