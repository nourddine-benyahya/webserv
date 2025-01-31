CPP=c++

CPPFLAGS= -Wall -Wextra -Werror -std=c++98

NAME= server

SRCS= main.cpp Server.cpp ServerMonitor.cpp Config.cpp ../Logger/Logger.cpp

INCLD=  Server.hpp ServerMonitor.hpp ../Logger/Logger.hpp

OBJS= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME) : $(OBJS)
			$(CPP) $(OBJS) -o $(NAME)

%.o:%.cpp $(INCLD)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS)

fclean : clean
		rm -f $(NAME)

re : fclean all