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
	// Cleanups done here
	sockets.clear();
}

ServerMonitor &ServerMonitor::getInstance()
{
	if (instance == NULL)
		instance = new ServerMonitor();
	return *instance;
}

void ServerMonitor::addServer(Server server)
{
	int fd = server.getServerFd();
	if (fd < 0){
		throw ServerMonitorException("Invalid Server fd");
	}
	FD_SET(fd, &master_set);
	if (fd > maxFds)
		maxFds = fd;
	sockets.insert(fd);
}

// tmp function
// static std::string readFile(const std::string& filePath) {
//     std::ifstream file(filePath);
//     std::stringstream buffer;
//     buffer << file.rdbuf();
//     return buffer.str();
// }

void ServerMonitor::run()
{
	// creating set of fds
	fd_set working_set;

	struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);

	FD_ZERO(&working_set);

	while (true)
	{
		// copying the master set to tmp one
		memcpy(&working_set, &master_set, sizeof(master_set)); // cross-platform alternative to FD_COPY
		// debugger
		printSet(working_set);
		// polling on the sets we have
		if (select(maxFds + 1, &working_set, NULL, NULL, NULL) < 0) {
			throw ServerMonitorException("Select error");
		}

		std::cout << "starting the loop on fds " << std::endl;
		// once returned we will check on the ready socks
		for (int i = 0; i <= maxFds; ++i)
		{
			// if fd sock exist and ready for listening
			std::cout << "inside the loop on : " << i << std::endl;
			if (FD_ISSET(i, &working_set))
			{
				std::cout << "inside iset " << i << std::endl;
				// Check if it's one of the listening sockets
				if (sockets.find( i ) != sockets.end())
				{
					// Accept new connection
					int new_socket = accept(i, (struct sockaddr *)&client_address, &client_address_len);
					if (new_socket < 0)
					{
						std::cerr << "Accept Error: " << std::endl;
						continue;
					}
					// Add new socket to master set
					FD_SET(new_socket, &master_set);
					if (new_socket > maxFds)
					{
						maxFds = new_socket;
					}
					// sockets.insert(new_socket);
					std::cout << "New connection accepted: " << new_socket << std::endl;
				}
				else
				{
					// we define a buffer to get what received from the sock
					char buffer[BUFFER_SIZE];
					int bytes_read = recv(i, buffer, sizeof(buffer), 0);
					if (bytes_read <= 0)
					{
						// Handle disconnection or error
						if (bytes_read == 0)
							std::cout << "Connection closed: " << i << std::endl;
						else
							std::cerr << "Recv Error  "<< std::endl;
						close(i);
						// sockets.erase(i);	
						FD_CLR(i, &master_set);
					} else {
						buffer[bytes_read] = 0;
						// std::cout << "Received: " << buffer << std::endl;
						std::cout << "Received (" << bytes_read << "): " << buffer << std::endl;
						// std::string htmlContent = readFile("index.html");
						std::stringstream ss;
						std::string response = "<h1>Hello, world!</h1>";
						ss << "HTTP/1.1 200 OK\r\n";
						ss << "Content-Type: text/html\r\n";
						// ss << "Content-Length: " << htmlContent.size() << "\r\n\r\n";
						ss << "Content-Length: " << response.size() << "\r\n\r\n";
						// ss << htmlContent;
						ss << response;


						// std::cout << indexHtml << std::endl;
						send(i, ss.str().c_str(), ss.str().size(), 0);
						ss.clear();
						// send(i, response.c_str(), response.size(), 0);// Process the received data
						std::cout << "Received data: " << buffer << std::endl;
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

ServerMonitor::ServerMonitorException::~ServerMonitorException() throw()
{
}

const char *ServerMonitor::ServerMonitorException::what() const throw()
{
	return msg.c_str();
}