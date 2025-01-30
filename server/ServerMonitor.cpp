#include "ServerMonitor.hpp"

ServerMonitor* ServerMonitor::instance = NULL;

void ServerMonitor::printSet(fd_set& ms) {
    std::cout << "set file descriptors: ";
    for (int i = 0; i <= maxFds; ++i) {
        if (FD_ISSET(i, &ms)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}




ServerMonitor::ServerMonitor() : maxFds(-1)  {
	FD_ZERO(&master_set);
}

ServerMonitor::~ServerMonitor() {
	// Cleanups done here
}

ServerMonitor& ServerMonitor::getInstance() {
	if (instance == NULL) {
		instance = new ServerMonitor();
	}
	return *instance;
}

void ServerMonitor::addServer(Server server){
	int fd = server.getServerFd();
	if (fd >= 0){
		FD_SET(fd, &master_set);
		if (fd > maxFds)
			maxFds = fd;
	} else {
		throw ServerMonitorException("Invalid Server fd");
	}
}

// tmp function
static std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void ServerMonitor::run() {
	// creating set of fds


	fd_set working_set;
	FD_ZERO(&working_set);



	while (true) {
		// copying the master set to tmp one
		FD_COPY(&master_set, &working_set);
		// debugger
		printSet(working_set);
		// polling on the sets we have
		int select_res = select(maxFds + 1, &working_set, NULL, NULL, NULL);
		if (select_res < 0) {
			std::cerr << strerror(errno) << " " << select_res << std::endl;
			throw ServerMonitorException("Select error");
		}

		std::cout << "starting the loop on fds " << std::endl;
		// once returned we will check on the ready socks
		for (int i = 0; i <= maxFds; ++i) {
			// if fd sock exist and ready for listening
			std::cout << "inside the loop on : " << i <<  std::endl;
			if (FD_ISSET(i, &working_set)) {
			std::cout << "inside iset " << i <<  std::endl;
					// we define a buffer to get what receieved from the sock
					char buffer[BUFFER_SIZE];
					int bytes_read = recv(i, buffer, sizeof(buffer), 0);
					if (bytes_read <= 0) {
						
						FD_CLR(i, &master_set);
						close(i);
					} else {
						buffer[bytes_read] = '\0';
						// std::cout << "Received: " << buffer << std::endl;
						std::cout << "Received (" << bytes_read << "): " << buffer << std::endl;
						std::string htmlContent = readFile("index.html");
						std::stringstream ss;
						// std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";
						ss << "HTTP/1.1 200 OK\r\n";
						ss << "Content-Type: text/html\r\n";
						ss << "Content-Length: " << htmlContent.size() << "\r\n\r\n";
						ss << htmlContent;


						// std::cout << indexHtml << std::endl;
						send(i, ss.str().c_str(), ss.str().size(), 0);
						ss.clear();
						// send(i, response.c_str(), response.size(), 0);
					}
			}
		}
	}
}



ServerMonitor::ServerMonitorException::ServerMonitorException(std::string msg) : msg("[Error::ServerMonitorException] : ") {
	this->msg += msg;
}

ServerMonitor::ServerMonitorException::~ServerMonitorException() throw() {
}

const char* ServerMonitor::ServerMonitorException::what() const throw() {
	return msg.c_str();
}