SRCS =  main.cpp \
		servsocket.cpp\
		
FLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ircserv
OBJ =  $(SRCS:.cpp=.o)

%.o : %.cpp servsocket.hpp
	@c++ $(FLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	@c++ $(FLAGS) $(OBJ) -o $@

all : $(NAME)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all