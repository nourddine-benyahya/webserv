
#include "ServerMonitor.hpp"

void ls()
{
	system("leaks server");
}

int main(int ac, char **av)
{
	// at exit to check leaks
	// atexit(ls);
	try
	{
		if (ac > 2)
		{
			Logger(Logger::ERROR, "Number of args is not valid");
			return (EXIT_FAILURE);
		}
		std::string path = "default.conf";
		if (ac == 2)
			path = av[ac - 1];
		ServerMonitor *serverMonitor = ServerMonitor::getInstance();
		parseConfig(path);

		serverMonitor->run();
		delete ServerMonitor::getInstance();
		Logger(Logger::INFO, "Exited[0] : Server Terminated");
		return (EXIT_SUCCESS);
	}
	catch (std::exception &e)
	{
		Logger(Logger::ERROR, e.what());
		delete ServerMonitor::getInstance();
		Logger(Logger::NOTICE, "Exited[1] : Server failed");
		return (EXIT_FAILURE);
	}
	return 0;
}