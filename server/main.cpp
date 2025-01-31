
#include "ServerMonitor.hpp"

void ls(){
	system("leaks server");
}

int main() {
	atexit(ls);


	try {
		ServerMonitor *serverMonitor = ServerMonitor::getInstance();

		Server::Config()
			.setName("abounab.42.fr")
			.setPort(9090)
			.build();

		Server::Config()
			.setPort(80)
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