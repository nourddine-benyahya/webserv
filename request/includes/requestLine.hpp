#pragma once

#include <string>
#include <iostream>

enum MethodType {
    GET,
    HEAD,
    OPTIONS,
    TRACE,
    PUT,
    DELETE,
    POST,
    PATCH,
    CONNECT,
};

class requestLine
{
    private :
        MethodType method;
        std::string reqTarget;
        std::string httpVers;
    public :
        requestLine(){method = GET; reqTarget = "/"; httpVers = "HTTP/1.1";}
        requestLine(const std::string requestLine);
        void setMethod(const std::string method);
        void setReqTarget(const std::string url);
        void setHttpVers(const std::string vers);
        std::string getMethod();
        std::string getReqTarget();
        std::string getHttpVers();
};

void trim(std::string& str);
