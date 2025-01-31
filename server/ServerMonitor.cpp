#include "ServerMonitor.hpp"

ServerMonitor *ServerMonitor::instance = NULL;

void ServerMonitor::printSet(fd_set &ms)
{
	std::cout << "set file descriptors: ";
	for (int i = 0; i <= maxFds; ++i)
	{
		if (FD_ISSET(i, &ms))
		{
			std::cout << i << " ";
		}
	}
	std::cout << std::endl;
}

ServerMonitor::ServerMonitor() : maxFds(-1)
{
	FD_ZERO(&master_set);
	sockets.clear();
}

ServerMonitor::~ServerMonitor()
{
	// Cleanups done here (delete all servers)
	for (std::map<int, Server*>::iterator it = sockets.begin(); it != sockets.end(); it++){
		delete sockets[it->first];
	}

	sockets.clear();
}

ServerMonitor *ServerMonitor::getInstance()
{
	if (instance == NULL)
		instance = new ServerMonitor();
	return instance;
}

void ServerMonitor::addServer(Server* server)
{
	int fd = server->getServerFd();
	if (fd < 0){
		throw ServerMonitorException("Invalid Server fd");
	}
	FD_SET(fd, &master_set);
	if (fd > maxFds)
		maxFds = fd;
	if (sockets.find(fd) == sockets.end())
		delete sockets[fd];
	sockets[fd] = server;
}


void ServerMonitor::update_maxFds() {
    maxFds = -1;
    for (int j = 0; j <= FD_SETSIZE; ++j) {
        if (FD_ISSET(j, &master_set) && j > maxFds) {
            maxFds = j;
        }
    }
}

void ServerMonitor::run()
{
	// creating set of fds
	fd_set read_set, write_set;
	std::map<int, Server *> tmpSockets;

	printSet(master_set);


	while (true)
	{
		FD_ZERO(&read_set);
		memcpy(&read_set, &master_set, sizeof(master_set)); 
		FD_ZERO(&write_set);
		memcpy(&write_set, &master_set, sizeof(master_set)); 
		if (select(maxFds + 1, &read_set, &write_set, NULL, NULL) < 0) {
			throw ServerMonitorException("Select error");
		}
		for (int i = 0; i <= maxFds; ++i)
		{
			if (FD_ISSET(i, &read_set)) {
				if (sockets.find( i ) != sockets.end()) {
					struct sockaddr_in client_address;
					socklen_t client_address_len = sizeof(client_address);

					int new_socket = accept(i, (struct sockaddr *)&client_address, &client_address_len);
					if (new_socket < 0) {
						// Logger(sockets[i]->getConfig()->getLogger(), Logger::ERROR,  "Accept Error");
						Logger(Logger::ERROR,  "Accept Error");
						continue ;
					}
					FD_SET(new_socket, &master_set);
					if (new_socket > maxFds)
						maxFds = new_socket;

					tmpSockets[new_socket] = sockets[i];
					{
						std::stringstream ss;
							ss << "WebSocket connection established with "
								<< tmpSockets[new_socket]->getConfig()->getName();
						// Logger(tmpSockets[new_socket]->getConfig()->getLogger(), Logger::INFO,  ss.str());
						Logger(Logger::INFO,  ss.str());
					}
				}
				else
				{
					char buffer[BUFFER_SIZE] = {0};
					int bytes_read = recv(i, buffer, sizeof(buffer), 0);

					std::stringstream ss;
						ss << "WebSocket message received from " 
							<< tmpSockets[i]->getConfig()->getName() + ":"
							<< tmpSockets[i]->getConfig()->getPort();
					// Logger(tmpSockets[i]->getConfig()->getLogger(), Logger::INFO,  ss.str());
					Logger(Logger::INFO,  ss.str());

					if (bytes_read <= 0)
					{
						// Handle disconnection or error
						if (bytes_read == 0)
							Logger( Logger::INFO,  "Connection closed");
							// Logger(tmpSockets[i]->getConfig()->getLogger(), Logger::INFO,  "Connection closed");
						else
							Logger( Logger::ERROR,  "Recv Error");
							// Logger(tmpSockets[i]->getConfig()->getLogger(), Logger::DEBUG,  "Recv Error");
						close(i);
						update_maxFds();
						tmpSockets.erase(i);
						FD_CLR(i, &master_set);
					} else {
						buffer[bytes_read] = 0;
						std::string response = "<h1>Hello, world!</h1>";
						std::stringstream ss;
							ss << "HTTP/1.1 200 OK";
						// Logger(tmpSockets[i]->getConfig()->getLogger(), Logger::DEBUG,  ss.str());
						Logger( Logger::DEBUG,  ss.str());
							ss << "\r\nContent-Type: text/html\r\n";
							ss << "Content-Length: " << response.size() << "\r\n\r\n";
							ss << response;
						if (FD_ISSET(i, &write_set)) {
							std::stringstream xx;
							xx << "Sender connection from socket "
								<< i;
							Logger( Logger::DEBUG,  xx.str());
                    		send(i, ss.str().c_str(), ss.str().size(), 0);
						}
					}
				}
			}
		}
	}
}

ServerMonitor::ServerMonitorException::ServerMonitorException(std::string msg) : msg("[Error::ServerMonitorException] : ")
{
	this->msg += msg;
}

ServerMonitor::ServerMonitorException::~ServerMonitorException() throw() {}

const char *ServerMonitor::ServerMonitorException::what() const throw()
{
	return msg.c_str();
}