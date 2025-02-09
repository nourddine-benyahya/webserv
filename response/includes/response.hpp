#pragma once

#include "Server.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include "request.hpp"
#include "cgi.hpp"
// class Server;
// class Server::Config;
class Response
{
    public :
        Server::Config *srv;
        Response(request r, Server::Config *srv);
        std::string body;
        std::string header;
        std::string method;
        request req;
        void get();
        void post();
        void DELETE();
        void methodFilter();
        std::string response;
};