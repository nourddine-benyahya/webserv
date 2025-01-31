
#include "Server.hpp"
#include "ServerMonitor.hpp"


struct sockaddr_in& Server::Config::getAddress(){
	return this->address;
}

int Server::Config::getPort() {
	return ntohs(this->address.sin_port);
}

std::string& Server::Config::getName() {
	return this->name;
}

void Server::Config::build() {
	ServerMonitor *serverMonitor = ServerMonitor::getInstance();

	serverMonitor->addServer(new Server(this->clone()));
}

Server::Config::~Config() {}

Server::Config::Config() : name("0.0.0.0") {
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(80);
}


Server::Config::Config(Server::Config& other) {
	this->address = other.address;
	this->name = other.name;
}

Server::Config& Server::Config::setPort(int port) {
	if (port <= 0 || port > 65535) {
		throw Server::ServerException("Port number must be between 1 and 65535");
	}
	address.sin_port = htons(port);
	return *this;
}

Server::Config& Server::Config::setName(std::string name) {
	this->name = name;
	return *this;
}

Server::Config* Server::Config::clone() {
	return new Server::Config(*this);
}

