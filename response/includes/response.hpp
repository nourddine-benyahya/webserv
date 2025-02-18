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
        Response(request r, Server::Config *srv);
        void matchRoute();
        Route matchedRoute;
        bool foundRoute;
        Server::Config *srv;
        std::string reqResourcePath;
        void getResource();
        bool checkResource();
        bool checkCgiResource();
        void checkResourceType();
        void checkIndexed();
        bool checkUploadRoute();
        int deleteContent();
        void DeletecheckResource();
        bool checkRedir();
        std::string body;
        std::string header;
        std::string method;
        request req;
        void get();
        std::string response;
        void post();
        void Delete();

		std::string listDir(std::string);
};