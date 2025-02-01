
#include "ServerMonitor.hpp"

void ls(){
	system("leaks server");
}

int main() {
	// at exit to check leaks 
	atexit(ls);


	try {
		ServerMonitor *serverMonitor = ServerMonitor::getInstance();
		Server::Config srv;
			srv.setName("abounab.42.fr");
			/*
				std::map<std::string, Location>
				Location(srv)
					.setName("/");
					.setFile("root");
					.setFile("root");
					.build();//
			*/
			// map<fd, port> every port have it"s own fdsock , when selecting from the fd ,we get the 
			srv.setPort(9090);
			srv.setPort(50);
			// srv.setPort(30);
			/*
				srv.setRoot(std::string)
				srv.setIndex(std::string)
				srv.setLogs(std::string)
				srv.setErrorPage(int status, std::string file)
			*/
			srv.build();


		Server::Config()
			// .setPort(80)
			.build();

		Server::Config()
			.setPort(1000)
			.build();

		serverMonitor->run();
		delete ServerMonitor::getInstance();
	} catch (std::exception& e){
		std::cerr
			<< e.what() 
			<< std::endl;
		delete ServerMonitor::getInstance();
		return (EXIT_FAILURE);
	}
	return 0;
}