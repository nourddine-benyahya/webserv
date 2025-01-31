

#include "Server.hpp"

// static void printAddressInfo(struct sockaddr_in& addr) {
// 	std::cout << "Address Info :\n"
//     			<< " IP family: " << ((addr.sin_family == AF_INET)? "IPv4":"NONE") << std::endl;
//     std::cout << " Port: " << ntohs(addr.sin_port) << std::endl;
//     // std::cout << "Address: " << inet_ntoa(addr.sin_addr) << std::endl;
// }


Server::Server(Config* conf): conf(conf) {
	// printAdressInfo(this->address);
	initServer();
}

Server::~Server() {
	delete conf;
}

void Server::readServerInfo(){
	std::cout << "Server " << std::endl;
	// this->conf.readInfo();
}

int Server::getServerFd(){
	return this->server_fd;
}

Server::Config* Server::getConfig(){
	return this->conf;
}


void Server::initServer() {
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throw Server::ServerException("Socket creation error");
	}
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		throw Server::ServerException("Setsockopt error");
	}
	if (bind(server_fd, (struct sockaddr *)&conf->getAddress(), sizeof(conf->getAddress())) < 0) {
		throw Server::ServerException("Bind error");
	}
	if (listen(server_fd, 3) < 0) {
		throw Server::ServerException("Listen error");
	}
}

// Exception

Server::ServerException::ServerException(std::string msg) : msg("[Error::ServerException]: ") {
	this->msg += msg;
}

Server::ServerException::~ServerException() throw() {
}

const char* Server::ServerException::what() const throw() {
	return msg.c_str();
}