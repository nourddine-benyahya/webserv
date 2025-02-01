#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>

class Response
{
    public :
        Response();
        std::string body;
        std::string header;
        std::string method;
        void GET(std::string line, int clientSock);
        void POST(std::string line, int clientSocket);
        void DELETE();
        void methodFilter();
};