#pragma once
#include <sstream>
#include "requestLine.hpp"
#include "requestHeader.hpp"
#include "requestBody.hpp"


class request
{
    private :
        requestLine     reqLine;
        requestHeader   reqHeader;
        requestBody     reqBody;

    public :
        request();
        request(const std::string request);
};