SRCS =  main.cpp \
		./src/classes/servsocket.cpp\
		./src/classes/client.cpp\
		./src/commands/join.cpp\
		./src/commands/mode.cpp\
		./src/commands/privmsg.cpp\
		./src/classes/manage.cpp\
		./src/commands/z_Kick.cpp\
		./src/commands/z_Invite.cpp\
		./src/commands/z_Topic.cpp
		
FLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ircserv
OBJ =  $(SRCS:.cpp=.o)

%.o : %.cpp ./inc/servsocket.hpp ./inc/channel.hpp ./inc/client.hpp inc/manage.hpp inc/privmsg.hpp inc/error.hpp inc/topic.hpp inc/invite.hpp inc/Kick.hpp inc/replies.hpp
	@c++ $(FLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	@c++ $(FLAGS) $(OBJ) -o $@

all : $(NAME)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all