#include "Logger.hpp"


Logger::Logger( LOGS log, std::string msg) {
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);

	char buffer[20];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S" , localTime);

	std::cerr << buffer
		   << " [" << logToString(log) << "] " 
		   << msg 
		   << std::endl;
}
// Logger::Logger(std::ostream& stream, LOGS log, std::string msg) {
// 	std::time_t now = std::time(NULL);
// 	std::tm* localTime = std::localtime(&now);

// 	char buffer[20];
// 	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S" , localTime);

// 	stream << buffer
// 		   << " [" << logToString(log) << "] " 
// 		   << msg 
// 		   << std::endl;
// }

std::string Logger::logToString(LOGS log) const{
	switch (log) {
        case INFO: return BGRN "INFO" RESET;
        case DEBUG: return BBLU "DEBUG" RESET;
        case WARNING: return BYEL "WARNING" RESET;
        case ERROR: return BRED "ERROR" RESET;
        case NOTICE: return BMAG "NOTICE" RESET;
    }
}

Logger::~Logger() {}