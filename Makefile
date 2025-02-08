SRC= response/srcs/main.cpp response/srcs/response.cpp ./request/srcs/parsFuncs.cpp request/srcs/requestBody.cpp request/srcs/requestLine.cpp request/srcs/request.cpp request/srcs/requestHeader.cpp

OBJ= $(SRC:.cpp=.o)

NAME= webserv
CC= c++

CFLAGS=  -I ./request/includes/ -I ./response/includes/ # -fsanitize=address -g

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