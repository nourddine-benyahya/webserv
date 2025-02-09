
SRC= server/main.cpp response/srcs/response.cpp ./request/srcs/parsFuncs.cpp request/srcs/requestBody.cpp request/srcs/requestLine.cpp request/srcs/request.cpp request/srcs/requestHeader.cpp \
	parser/parser.cpp server/Server.cpp  server/Config.cpp server/ServerMonitor.cpp Logger/Logger.cpp CGI/srcs/cgi.cpp

OBJ= $(SRC:.cpp=.o)

NAME= webserv

CC= c++

CFLAGS=  -I ./request/includes/ -I ./response/includes/ -I ./parser/  -I ./server/ -I ./Logger/ -I ./CGI/includes/ # -fsanitize=address -g

all: $(NAME)
	mkdir -p ~/my-php-config
	cp ./cgi-bin/php.ini ~/my-php-config/
	export PHPRC=~/my-php-config
	# source ~/.zshrc

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -rf ~/my-php-config

re: fclean all

.PHONY: all clean fclean re