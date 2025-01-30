

#include "Server.hpp"

static void printAddressInfo(struct sockaddr_in& addr) {
	std::cout << "Address Info :\n"
    			<< " IP family: " << ((addr.sin_family == AF_INET)? "IPv4":"NONE") << std::endl;
    std::cout << " Port: " << ntohs(addr.sin_port) << std::endl;
    // std::cout << "Address: " << inet_ntoa(addr.sin_addr) << std::endl;
}


Server::Server(struct sockaddr_in& address): address(address) {
	// this->address = address;
	// printAdressInfo(this->address);
	initServer();
}

Server::~Server() {
	// close(server_fd);
}

void Server::readServerInfo(){
	std::cout << "Server " << std::endl;
	printAddressInfo(this->address);
}

int Server::getServerFd(){
	return this->server_fd;
}


void Server::initServer() {
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
	if (listen(server_fd, 3) < 0) {
		throw Server::ServerException("Listen error");
	}
}

// builder

Server Server::Builder::build() {
	return Server(address);
}

Server::Builder::Builder() {
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(80);
}


Server::Builder& Server::Builder::setPort(int port) {
	if (port <= 0 || port > 65535) {
		throw Server::ServerException("Port number must be between 1 and 65535");
	}
	address.sin_port = htons(port);
	return *this;
}


// Exception

Server::ServerException::ServerException(std::string msg) : msg("[Error::ServerException] : ") {
	this->msg += msg;
}

Server::ServerException::~ServerException() throw() {
}

const char* Server::ServerException::what() const throw() {
	return msg.c_str();
}