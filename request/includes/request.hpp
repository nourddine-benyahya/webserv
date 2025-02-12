#pragma once

#include "Server.hpp"
#include "requestHeader.hpp"
#include "requestBody.hpp"
#include "requestLine.hpp"

class requestLine;

class request
{
    private :
        requestLine     reqLine;
        requestHeader   reqHeader;
        requestBody     reqBody;
        std::string    requestString;
        Server::Config *srv;

        //debuging functions
    public :
        void printFullRequest();
        void printRequestLine();
        void printRequestHeader();
        void printRequestBody();
        request(){};
        request(const std::string request, Server::Config *server);
        requestLine &getReqLine() { return reqLine; }
        requestHeader &getReqHeader() { return reqHeader; }
        requestBody &getReqBody() { return reqBody; }
};