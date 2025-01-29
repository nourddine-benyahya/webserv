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
    while (std::getline(requestStream, line) && !line.empty()) {
        requestHeaderStr += line + "\n";
        std::cout << "----" << line << "----" << std::endl;
    }

    std::cout << "+++++++++++++" << requestHeaderStr << "+++++++++++++" << std::endl;

    try {
        reqLine = requestLine(requestLineStr);
        std::cout << "------------------------------request Line-----------------------" << std::endl;
        std::cout << "Method: " << reqLine.getMethod() << std::endl;
        std::cout << "Request Target: " << reqLine.getReqTarget() << std::endl;
        std::cout << "HTTP Version: " << reqLine.getHttpVers() << std::endl;
        std::cout << "------------------------------request header-----------------------" << std::endl;
    } catch(const char *e) {
        std::cerr << e << '\n';
    }
}