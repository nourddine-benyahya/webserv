#include "ServerMonitor.hpp"

ServerMonitor *ServerMonitor::instance = NULL;

static bool rootExist(std::string file){
	struct stat info;
	if (stat(file.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
		return false;
	}
	return true;
}

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
		mkdir("html", 0750);
	std::ofstream indexFile("html/index.html");
    if (indexFile.is_open()) {
        indexFile << "<!DOCTYPE html>\n"
                  << "<html lang=\"en\">\n"
                  << "<head>\n"
                  << "    <meta charset=\"UTF-8\">\n"
                  << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                  << "    <title>Welcome Page</title>\n"
                  << "    <style>\n"
                  << "        body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }\n"
                  << "        h1 { font-size: 50px; font-weight: bold; }\n"
                  << "        p { font-size: 20px; }\n"
                  << "    </style>\n"
                  << "</head>\n"
                  << "<body>\n"
                  << "    <h1>Welcome to the WebServ</h1>\n"
                  << "    <h2>Dedicace l had Chabab:</h2>\n"
                  << "    <p>Abdellah Bounab</p>\n"
                  << "    <p>Anas Mejdoub</p>\n"
                  << "    <p>Nourddine BenYahya</p>\n"
                  << "    <p>Abderrafia Askal</p>\n"
                  << "</body>\n"
                  << "</html>";
        indexFile.close();
    }
}

ServerMonitor::~ServerMonitor()
{
	// Cleanups done here (delete all servers)
	std::set<Server *> srvs;
	for (std::map<int, Server*>::iterator it = sockets.begin(); it != sockets.end(); it++){
		srvs.insert(sockets[it->first]);
	}
	for (std::set<Server*>::iterator it = srvs.begin(); it != srvs.end(); ++it) {
        delete *it;
    }

	// sockets.clear();
}

ServerMonitor *ServerMonitor::getInstance()
{
	if (instance == NULL)
		instance = new ServerMonitor();
	return instance;
}

void ServerMonitor::addServer(Server* server)
{
	if (!server)
		throw ServerMonitorException("Null Server");
	if (!rootExist(server->getConfig()->getRoot())){
		std::stringstream ss;
			ss << "Folder doesnt exists :" << server->getConfig()->getRoot()
				<< " - " << server->getConfig()->getName();
			Logger(Logger::ERROR, ss.str());
		return	(delete server);
	}
	std::map<int, int>::iterator it = server->getConfig()->getSockets().begin();
	while (it != server->getConfig()->getSockets().end()){

		FD_SET(it->first, &master_set);
		if (it->first > maxFds)
			maxFds = it->first;
		// if (sockets.find(it->first) == sockets.end())
		// 	delete sockets[it->first];
		sockets[it->first] = server;
		it++;
	}
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
	fd_set read_set, write_set;

	std::map<int, ServerAndPort> tmpSockets;

	// printSet(master_set);

// the throws have to be handled to not segv 

	while (maxFds > 0)
	{
		FD_ZERO(&read_set);
		memcpy(&read_set, &master_set, sizeof(master_set)); 
		FD_ZERO(&write_set);
		memcpy(&write_set, &master_set, sizeof(master_set)); 
		if (select(maxFds + 1, &read_set, &write_set, NULL, NULL) < 0) {
			throw ServerMonitorException("Select error");
		}
		for (int i = 3; i <= maxFds; ++i)
		{
			if (FD_ISSET(i, &read_set)) {
				if (sockets.find( i ) != sockets.end()) {
					struct sockaddr_in client_address;
					socklen_t client_address_len = sizeof(client_address);

					int new_socket = accept(i, (struct sockaddr *)&client_address, &client_address_len);
					if (new_socket < 0) {
						Logger(sockets[i], Logger::ERROR,  "Accept Error");
						continue ;
					}

					FD_SET(new_socket, &master_set);
					if (new_socket > maxFds)
						maxFds = new_socket;

					ServerAndPort tmp;
						tmp.port = sockets[i]->getConfig()->getSockets().find(i)->second;
						tmp.srv = sockets[i];
					tmpSockets[new_socket] = tmp;
					{
						std::stringstream ss;
							ss << "WebSocket connection established with "
								<< (tmpSockets[new_socket].srv)->getConfig()->getName();
						Logger(tmpSockets[new_socket].srv, Logger::INFO,  ss.str());
					}
				}
				else
				{
					char buffer[BUFFER_SIZE] = {0};
					int bytes_read = recv(i, buffer, sizeof(buffer), 0);

					{
						std::stringstream ss;
							ss << "WebSocket message received from " 
								<< (tmpSockets[i].srv)->getConfig()->getName() + ":"
								<< tmpSockets[i].port;
						Logger(tmpSockets[i].srv, Logger::INFO,  ss.str());
					}
					if (bytes_read <= 0)
					{
						// Handle disconnection or error
						if (bytes_read == 0)
							Logger(tmpSockets[i].srv, Logger::WARNING,  "Connection closed");
						else
							Logger(tmpSockets[i].srv, Logger::ERROR,  "Recv Error");
						tmpSockets.erase(i);
						FD_CLR(i, &master_set);
						update_maxFds();
						close(i);
					} else {
						buffer[bytes_read] = 0;
						tmpSockets[i].srv->setRecvBuffer(buffer);
						if (bytes_read < BUFFER_SIZE)
							// chunkedRemaining = i;
						// else {
							{
							std::string msgTwil = tmpSockets[i].srv->getRecvBuffer();
							/*
								try {
									response(request(buffer), server->config());
								} catch (){
									Logger(server[i], ERROR, e.what)
								}
							*/
						// this would be made inside the response
							std::string response;
							std::string valid = " OK";
							int status = 200;
							try {
								response = tmpSockets[i].srv->getConfig()->getIndex();
							} catch (std::exception& e){
								status = 404;
								response = tmpSockets[i].srv->getConfig()->getErrorPage(status);
								valid = " KO";
								Logger(tmpSockets[i].srv, Logger::WARNING, e.what());
							}
							
							if (FD_ISSET(i, &write_set)) {

								std::stringstream logs;
								logs << "Sender connection from socket "
									<< i << " with status " << status << valid;
								
								std::stringstream ss;
									ss << "HTTP/1.1 " << status << valid;
									ss << "\r\nContent-Type: text/html\r\n";
									ss << "Content-Length: " << response.size() << "\r\n\r\n";
									ss << response;

								Logger(tmpSockets[i].srv, Logger::DEBUG,  logs.str());
								send(i, ss.str().c_str(), ss.str().size(), 0);
							}
						}
					}
				}
			}
		} 
	}
}

ServerMonitor::ServerMonitorException::ServerMonitorException(std::string msg) : msg("Internal Server Monitor Exception:: ")
{
	this->msg += msg;
}

ServerMonitor::ServerMonitorException::~ServerMonitorException() throw() {}

const char *ServerMonitor::ServerMonitorException::what() const throw()
{
	return msg.c_str();
}