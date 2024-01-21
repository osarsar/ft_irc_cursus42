SRCS =  main.cpp \
		./src/classes/servsocket.cpp\
		./src/classes/client.cpp\
		./src/commands/join.cpp\
		./src/classes/manage.cpp
		
FLAGS = -Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address

NAME = ircserv
OBJ =  $(SRCS:.cpp=.o)

%.o : %.cpp ./inc/servsocket.hpp ./inc/channel.hpp ./inc/client.hpp inc/manage.hpp
	@c++ $(FLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	@c++ $(FLAGS) $(OBJ) -o $@

all : $(NAME)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all