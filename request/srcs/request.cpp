#include "request.hpp"
#include "exeptions.hpp"


request::request(const std::string request, Server::Config *server)
{
    this->requestString = std::string(request);
    srv = server;
    std::string line;

    // Put the request in a stream and read it line by line
    std::istringstream requestStream(request);


    try {

        std::getline(requestStream, line);
        try {
            // Read the request line
            reqLine = requestLine(line);
        } catch (exeptions ex)
        {
            throw Server::ServerException(ex.getMsg(), ex.getStatus());
        }

        // Read the headers
        while (std::getline(requestStream, line) && !line.empty() && line != "\r")
        {
            trim(line);
            reqHeader.setHeader(line);
        }

        // printRequestHeader();

        //check Request if Transfer-Encoding header exist and is different to “chunked”
        if (reqHeader.getHeader().find("Transfer-Encoding") != reqHeader.getHeader().end()
            && reqHeader.getHeader()["Transfer-Encoding"] != "chunked")
            throw Server::ServerException("501 Not Implemented", 501);

        // Transfer-Encoding not exist Content-Length not exist The method is Post
        if ((reqHeader.getHeader().find("Transfer-Encoding") != reqHeader.getHeader().end()
            && reqHeader.getHeader()["Transfer-Encoding"] == "chunked"
            && reqHeader.getHeader().find("Content-Length") == reqHeader.getHeader().end())
            && reqLine.getMethod() == POST)
            throw Server::ServerException("411 Length Required", 411);

        // check if Request body larger than client max body size in config file
        if (srv->body_limit != -1 && reqHeader.getHeader().find("Content-Length") != reqHeader.getHeader().end() 
            && std::stoi(reqHeader.getHeader()["Content-Length"]) > srv->body_limit)
            throw Server::ServerException("413 Payload Too Large", 413);

        // check unsupported media type
        if (reqHeader.getHeader().find("Content-Type") == reqHeader.getHeader().end() && reqHeader.getHeader().find("Content-Length") != reqHeader.getHeader().end())
            throw Server::ServerException("415 Unsupported Media Type", 415);

        // Read the body
        reqBody = requestBody(requestStream, reqHeader);

    } 
    catch (Server::ServerException &e) 
    {
        std::string body = this->srv->getErrorPage(e.getStatus());
        std::string response = e.createHTTPErrorHeader(body.length()) + body;
        throw Server::ServerException(e.what(), response, e.getStatus());
	}
}
