#pragma once
#include <sstream>
#include "requestLine.hpp"
// #include "requestHeader.hpp"


class request
{
    private :
        requestLine reqLine;
        // requestHeader reqHeader;

    public :
        request();
        request(const std::string request);
};