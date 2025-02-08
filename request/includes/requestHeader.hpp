#pragma once

#include "parsFuncs.hpp"

class requestHeader
{
    private :
        std::map<std::string, std::string> header;
    public :
        requestHeader();
        void setHeader(const std::string header);
        std::map<std::string, std::string> getHeader();
};