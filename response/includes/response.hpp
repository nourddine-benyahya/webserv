#pragma once

#include "Server.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include "request.hpp"
#include "cgi.hpp"
class Server;
// class Server::Config;
class Response
{
    public :
        void matchRoute();
        Route matchedRoute;
        bool foundRoute;
        Server::Config *srv;
        std::string reqResourcePath;
        Response(request r, Server::Config *srv);
        void getResource();
        void checkResource();
        bool checkCgiResource();
        void checkResourceType();
        void checkIndexed();
        bool checkUploadRoute();
        std::string body;
        std::string header;
        std::string method;
        request req;
        void get();
        void post();
        void Delete();
        std::string response;

		std::string listDir(std::string);
};