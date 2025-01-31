
#include "Server.hpp"


struct sockaddr_in& Server::IConfig::getAddress(){
	return this->address;
}

Server Server::IConfig::build() {
	return Server(this->clone());
}

Server::IConfig::~IConfig() {}