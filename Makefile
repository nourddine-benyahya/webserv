SRC= server/main.cpp response/srcs/response.cpp ./request/srcs/parsFuncs.cpp request/srcs/requestBody.cpp request/srcs/requestLine.cpp request/srcs/request.cpp request/srcs/requestHeader.cpp \
		server/Server.cpp  server/Config.cpp server/ServerMonitor.cpp  ./parser/srcs/parser.cpp  ./parser/srcs/parseServer.cpp ./parser/srcs/tokenizer.cpp Logger/Logger.cpp
OBJ= $(SRC:.cpp=.o)

NAME= webserv
CC= c++

CFLAGS=  -I ./request/includes/ -I ./response/includes/ -I ./parser/includes  -I ./server/ -I ./Logger/# -fsanitize=address -g

all: $(NAME)

$(NAME): $(OBJ)
	@ $(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	@ $(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re