SRC= src/main.cpp src/response.cpp ./request/srcs/parsFuncs.cpp request/srcs/requestBody.cpp request/srcs/requestLine.cpp request/srcs/request.cpp request/srcs/requestHeader.cpp

OBJ= $(SRC:.cpp=.o)

NAME= webserv
CC= c++

CFLAGS= -Wall -Wextra -Werror -I ./request/includes/ # -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) #-I ./request/includes/

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re