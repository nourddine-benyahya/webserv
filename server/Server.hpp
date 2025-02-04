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

#include <ostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <map>


#include "../Logger/Logger.hpp"


#define BUFFER_SIZE 1024

class Server {
public:
	// for debug
	void readServerInfo();
	void closeSocks();


			class ServerException : public std::exception {
				std::string msg;
				public :
					const char * what() const throw();
					ServerException(std::string);
					~ServerException() throw();
			};


		// Using API Config
			class Config {
				void create_sock();
				bool portRedifined;

				std::string fileIndex;
				std::map<int, int> sock_port;
				struct sockaddr_in address;
				std::string name;
				std::string logsFile;
				std::string rootFolder;
				std::map<int, std::string> errorPages;


				std::string readFile(std::string);
				std::string catRoot(std::string file);
			public:
				~Config();
				Config();
				Config(Config&);

				Config& setPort(int);
				Config& setName(std::string);
				Config& setIndex(std::string);

				Config& setRoot(std::string);
				Config& setLogs(std::string);
				Config& setErrorPage(int, std::string);

				void build();


				Config* clone();
				// getter
				struct sockaddr_in& getAddress() ;
				std::string& getName() ;
				std::string getIndex() ;
				std::map<int, int>& getSockets() ;
				std::string getRoot();
				std::string getLogs();
				std::map<int, std::string> getErrorPages();
				std::string getErrorPage(int);
				int getPort() ;
			};

	Server::Config* getConfig();

	int getServerFd();
	void setRecvBuffer(std::string);
	std::string getRecvBuffer();
	~Server();



		std::string recvBuffer;
	private:
		int server_fd;
		// std::set<int> server_fd;
		//dependencies to build a server
		Config *conf;

		Server(Config*);
		void initServer();
};
