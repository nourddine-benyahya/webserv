

#include "Server.hpp"
#include "response.hpp"
// static void printServerInfo(Server srv) {
// 	std::cout << "Address Info :\n"
//     			<< " IP family: " << ((srv.getConfig()->getAddress().sin_family == AF_INET)? "IPv4":"NONE") << std::endl;
//     std::cout << " Port: " << ntohs(srv.getConfig()->getAddress().sin_port) << std::endl;
//     std::cout << " Logs: " << srv.getConfig()->getLogs() << std::endl;

//     // std::cout << "Address: " << inet_ntoa(addr.sin_addr) << std::endl;
// }
void Server::closeSocks(){
	std::map<int, int>::iterator it = this->getConfig()->getSockets().begin();
	std::map<int, int>::iterator end = this->getConfig()->getSockets().end();
	while (it != end){
		close(it->first);
		std::stringstream ss;
			ss << "WebSocket socket " << it->first << " closed with "
				<< this->getConfig()->getName() << ":" << it->second;
		Logger(Logger::WARNING, ss.str());
		it++;
	}
}

Server::Server(Config* conf): conf(conf) {
	// std::cout << "Server Cretead " << std::endl;
}

Server::~Server() {
	// shutdown all socks
	closeSocks();

	delete conf;
}

void Server::readServerInfo(){
	std::cout << "Server " << std::endl;
	std::cout
		<< " IP family: " << ((this->getConfig()->getAddress().sin_family == AF_INET)? "IPv4":"NONE") << std::endl;
    std::cout << " Port: " << ntohs(this->getConfig()->getAddress().sin_port) << std::endl;
    std::cout << " Logs: " << this->getConfig()->getLogs() << std::endl;

}

int Server::getServerFd(){
	return this->server_fd;
}

void Server::setRecvBuffer(std::string buffer){
	this->recvBuffer << buffer;
}

std::string Server::getRecvBuffer(){
	std::string res = this->recvBuffer.str();
	this->recvBuffer.str("");
		this->recvBuffer.clear();
	return res;
	// std::string res = recvBuffer;
	// recvBuffer = "";
	// return res;
}

int Server::getRecvBufferLenght(){
	return this->recvBuffer.str().size();
}

Server::Config* Server::getConfig(){
	return this->conf;
}


void Server::initServer() {
	// if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	// 	throw Server::ServerException("Socket creation error");
	// }
	// int opt = 1;
	// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
	// 	throw Server::ServerException("Setsockopt error");
	// }
	// if (bind(server_fd, (struct sockaddr *)&conf->getAddress(), sizeof(conf->getAddress())) < 0) {
	// 	throw Server::ServerException("Bind error");
	// }
	// whiling on all map keys of sock_port and start listening using them
	// if (listen(server_fd, 3) < 0) {
	// 	throw Server::ServerException("Listen error");
	// }
}

// Exception

Server::ServerException::ServerException(std::string msg) : msg("Internal Server Exception::") {
	this->msg += msg;
}

Server::ServerException::~ServerException() throw() {
}

const char* Server::ServerException::what() const throw() {
	return msg.c_str();
}