#include "response.hpp"

Response::Response(request r, Server::Config *srv)
{
    req = r;
    this->srv = srv;
    if (req.getReqLine().getMethod() == GET)
    {
        get();
    }
    else if (req.getReqLine().getMethod() == POST)
    {
        post();
        std::cout << "post method" << std::endl;
    }
}

void Response::get()
{
    // std::cout << "HEER" << std::endl;
    std::ifstream file("." + req.getReqLine().getReqTarget());
    if (!file.is_open())
    {
        std::ifstream file404("./404.html");
        std::cout << req.getReqLine().getReqTarget() << std::endl;
        if (!file404.is_open())
        {
            std::cerr << "error opening 404.html" << std::endl;
            return;
        }
        std::cerr << "error with the file" << std::endl;
        std::ostringstream fileContent;
        fileContent << file404.rdbuf();
        body = fileContent.str();
        header = "HTTP/1.1 404 KO\r\nContent-Length: ";
        std::stringstream ss;
        ss << body.length();
        response = header + ss.str() + "\r\n\r\n" + body;
        // std::cout << body << std::endl;
        file404.close();
        return ;
    }
    std::string str;
    std::ostringstream fileContent;
    fileContent << file.rdbuf();
    body = fileContent.str();
    header = "HTTP/1.1 200 OK\r\nContent-Length: ";
    std::stringstream ss;
    ss << body.length();
    std::string lengthStr = ss.str();
    response = header + lengthStr + "\r\n\r\n" + body;
    file.close();
}
void Response::post()
{

    cgi c(req);

    c.runCgi();


    // response = "HTTP/1.1 200 OK\r\n";
    response = c.getResponse();
}

void Response::DELETE()
{

}
void Response::methodFilter()
{
    // if ()
}

