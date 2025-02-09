

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
}

int Server::getRecvBufferLenght(){
	return this->recvBuffer.str().size();
}

Server::Config* Server::getConfig(){
	return this->conf;
}

// Exception

Server::ServerException::ServerException(std::string msg) : msg("SE::") {
	this->msg += msg;
}

Server::ServerException::ServerException(std::string msg, int status) : msg("SE::"), status(status) {
	this->msg += msg;
}

Server::ServerException::ServerException(std::string msg, std::string error, int status) : msg(msg), error(error), status(status) {}

Server::ServerException::~ServerException() throw() {}

const char* Server::ServerException::what() const throw() {
	return msg.c_str();
}

std::string Server::ServerException::getError(){
	return error;
}

std::string Server::ServerException::createHTTPErrorHeader( int contentLength ) {
	std::stringstream ss;
			ss << "HTTP/1.1 "
				<< status << " KO"
				<< "\r\nContent-Length: "
				<< contentLength
				<< "\r\n\r\n";
	return ss.str();
}

int Server::ServerException::getStatus(){
	return status;
}