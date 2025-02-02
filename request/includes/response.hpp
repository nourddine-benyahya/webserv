#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include "request.hpp"


class Response
{
    public :
        Response(request r);
        std::string body;
        std::string header;
        std::string method;
        request req;
        void GET(std::string line, int clientSock);
        void POST(std::string line, int clientSocket);
        void DELETE();
        void methodFilter();
};