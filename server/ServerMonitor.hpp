
#pragma once
#include "Server.hpp"
#include <map>
#include <set>

#define BUFFER_SIZE 1000000
#define CHUNK 10


class Server;

struct ServerAndPort {
		int port;
		Server *srv;
		bool isReady;
		int contentLength;
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
	    void engine();
		void addServer(Server *server);

		void	acceptNewConnections(int , std::map<int, ServerAndPort> &) ;
		int		getContentLenght(std::string) ;
		void	fillRecvBuffer(ServerAndPort&, std::string ) ;
		int		returnRecvBuffer(int, std::string &) ;
		void	handleError(int, int, std::map<int, ServerAndPort> &) ;



		class ServerMonitorException : public std::exception {
			std::string msg;
			public :
				const char * what() const throw();
				ServerMonitorException(std::string);
				~ServerMonitorException() throw();
		};
};