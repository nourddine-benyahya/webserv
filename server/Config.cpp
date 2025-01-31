
#include "Config.hpp"

Config::Config() {
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(80);
}

Config::Config(Config& other) {
	this->address = other.getAddress();
}

Config::~Config(){}

Server::IConfig& Config::setPort(int port) {
	if (port <= 0 || port > 65535) {
		throw Server::ServerException("Port number must be between 1 and 65535");
	}
	address.sin_port = htons(port);
	return *this;
}

Server::IConfig* Config::clone(){
	return new Config(*this);
}