
#pragma once
#include "Server.hpp"

class Server;

class ServerMonitor {
	static ServerMonitor* instance;

	fd_set master_set;
	int maxFds;

	ServerMonitor();
	// debuger
	void printSet(fd_set&);
	public:
		~ServerMonitor();
		static ServerMonitor& getInstance();
	    void run();
		void addServer(Server server);
		// void setFD(int sockFd);


		// void createServer();
		// void clearFD(int sockfd);


		class ServerMonitorException : public std::exception {
			std::string msg;
			public :
				const char * what() const throw();
				ServerMonitorException(std::string);
				~ServerMonitorException() throw();
		};
};