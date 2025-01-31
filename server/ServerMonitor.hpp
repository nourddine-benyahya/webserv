
#pragma once
#include "Config.hpp"
#include <set>

class Server;

class ServerMonitor {
	static ServerMonitor* instance;

	fd_set master_set;
	int maxFds;
	std::set<int> sockets;

	ServerMonitor();
	// debuger
	void printSet(fd_set&);
	public:
		~ServerMonitor();
		static ServerMonitor& getInstance();
	    void run();
		void addServer(Server server);


		class ServerMonitorException : public std::exception {
			std::string msg;
			public :
				const char * what() const throw();
				ServerMonitorException(std::string);
				~ServerMonitorException() throw();
		};
};