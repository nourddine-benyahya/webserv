#include "ServerMonitor.hpp"

ServerMonitor *ServerMonitor::instance = NULL;

static bool rootExist(std::string file)
{
	struct stat info;
	if (stat(file.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR))
	{
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
	if (indexFile.is_open())
	{
		indexFile << "<!DOCTYPE html>\n"
				  << "<html lang=\"en\">\n"
				  << "<head>\n"
				  << "    <meta charset=\"UTF-8\">\n"
				  << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
				  << "    <title>Welcome Page</title>\n"
				  << "    <style>\n"
				  << "        body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }\n"
				  << "        h1 { font-size: 50px; font-weight: bold; }\n"
				  << "        h2 { font-size: 30px; font-weight: bold; }\n"
				  << "        .container { display: flex; justify-content: center; flex-wrap: wrap; gap: 20px; }\n"
				  << "        .card { background: #f9f9f9; border-radius: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); overflow: hidden; width: 300px; text-align: center; }\n"
				  << "        .card img { width: 100%; height: auto; }\n"
				  << "        .card p { font-size: 24px; font-weight: bold; margin: 10px 0; }\n"
				  << "    </style>\n"
				  << "</head>\n"
				  << "<body>\n"
				  << "    <h1>Welcome to the WebServ</h1>\n"
				  << "    <h2>Dedicated to Ahssan Chabab:</h2>\n"
				  << "    <div class=\"container\">\n"
				  << "        <div class=\"card\">\n"
				  << "            <img src=\"../assets/abounab.jpg\" alt=\"Abdellah Bounab\">\n"
				  << "            <p>Abdellah Bounab</p>\n"
				  << "        </div>\n"
				  << "        <div class=\"card\">\n"
				  << "            <img src=\"../assets/amejdoub.jpg\" alt=\"Anas Mejdoub\">\n"
				  << "            <p>Anas Mejdoub</p>\n"
				  << "        </div>\n"
				  << "        <div class=\"card\">\n"
				  << "            <img src=\"../assets/nbenyahy.jpg\" alt=\"Nourddine BenYahya\">\n"
				  << "            <p>Nourddine BenYahya</p>\n"
				  << "        </div>\n"
				  << "    </div>\n"
				  << "</body>\n"
				  << "</html>";
		indexFile.close();
	}
}

ServerMonitor::~ServerMonitor()
{
	// Cleanups done here (delete all servers)
	std::set<Server *> srvs;
	for (std::map<int, Server *>::iterator it = sockets.begin(); it != sockets.end(); it++)
	{
		srvs.insert(sockets[it->first]);
	}
	for (std::set<Server *>::iterator it = srvs.begin(); it != srvs.end(); ++it)
	{
		delete *it;
	}
	Logger(Logger::NOTICE, "WebServ Closing.");
}

ServerMonitor *ServerMonitor::getInstance()
{
	if (instance == NULL)
		instance = new ServerMonitor();
	return instance;
}

void ServerMonitor::addServer(Server *server)
{
	if (!server)
		throw ServerMonitorException("Null Server");
	if (!rootExist(server->getConfig()->getRoot()))
	{
		std::stringstream ss;
		ss << "Folder doesnt exists :" << server->getConfig()->getRoot()
		   << " - " << server->getConfig()->getName();
		Logger(Logger::ERROR, ss.str());
		return (delete server);
	}
	std::map<int, int>::iterator it = server->getConfig()->getSockets().begin();
	while (it != server->getConfig()->getSockets().end())
	{

		FD_SET(it->first, &master_set);
		if (it->first > maxFds)
			maxFds = it->first;
		std::stringstream ss;
		ss << "Connection created at "
		   << server->getConfig()->getName()
		   << ":" << it->second;
		Logger(Logger::INFO, ss.str());
		sockets[it->first] = server;
		it++;
	}
}

void ServerMonitor::update_maxFds()
{
	maxFds = -1;
	for (int j = 0; j <= FD_SETSIZE; ++j)
	{
		if (FD_ISSET(j, &master_set) && j > maxFds)
		{
			maxFds = j;
		}
	}
}

void ServerMonitor::acceptNewConnections(int i, std::map<int, ServerAndPort> &tmpSockets)
{
	struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);

	int new_socket = accept(i, (struct sockaddr *)&client_address, &client_address_len);
	if (new_socket < 0)
		throw ServerMonitorException("Accept Error");

	FD_SET(new_socket, &master_set);
	if (new_socket > maxFds)
		maxFds = new_socket;

	ServerAndPort tmp;
	tmp.port = sockets[i]->getConfig()->getSockets().find(i)->second;
	tmp.srv = sockets[i];
	tmp.contentLength = -1;
	tmpSockets[new_socket] = tmp;
	{
		std::stringstream ss;
		ss << "new WebSocket connection established with "
		   << (tmpSockets[new_socket].srv)->getConfig()->getName()
		   << ":" << tmpSockets[new_socket].port;
		Logger(tmpSockets[new_socket].srv, Logger::INFO, ss.str());
	}
}

