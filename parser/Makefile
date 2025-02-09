SRC= ./srcs/parser.cpp ./srcs/parseRoute.cpp ./srcs/parseServer.cpp ./srcs/tokenizer.cpp ../server/Server.cpp ../server/ServerMonitor.cpp  ../server/Location.cpp ../server/Config.cpp ../Logger/Logger.cpp

OBJ= $(SRC:.cpp=.o)

NAME= webserv
CC= c++

CFLAGS=  -I ../server/ -I ../request/includes/ -I includes/ # -fsanitize=address -g

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re