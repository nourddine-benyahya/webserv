#pragma once

#include "parsFuncs.hpp"

class requestHeader
{
    private :
        std::map<std::string, std::string> header;
    public :
        requestHeader();
        // requestHeader(const std::string header);
        void setHeader(const std::string header);
        std::map<std::string, std::string> getHeader();
};