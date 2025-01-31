#pragma once

#include "requestLine.hpp"
#include "requestHeader.hpp"
#include "requestBody.hpp"

class request
{
    private :
        requestLine     reqLine;
        requestHeader   reqHeader;
        requestBody     reqBody;
        //debuging functions
        void printFullRequest(const std::string request);
        void printRequestLine();
        void printRequestHeader();
        void printRequestBody();
    public :
        request(){};
        request(const std::string request);
        requestLine getReqLine() const { return reqLine; }
        requestHeader getReqHeader() const { return reqHeader; }
        requestBody getReqBody() const { return reqBody; }
};