void ServerMonitor::fillRecvBuffer(ServerAndPort& sp, std::string buffer) {
	sp.srv->setRecvBuffer(buffer);
	if (sp.contentLength == -1)
		sp.contentLength = getContentLenght(buffer);
	sp.isReady = (sp.contentLength <= sp.srv->getRecvBufferLenght()) ? true : false;

	std::stringstream ss;
	ss << "WebSocket message received from "
		<< (sp.srv)->getConfig()->getName() + ":"
		<< sp.port;
	Logger(sp.srv, Logger::DEBUG, ss.str());
}

int ServerMonitor::returnRecvBuffer(int sock, std::string &buffer){
	char buff[BUFFER_SIZE] = {0};
	int bytes_read;
	int k = 0;
	while (k < 5 && (bytes_read = recv(sock, buff, sizeof(buff), 0)) > 0)
	{
		buffer.append(buff, bytes_read);
		if (bytes_read < BUFFER_SIZE)
			break;
		k++;
	}
	return bytes_read;
}

void ServerMonitor::handleError(int sock, int bytes_read, std::map<int, ServerAndPort> &tmpSockets) {	
	if (bytes_read == 0)
	{
		std::stringstream ss;
		ss << "Connection closed at " << tmpSockets[sock].port;
		Logger(tmpSockets[sock].srv, Logger::WARNING, ss.str());
	}
	else
		Logger(tmpSockets[sock].srv, Logger::ERROR, "Recv Error");
	tmpSockets.erase(sock);
	FD_CLR(sock, &master_set);
	update_maxFds();
	close(sock);
}


int ServerMonitor::getContentLenght(std::string header)
{
	size_t content_length = 0;
	size_t pos = header.find("\r\n\r\n");

	if (pos != std::string::npos)
	{
		std::string headers = header.substr(0, pos);
		header.erase(0, pos + 4);

		std::istringstream header_stream(headers);
		std::string line;
		while (std::getline(header_stream, line))
		{
			if (line.find("Content-Length:") != std::string::npos)
			{
				std::string content_length_str = line.substr(line.find(":") + 1);
				std::stringstream content_length_stream(content_length_str);
				content_length_stream >> content_length;
				break;
			}
		}
	}
	return content_length;
}

void ServerMonitor::run()
{
	fd_set read_set, write_set;
	std::map<int, ServerAndPort> tmpSockets;

	Logger(Logger::NOTICE, "WebServ starting...");
	while (maxFds > 0)
	{
		read_set = write_set = master_set;
		if (select(maxFds + 1, &read_set, &write_set, NULL, NULL) < 0)
			throw ServerMonitorException("Select error");
		for (int i = 3; i <= maxFds; ++i)
		{
			if (FD_ISSET(i, &read_set))
			{
				if (sockets.find(i) != sockets.end())
				{
					try {
						acceptNewConnections(i, tmpSockets);
					}
					catch (std::exception &e) {
						Logger(sockets[i], Logger::ERROR, e.what());
					}
				}
				else
				{
					std::string buffer;
					int	bytes_read = returnRecvBuffer(i, buffer) ;

					if (bytes_read > 0)
						fillRecvBuffer(tmpSockets[i], buffer);
					if (bytes_read <= 0)
						handleError(i, bytes_read, tmpSockets);
				}
			}
			if (FD_ISSET(i, &write_set) && tmpSockets[i].isReady)
			{
				std::string msgTwil = tmpSockets[i].srv->getRecvBuffer();

				//this is where request would be used (using the getRecvBuffer() to get the request)
				request req(msgTwil);
				Response res(req);
				// Request(msgTwil, tmpSocket[i].srv->getConfig()); : adding Config in case he needed it or for CGI
				//all what remain would be used as reponse
				// Response(Request.getElements, tmpSocket[i].srv->getConfig());


				// std::string response;
				// std::string valid = " OK";
				// int status = 200;
				// try
				// {
				// 	response = tmpSockets[i].srv->getConfig()->getIndex();
				// }
				// catch (std::exception &e)
				// {
				// 	status = 404;
				// 	response = tmpSockets[i].srv->getConfig()->getErrorPage(status);
				// 	valid = " KO";
				// 	Logger(tmpSockets[i].srv, Logger::WARNING, e.what());
				// }

				// std::stringstream ss;
				// ss << "HTTP/1.1 " << status << valid;
				// ss << "\r\nContent-Type: text/html\r\n";
				// ss << "Content-Length: " << response.size() << "\r\n\r\n";
				// ss << response;
				// send(i, ss.str().c_str(), ss.str().size(), 0);
				// std::cout << res.response << std::endl;
				send(i, res.response.c_str(), res.response.size(), 0);


				std::stringstream logs;
				logs << "Sender connection from socket " << i ;
					//  << i << " with status " << status << valid;
				Logger(tmpSockets[i].srv, Logger::DEBUG, logs.str());
				FD_CLR(i, &master_set);
				close(i);
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