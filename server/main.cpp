
#include "ServerMonitor.hpp"

int main() {
	try {
		ServerMonitor serverMonitor = ServerMonitor::getInstance();
		Server server = Server::Builder()
									.setPort(8080)
									.build();
		// server.readServerInfo();
		serverMonitor.addServer(server);
		serverMonitor.run();
	} catch (std::exception& e){
		std::cerr 
			<< e.what() 
			<< std::endl;
		exit(EXIT_FAILURE);
	}
	return 0;
}