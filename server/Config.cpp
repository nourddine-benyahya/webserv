
#include "Server.hpp"

#include "ServerMonitor.hpp"


void Server::Config::create_sock(){
	int server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throw Server::ServerException("Socket creation error");
	}
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		throw Server::ServerException("Setsockopt error");
	}
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		throw Server::ServerException("Bind error");
	}
	if (listen(server_fd, 5) < 0) {
		throw Server::ServerException("Listen error");
	}
	std::cout << "createsock: " << server_fd;
	std::cout << " port: " << this->getPort() << std::endl;
	sock_port[server_fd] = this->getPort();
}

// Builder
void Server::Config::build() {

	if (!portRedifined){
		create_sock();
	}

	ServerMonitor *serverMonitor = ServerMonitor::getInstance();

	serverMonitor->addServer(new Server(this->clone()));
}

Server::Config::~Config() {}

Server::Config::Config() : name("0.0.0.0") {
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(80);
	sock_port.clear();
	this->fileIndex = "index.html";
}


Server::Config::Config(Server::Config& other) {
	this->address = other.address;
	this->name = other.name;
	this->fileIndex = other.fileIndex;
	this->sock_port = other.sock_port;
	this->errorPages = other.errorPages;
	this->rootFolder = other.rootFolder;
	this->logsFile = other.logsFile;
}

// Setters

Server::Config& Server::Config::setPort(int port) {
	if (port <= 0 || port > 65535) {
		throw Server::ServerException("Port number must be between 1 and 65535");
	}
	address.sin_port = htons(port);
	portRedifined = true;
	create_sock();
	return *this;
}

Server::Config& Server::Config::setName(std::string name) {
	this->name = name;
	return *this;
}

Server::Config& Server::Config::setIndex(std::string file){
	this->fileIndex = file;
	return *this;
}

Server::Config& Server::Config::setErrorPage(int status, std::string filePath){
	this->errorPages[status] = filePath;
	return *this;
}

Server::Config& Server::Config::setRoot(std::string folderPath){
	this->rootFolder = folderPath;
	return *this;
}

Server::Config& Server::Config::setLogs(std::string filePath){
	this->logsFile = filePath;
	return *this;
}

// getters

struct sockaddr_in& Server::Config::getAddress(){
	return this->address;
}

int Server::Config::getPort() {
	return ntohs(this->address.sin_port);
}

std::string& Server::Config::getName() {
	return this->name;
}

std::map<int, int>& Server::Config::getSockets(){
	return this->sock_port;
}

std::string Server::Config::getIndex() {
	std::ifstream file(this->fileIndex.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + this->fileIndex);
    }

	// tester function
    std::stringstream buffer;
   		buffer << file.rdbuf();
    return buffer.str();
}

std::string Server::Config::getRoot(){
	return this->rootFolder;
}

std::string Server::Config::getLogs(){
	return this->logsFile;
}

std::map<int, std::string> Server::Config::getErrorPages(){
	return this->errorPages;
}




Server::Config* Server::Config::clone() {
	return new Server::Config(*this);
}

