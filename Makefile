
SRC= main.cpp	server/Server.cpp	server/Config.cpp	server/ServerMonitor.cpp  \
	request/srcs/parsFuncs.cpp	request/srcs/requestBody.cpp	request/srcs/requestLine.cpp	request/srcs/request.cpp request/srcs/requestHeader.cpp \
 	CGI/srcs/cgi.cpp   parser/srcs/parser.cpp  parser/srcs/parseServer.cpp parser/srcs/tokenizer.cpp Logger/Logger.cpp response/srcs/response.cpp

OBJ= $(SRC:.cpp=.o)

NAME= webserv

CC= c++

INCLUDES_FOLDERS=   server	Logger	CGI/includes \
			request/includes	parser/includes \
			response/includes
HEADERS = ./CGI/includes/cgi.hpp ./Logger/Logger.hpp ./parser/includes/parser.hpp ./request/includes/exeptions.hpp ./request/includes/parsFuncs.hpp ./request/includes/request.hpp ./request/includes/requestBody.hpp ./request/includes/requestHeader.hpp ./request/includes/requestLine.hpp ./response/includes/response.hpp ./server/Route.hpp ./server/Server.hpp ./server/ServerMonitor.hpp
INCLUDES=   $(foreach d, $(INCLUDES_FOLDERS), -I $d)

CFLAGS= -Wall -Wextra -Werror --std=c++98

all: $(NAME) phpInit
	

phpInit:
	@mkdir -p ~/my-php-config
	@cp ./cgi-bin/php.ini ~/my-php-config/
	@export PHPRC=~/my-php-config

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "Server Created ./"$(NAME)

%.o: %.cpp $(HEADERS)
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