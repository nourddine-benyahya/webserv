#include "request.hpp"
#include <sstream>
#include <iostream>

request::request()
{
    this->reqLine = requestLine("");
}

request::request(const std::string request) {
    std::string requestLineStr;
    std::string requestHeaderStr = "";
    std::string line;

    // Put the request in a stream and read it line by line
    std::istringstream requestStream(request);

    // Read the request line
    std::getline(requestStream, requestLineStr);

    // Read the headers
    reqHeader = requestHeader();
    while (std::getline(requestStream, line) && !line.empty() && line != "\r") {
        trim(line);
        reqHeader.setHeader(line);
    }


    // Read the body
    reqBody = requestBody(requestStream, reqHeader);

    try {
        reqLine = requestLine(requestLineStr);
        std::cout << "------------------------------request Line-----------------------" << std::endl;
        std::cout << "Method: *" << reqLine.getMethod() << "*" << std::endl;
        std::cout << "Request Target: *" << reqLine.getReqTarget() << "*" << std::endl;
        std::cout << "HTTP Version: *" << reqLine.getHttpVers() << "*"  << std::endl;
        std::cout << "------------------------------request header-----------------------" << std::endl;
        std::map<std::string, std::string> header = reqHeader.getHeader();
        std::cout << "key " << "value" << std::endl;
        for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); it++) {
            std::cout << "*" << it->first << "*  *" << it->second << "*" << std::endl;
        }
        std::vector<char> datae = reqBody.getData();
        std::cout << "------------------------------request body-----------------------" << std::endl;
        std::cout << "type : *" << reqBody.getType() << "*" << std::endl;
        int i;
        std::cout  << "*";
        for (i = 0; i < datae.size(); i++)
        {
            std::cout << datae[i] ;
        }
        std::cout << "*" << std::endl;
        // std::cout << "actual size : " << header["Content-Length"]  << " vector size : " << datae.size() << " reading bytes : " << i << std::endl;
    } catch(const char *e) {
        std::cerr << e << '\n';
    }
}