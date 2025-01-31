#pragma once

#include "Server.hpp"

class Server;

//API builder of server usig those configurations
class Config : public Server::IConfig {

		public:

		// Starting
			Config();
			Config(Config&);
			~Config();

		// Characteristics
			IConfig& setPort(int port);


			IConfig* clone();
};