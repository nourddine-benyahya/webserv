#pragma once

#include "parsFuncs.hpp"

class requestLine
{
    private :
        MethodType method;
        std::string reqTarget;
        std::string httpVers;
    public :
        requestLine(){};
        requestLine(const std::string requestLine);
        void setMethod(const std::string method);
        void setReqTarget(const std::string url);
        void setHttpVers(const std::string vers);
        std::string getMethod();
        std::string getReqTarget();
        std::string getHttpVers();
};
