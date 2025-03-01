
#pragma once
#include "Server.hpp"
#include "response.hpp"
#include <map>
#include <set>

#define BUFFER_SIZE 1000000
#define CHUNK 10


class Server;

class ServerAndPort {
	public:
	int port;
	Server* srv;
	ssize_t contentLength;
	bool isReady;
	std::stringstream recvBuffer;
	
		ServerAndPort() : port(0), srv(NULL), contentLength(-1), isReady(false) {}
		ServerAndPort(int port, Server *srv) : port(port), srv(srv), contentLength(-1), isReady(false) {}
	
		 ServerAndPort(const ServerAndPort& other) 
        : port(other.port), srv(other.srv), contentLength(other.contentLength), isReady(other.isReady) {
        recvBuffer.str(other.recvBuffer.str());
    }

		ServerAndPort& operator=(const ServerAndPort& other) {
			if (this != &other) {
				port = other.port;
				srv = other.srv;
				contentLength = other.contentLength;
				recvBuffer.str(other.recvBuffer.str());
				isReady = other.isReady;
			}
			return *this;
		}
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