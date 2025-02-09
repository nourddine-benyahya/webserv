
SRC= server/Server.cpp server/main.cpp   server/Config.cpp server/ServerMonitor.cpp  \
	./request/srcs/parsFuncs.cpp request/srcs/requestBody.cpp request/srcs/requestLine.cpp request/srcs/request.cpp request/srcs/requestHeader.cpp \
 		CGI/srcs/cgi.cpp   ./parser/srcs/parser.cpp  ./parser/srcs/parseServer.cpp ./parser/srcs/tokenizer.cpp Logger/Logger.cpp response/srcs/response.cpp

OBJ= $(SRC:.cpp=.o)

NAME= webserv

CC= c++

CFLAGS=   -I ./server/ -I ./Logger/ -I ./CGI/includes/ -I ./request/includes/  -I ./parser/includes -I ./response/includes/ -I ./Logger/ -I ./CGI/includes/ # -fsanitize=address -g

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