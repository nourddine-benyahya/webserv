#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include "../server/Server.hpp"



// Regular bold text
# define BRED "\033[1;31m"
# define BGRN "\033[1;32m"
# define BYEL "\033[1;33m"
# define BBLU "\033[1;34m"
# define BMAG "\033[1;35m"
# define BCYN "\033[1;36m"

// Regular underline text
# define UWHT "\033[4;37m"

// Reset
# define RESET "\033[0m"

class Server;


class Logger {
	public:
	enum LOGS {
		INFO,
		DEBUG,
		WARNING,
		ERROR,
		NOTICE
	};

		Logger( Server* , LOGS, std::string );
		Logger(LOGS, std::string );
		~Logger( );
	private:
		std::string logToString(LOGS log) const;
		bool writeOnFile(std::string, std::string);
};
