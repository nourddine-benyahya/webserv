#pragma once

/*
	@ressources:
		# Articles:
			- https://www.lowtek.com/sockets/
			- https://beej.us/guide/bgnet/html/index-wide.html
			- https://ruslanspivak.com/lsbaws-part1/
			- https://www.ibm.com/docs/en/i/7.2?topic=designs-example-nonblocking-io-select
			- https://medium.com/@md.abir1203/visual-nuts-and-bolts-of-42-webserver-b381abec4df3
			- https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
			- https://ncona.com/2019/04/building-a-simple-server-with-cpp/
			- https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm

		# Videos :
			- https://www.youtube.com/watch?v=-utm73RxNo4
			- https://www.youtube.com/watch?v=XXfdzwEsxFk
			- https://www.youtube.com/watch?v=hJHvdBlSxug
			- https://www.youtube.com/watch?v=3b_TAYtzuho
			- https://www.youtube.com/watch?v=9J1nJOivdyw
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#define BUFFER_SIZE 1024

class Server {
private:
	int server_fd;
	struct sockaddr_in address;


	// Private constructor
	Server(struct sockaddr_in&);

	void initServer();

public:
	// for debug
	void readServerInfo();
	~Server();

	int getServerFd();



		class ServerException : public std::exception {
			std::string msg;
			public :
				const char * what() const throw();
				ServerException(std::string);
				~ServerException() throw();
		};

		// Builder Design Pattern
		class Builder {
		private:
			struct sockaddr_in address;

		public:
		// Starting
			Builder();

		// Characteristics
			Builder& setPort(int port);

		// final Build
			Server build();
		};
};
