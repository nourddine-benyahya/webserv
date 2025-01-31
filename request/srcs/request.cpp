#include "request.hpp"
#include <sstream>
#include <iostream>

void printRequestLine(requestLine reqLine)
{
    std::cout << std::endl << "-------------------------------------------request Line---------------------------------" << std::endl;
    std::cout << "Method: ~" << reqLine.getMethod() << "~" << std::endl;
    std::cout << "Request Target: ~" << reqLine.getReqTarget() << "~" << std::endl;
    std::cout << "HTTP Version: ~" << reqLine.getHttpVers() << "~"  << std::endl;
}

void printRequestHeader(requestHeader reqHeader)
{
    std::cout << std::endl << "-------------------------------------------request header-------------------------------" << std::endl;
    std::map<std::string, std::string> header = reqHeader.getHeader();
    std::cout << "key   " << "  value" << std::endl;
    for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); it++) {
        std::cout << "~" << it->first << "~ : ~" << it->second << "~" << std::endl;
    }
}

void printRequestBody(requestBody reqBody)
{
    std::cout << std::endl << "-------------------------------------------request body---------------------------------" << std::endl;
    std::cout  << std::endl << "----------body type----------- " << std::endl;
    if (reqBody.getType() == NONE)
        std::cout << "NONE" << std::endl;
    else if (reqBody.getType() == TEXT)
        std::cout << "TEXT" << std::endl;
    else if (reqBody.getType() == XML)
        std::cout << "XML" << std::endl;
    else if (reqBody.getType() == HTML)
        std::cout << "HTML" << std::endl;
    else if (reqBody.getType() == JSON)
        std::cout << "JSON" << std::endl;
    else if (reqBody.getType() == JAVA_SCRIPT)
        std::cout << "JAVA_SCRIPT" << std::endl;
    else if (reqBody.getType() == OCTET_STREAM)
        std::cout << "OCTET_STREAM" << std::endl;
    else if (reqBody.getType() == URLENCODED)
        std::cout << "URLENCODED" << std::endl;
    else if (reqBody.getType() == FORM_DATA)
        std::cout << "FORM_DATA" << std::endl;
    else if (reqBody.getType() == TEXT)
        std::cout << "TEXT" << std::endl;
    else
        std::cout << "Type: UNKNOWN" << std::endl;

    std::cout << std::endl << "--------------body formFields-------------" << std::endl;
    for (std::map<std::string, std::string>::iterator it = reqBody.formFields.begin(); it != reqBody.formFields.end(); it++) {
        std::cout << "~" << it->first << "~   :   ~" << it->second << "~" << std::endl;
    }
    std::cout << std::endl << "-------------body data----------" << std::endl;
    int i;
    std::cout  << "~";
    std::vector<char> datae = reqBody.getData();
    for (i = 0; i < datae.size(); i++)
    {
        std::cout << datae[i] ;
    }
    std::cout << "~" << std::endl;
}

void printFullRequest(const std::string request)
{
    std::cout << std::endl << "-------------------------------------------start request---------------------------------" << std::endl;
    std::cout << request << std::endl;
    std::cout << std::endl << "-------------------------------------------end request---------------------------------" << std::endl;
}

request::request()
{
    this->reqLine = requestLine("");
}

request::request(const std::string request) {
    std::string line;

    // Put the request in a stream and read it line by line
    std::istringstream requestStream(request);


    try {

        // Read the request line
        std::getline(requestStream, line);
        reqLine = requestLine(line);

        // Read the headers
        reqHeader = requestHeader();
        while (std::getline(requestStream, line) && !line.empty() && line != "\r") {
            trim(line);
            reqHeader.setHeader(line);
        }

        // Read the body
        reqBody = requestBody(requestStream, reqHeader);
        

        // //print full request
        // printFullRequest(request);

        // //print request Line
        // printRequestLine(reqLine);

        // //print request header
        // printRequestHeader(reqHeader);

        // //print request body
        // printRequestBody(reqBody);

    } catch(const char *e) {
        std::cerr << e << '\n';
    }
}
