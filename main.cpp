
#include "ServerMonitor.hpp"

void ls()
{
	system("leaks server");
}

int main(int ac, char **av)
{
	// at exit to check leaks
	// atexit(ls);
	/*
		* Parsing
			* add multiports
			* add log file
		* CGI
			* use root upload or root where to upload
	
		* Server
			* rechecking the health of the servers

	
	*/

	try
	{
		std::string path = "default.conf";
		if (ac == 2)
			path = av[ac - 1];
		ServerMonitor *serverMonitor = ServerMonitor::getInstance();
		parseConfig(path);

		serverMonitor->run();
		delete ServerMonitor::getInstance();
	}
	catch (std::exception &e)
	{
		Logger(Logger::ERROR, e.what());
		delete ServerMonitor::getInstance();
		Logger(Logger::NOTICE, "Exited[1] : Server failed");
		while(1);
		return (EXIT_FAILURE);
	}
	return 0;
}