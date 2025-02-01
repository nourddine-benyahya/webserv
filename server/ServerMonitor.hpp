
#pragma once
#include "Server.hpp"
#include <map>

class Server;

struct ServerAndPort {
		int port;
		Server *srv;
	};

class ServerMonitor {
	static ServerMonitor* instance;

	fd_set master_set;
	int maxFds;

	

	std::map<int, Server *> sockets;

	ServerMonitor();
	void	update_maxFds();
	// debuger
	void printSet(fd_set&);
	public:
		~ServerMonitor();
		static ServerMonitor* getInstance();
	    void run();
		void addServer(Server *server);


		class ServerMonitorException : public std::exception {
			std::string msg;
			public :
				const char * what() const throw();
				ServerMonitorException(std::string);
				~ServerMonitorException() throw();
		};
};