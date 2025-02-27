
SRC= main.cpp	server/Server.cpp	server/Config.cpp	server/ServerMonitor.cpp  \
	request/srcs/parsFuncs.cpp	request/srcs/requestBody.cpp	request/srcs/requestLine.cpp	request/srcs/request.cpp request/srcs/requestHeader.cpp \
 	CGI/srcs/cgi.cpp   parser/srcs/parser.cpp  parser/srcs/parseServer.cpp parser/srcs/tokenizer.cpp Logger/Logger.cpp response/srcs/response.cpp

OBJ= $(SRC:.cpp=.o)

NAME= webserv

CC= c++

INCLUDES_FOLDERS=   server	Logger	CGI/includes \
			request/includes	parser/includes \
			response/includes

INCLUDES=   $(foreach d, $(INCLUDES_FOLDERS), -I $d)

CFLAGS= -Wall -Wextra -Werror --std=c++98 -fsanitize=address -g

all: $(NAME) phpInit
	

phpInit:
	@mkdir -p ~/my-php-config
	@cp ./cgi-bin/php.ini ~/my-php-config/
	@export PHPRC=~/my-php-config

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "Server Created ./"$(NAME)

%.o: %.cpp
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Object file created for:" $<

clean:
	@rm -f $(OBJ)
	@echo "Clearing obj -o files"

fclean: clean
	@rm -f $(NAME)
	@echo "Clearing Server "$(NAME)
	@rm -rf ~/my-php-config
	@echo "Removing my-php-config "

re: fclean all

.PHONY: all clean fclean re