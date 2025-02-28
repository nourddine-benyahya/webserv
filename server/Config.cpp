#include "Server.hpp"
#include "ServerMonitor.hpp"

std::string Server::Config::catRoot(std::string file)
{

	std::stringstream ss;
	ss << this->getRoot() << ((file[0] != '/') ? "/" : "")
	   << file;
	return ss.str();
}

std::string Server::Config::readFile(std::string fileName)
{
	struct stat fileStat;
	if (stat(fileName.c_str(), &fileStat) != 0)
	{
		throw Server::ServerException("No such file or directory: " + fileName, 404);
	}
	if (!(fileStat.st_mode & S_IRUSR))
	{
		throw Server::ServerException("Permission denied: " + fileName, 403);
	}
	std::ifstream file(fileName.c_str());
	if (!file.is_open())
	{
		throw Server::ServerException("Unable to open file: " + fileName, 500);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

void Server::Config::create_sock()
{
	int server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		throw Server::ServerException("Socket creation error");
	}
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(server_fd);
		throw Server::ServerException("Setsockopt error");
	}

	int flags = fcntl(server_fd, F_GETFL, 0);
	if (flags == -1)
	{
		close(server_fd);
		throw Server::ServerException("fcntl F_GETFL error");
	}
	if (fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		close(server_fd);
		throw Server::ServerException("fcntl F_SETFL error");
	}

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		close(server_fd);
		throw Server::ServerException("Bind: Address already in use");
	}
	if (listen(server_fd, 250) < 0)
	{
		close(server_fd);
		throw Server::ServerException("Listen error");
	}

	sock_port[server_fd] = this->getPort();
}

// Builder
void Server::Config::build()
{
	// if no port defined before
	if (!portRedifined)
		create_sock();

	ServerMonitor *serverMonitor = ServerMonitor::getInstance();

	serverMonitor->addServer(new Server(this->clone()));
}

Server::Config::~Config() {}

Server::Config::Config() : name("0.0.0.0")
{
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(80);
	sock_port.clear();
	this->fileIndex = "";
	this->rootFolder = ".";
	this->logsFile = "";
	this->body_limit = -1;
}

Server::Config::Config(Server::Config &other)
{
	this->address = other.address;
	this->name = other.name;
	this->fileIndex = other.fileIndex;
	this->sock_port = other.sock_port;
	this->errorPages = other.errorPages;
	this->rootFolder = other.rootFolder;
	this->logsFile = other.logsFile;
	this->routes = other.routes;
	this->body_limit = other.body_limit;
}

// Setters

void Server::Config::updatePort(int port)
{
	address.sin_port = htons(port);
}

Server::Config &Server::Config::setPort(int port)
{
	if (port <= 0 || port > 65535)
	{
		throw Server::ServerException("Port number must be between 1 and 65535");
	}
	address.sin_port = htons(port);
	portRedifined = true;
	create_sock();
	return *this;
}

Server::Config &Server::Config::setName(std::string name)
{
	if (name.empty() || (this->name != "0.0.0.0" && this->name != "localhost"))
		throw Server::ServerException("Host name failed:" + name);

	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(name.c_str(), NULL, &hints, &res);
	if (status != 0)
	{
		freeaddrinfo(res);
		this->name = "localhost";
		return *this;
	}
	this->name = name;

	this->address.sin_addr = ((struct sockaddr_in *)res->ai_addr)->sin_addr;
	freeaddrinfo(res);
	return *this;
}

Server::Config &Server::Config::setIndex(std::string file)
{
	this->fileIndex = file;
	return *this;
}

Server::Config &Server::Config::setErrorPage(int status, std::string filePath)
{
	this->errorPages[status] = filePath;
	return *this;
}

static std::string getErrorDescription(int status)
{
	switch (status)
	{
	case 400:
		return "Bad Request";
	case 401:
		return "Unauthorized";
	case 403:
		return "Forbidden";
	case 404:
		return "Page Not Found";
	case 405:
		return "Method Not Allowed";
	case 409:
		return "Conflict";
	case 411:
		return "Length Required";
	case 413:
		return "Payload Too Large";
	case 414:
		return "Request-URI Too Long";
	case 415:
		return "Unsupported Media Type";
	case 500:
		return "Internal Server Error";
	case 501:
		return "Not Implemented";
	case 502:
		return "Bad Gateway";
	case 503:
		return "Service Unavailable";
	case 504:
		return "Gateway Timeout";
	case 505:
		return "HTTP Version Not Supported";
	default:
		return "Unknown Error";
	}
}

std::string Server::Config::getFile(std::string fileName)
{
	if (!fileName.compare("/"))
		return getIndex();
	return readFile(catRoot(fileName));
}

std::string Server::Config::getErrorPage(int status)
{
	if (errorPages.find(status) != errorPages.end())
	{
		try
		{
			return readFile(catRoot(errorPages[status]));
		}
		catch (Server::ServerException &e)
		{
			if ((status = e.getStatus()) != 404)
				throw e;
		}
	}
	// create a template error page

	std::ostringstream oss;
	oss << "<!DOCTYPE html>\n"
		<< "<html lang=\"en\">\n"
		<< "<head>\n"
		<< "    <meta charset=\"UTF-8\">\n"
		<< "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		<< "    <title>Error " << status << "</title>\n"
		<< "</head>\n"
		<< "    <style>\n"
		<< "        body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }\n"
		<< "        h1 { font-size: 50px; font-weight: bold; }\n"
		<< "        p { font-size: 20px; font-weight: bold; }\n"
		<< "    </style>\n"
		<< "<body>\n"
		<< "    <h1>Error " << status << "</h1>\n"
		<< "    <p>" << getErrorDescription(status) << "</p>\n"
		<< "</body>\n"
		<< "</html>";
	return oss.str();
}

Server::Config &Server::Config::setRoot(std::string folderPath)
{
	this->rootFolder = folderPath;
	return *this;
}

Server::Config &Server::Config::setLogs(std::string filePath)
{
	this->logsFile = filePath;
	return *this;
}

// getters

struct sockaddr_in &Server::Config::getAddress()
{
	return this->address;
}

int Server::Config::getPort()
{
	return ntohs(this->address.sin_port);
}

std::string &Server::Config::getName()
{
	return this->name;
}

std::map<int, int> &Server::Config::getSockets()
{
	return this->sock_port;
}

std::string Server::Config::getIndex()
{
	return readFile(catRoot(this->fileIndex));
}

std::string Server::Config::getRoot()
{
	return this->rootFolder;
}

std::string Server::Config::getLogs()
{
	if (!this->logsFile.empty())
		return catRoot(this->logsFile);
	return this->logsFile;
}

std::map<int, std::string> Server::Config::getErrorPages()
{
	return this->errorPages;
}

Server::Config *Server::Config::clone()
{
	return new Server::Config(*this);
}
