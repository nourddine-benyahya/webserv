#include "Logger.hpp"



bool Logger::writeOnFile(std::string fileName, std::string msg){
	std::ofstream file;
	file.open(fileName.c_str(), std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("Invalid file: " + fileName);
    }
    file << msg;
    return true;
}



Logger::Logger( Server *server, LOGS log, std::string msg) {
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);

	char buffer[20];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S" , localTime);

	std::stringstream stream;
	stream << buffer
		   << " [" << logToString(log) << "] " 
		   << msg 
		   << std::endl;

	std::string fileName = server->getConfig()->getLogs();
	if (fileName.empty())
		std::cout << stream.str();
	else{
		try {
			writeOnFile(fileName, stream.str());
		} catch (std::exception& e) {
			std::cout << stream.str();
			server->getConfig()->setLogs("");
			Logger(server, Logger::ERROR, e.what());
		}
	}
}

Logger::Logger( LOGS log, std::string msg) {
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);

	char buffer[20];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S" , localTime);

	std::stringstream stream;
	stream << buffer
		   << " [" << logToString(log) << "] " 
		   << msg 
		   << std::endl;

		std::cout << stream.str();
}

std::string Logger::logToString(LOGS log) const{
	switch (log) {
        case INFO: return BGRN "INFO" RESET;
        case DEBUG: return BBLU "DEBUG" RESET;
        case WARNING: return BYEL "WARNING" RESET;
        case ERROR: return BRED "ERROR" RESET;
        case NOTICE: return BMAG "NOTICE" RESET;
    }
	return "NONE";
}

Logger::~Logger() {}