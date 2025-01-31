
#include "ServerMonitor.hpp"

int main() {
	try {
		ServerMonitor serverMonitor = ServerMonitor::getInstance();
		Server server = Config()
							.setPort(8080)
							.build();
		// server.readServerInfo();
		serverMonitor.addServer(server);
		serverMonitor.addServer(Config()
									.setPort(9090)
									.build());
		serverMonitor.run();
	} catch (std::exception& e){
		std::cerr
			<< e.what() 
			<< std::endl;
		return (EXIT_FAILURE);
	}
	return 0;
}