#pragma once

#include "parsFuncs.hpp"
#include <map>
class requestHeader
{
    private :
        std::map<std::string, std::string> header;
    public :
        //contructer
        requestHeader();
        //getter and setter
        void setHeader(const std::string header);
        std::map<std::string, std::string> &getHeader();
};