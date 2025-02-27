#include "request.hpp"
#include "exeptions.hpp"


request::request(const std::string request, Server::Config *server)
{
    // std::cout << "SERVER SALA KHDEMTOOOOOOOO" << std::endl;
    // std::cout << "REQUEST : " << request << std::endl;
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
            std::cout << "err in requist line " << std::endl;
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
            && std::atoi(reqHeader.getHeader()["Content-Length"].c_str()) > srv->body_limit)
            throw Server::ServerException("413 Payload Too Large", 413);

        // check unsupported media type
        if (reqHeader.getHeader().find("Content-Type") == reqHeader.getHeader().end() && reqHeader.getHeader().find("Content-Length") != reqHeader.getHeader().end() && std::atol(reqHeader.getHeader()["Content-Length"].c_str()) > 0)
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


    //print requist line
    // std::cout << "---------------------request line : ------------------" << std::endl;
    // std::cout << "Method : " << reqLine.getMethod() << std::endl;
    // std::cout << "Full URI : " << reqLine.getReqFullTarget() << std::endl;
    // std::cout << "URI : " << reqLine.getReqTarget() << std::endl;
    // std::cout << "HTTP Version : " << reqLine.getHttpVers() << std::endl;
    // std::cout << "params : "  << std::endl;
    // for (std::map<std::string, std::string>::iterator it = reqLine.getParams().begin(); it != reqLine.getParams().end(); ++it)
    //     std::cout << it->first << " : " << it->second << std::endl;
    // std::cout << "---------------------end request line : ------------------" << std::endl;


    // std::cout << "--------------------- header : ------------------" << std::endl;
    // for (std::map<std::string, std::string>::iterator it = reqHeader.getHeader().begin(); it != reqHeader.getHeader().end(); ++it)
    //     std::cout << it->first << " : " << it->second << std::endl;
    // std::cout << "---------------------end  header : ------------------" << std::endl;


    // std::cout << "--------------------- body : ------------------" << std::endl;
    // std::cout << "type : " << reqBody.getType() << std::endl;
    // std::cout << "all body" << std::endl;
    // for (std::vector<formData>::iterator it = reqBody.getBody().begin() ; it != reqBody.getBody().end(); it++)
    // {
    //     std::cout << "type : " << it->type << std::endl;
    //     if (it->filePath.size() > 0)
    //         std::cout << "filePath : " << it->filePath << std::endl;
    //     if (it->fileBuffer.size() > 0)
    //         std::cout << "fileBuffer : " << it->fileBuffer.size() << std::endl;
    //     std::cout << "formFields : " << std::endl;
    //     for (std::map<std::string, std::string>::iterator it2 = it->formFields.begin(); it2 != it->formFields.end(); ++it2)
    //         std::cout << it2->first << " : " << it2->second << std::endl;
    // }
    // std::cout << "---------------------end  body : ------------------" << std::endl;

}
