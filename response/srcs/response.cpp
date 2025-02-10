#include "response.hpp"
#include <sys/stat.h>
#include <unistd.h>

void Response::matchRoute()
{
    std::cout << req.getReqLine().getReqTarget() << std::endl;
    foundRoute = false;
    std::map<std::string , Route>::iterator it = srv->routes.begin();
    // std::cout << it->first << std::endl;
    if (it ==  srv->routes.end())
        std::cout << "empty" << std::endl;
    while (it != srv->routes.end())
    {
        // std::cout << "here" << std::endl;
        if (it->first == req.getReqLine().getReqTarget())
        {
            std::cout << "found" << std::endl;
            matchedRoute = it->second;
            foundRoute = true;
        }
        it++;

    }
}
Response::Response(request r, Server::Config *server)
{

    req = r;
    srv = server;
    // std::cout << "response" << std::endl;

    matchRoute();
    // if (foundRoute == false)
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
void checkFile(std::string fileName)
{
    std::ifstream resource(fileName);
    if (!resource.is_open())
        throw Server::ServerException("file not found", 404);
    resource.close();
}
bool isDirectory(const std::string &path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0) {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}
void Response::checkResource()
{
    std::stringstream resourcePath;
    resourcePath << srv->getRoot() << ((matchedRoute.root.front() == '/') ? "" : "/") <<matchedRoute.root <<  ((matchedRoute.path.front() == '/') ? "" : "/") << matchedRoute.path;
    std::cout << "|" << resourcePath.str() << "|" << std::endl;
    checkFile(resourcePath.str());
    reqResourcePath = resourcePath.str();
    std::cout << "is dir :" << isDirectory(reqResourcePath) <<  std::endl;
    if (isDirectory(reqResourcePath))
    {
        if (matchedRoute.index.empty())
            throw Server::ServerException("forbidden", 403);
        else
        {
            reqResourcePath +=  matchedRoute.index;
            checkFile(reqResourcePath);
        }
    }
    std::cout << "final req :"<< reqResourcePath << std::endl;
}
bool Response::checkCgiResource()
{
    if (matchedRoute.cgis.size() == 0)
        return false;
    cgi c(req);
    c.runCgi();
    response = c.getResponse();
    return true;
}
std::string getContent(std::string fileName)
{
    std::ifstream resource(fileName);
    if (!resource.is_open())
        throw Server::ServerException("file not found", 404);
    std::stringstream content;
    content << resource.rdbuf();
    return content.str();
}
void Response::get()
{
	try {
		// have to check the permission of the request to throw ServerException with 403
        if (foundRoute == false)
        {
            body = srv->getFile(req.getReqLine().getReqTarget());
            header = "HTTP/1.1 200 OK\r\nContent-Length: ";
            std::stringstream lengthStr;
                lengthStr << body.length();
            response = header + lengthStr.str() + "\r\n\r\n" + body;
        }
        else if (foundRoute)
        {
            if (std::find(matchedRoute.allowedMethods.begin(), matchedRoute.allowedMethods.end(), "GET") == matchedRoute.allowedMethods.end())
                throw Server::ServerException("Method not allowed ", 405);
            checkResource();
            if (checkCgiResource())
                return;
            header = "HTTP/1.1 200 OK\r\nContent-Length: ";
            body = getContent(reqResourcePath);
            std::stringstream lengthStr;
                lengthStr << body.length();
            response = header + lengthStr.str() + "\r\n\r\n" + body;
        }
	} catch (Server::ServerException &e) {
		body = this->srv->getErrorPage(e.getStatus());
        response = e.createHTTPErrorHeader(body.length()) + body;
		throw Server::ServerException(e.what(), response, e.getStatus());
	}
}
void Response::post()
{
    std::cout << "inside post" << std::endl;
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

