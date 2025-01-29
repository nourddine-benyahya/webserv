#pragma once

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
        requestLine(const std::string requestLine);
        void setMethod(const std::string method);
        void setReqTarget(const std::string url);
        void setHttpVers(const std::string vers);
};

// class request
// {
// }