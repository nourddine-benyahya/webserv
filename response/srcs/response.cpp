#include "response.hpp"
#include <sys/stat.h>
#include <unistd.h>

template <typename T>
static std::vector<std::string>::iterator find(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, T value)
{
    while (begin != end)
    {
        if (*begin == value)
            return begin;
        begin++;
    }
    return end;
}

void Response::matchRoute()
{
    foundRoute = false;
    std::map<std::string , Route>::iterator it = srv->routes.begin();
    if (it ==  srv->routes.end())
        std::cout << "empty" << std::endl;
    it = srv->routes.find(req.getReqLine().getReqTarget());
    if (it != srv->routes.end())
    {

        matchedRoute = it->second;
        foundRoute = true;
    }
    else if (srv->routes.find("/") != srv->routes.end())
    {
        // std::cout << "root route found" << std::endl;
        matchedRoute = srv->routes.find("/")->second;
        foundRoute = true;
    }
    std::cout << "MATCHED ROUTE " << matchedRoute.path << std::endl;
}
Response::Response(request r, Server::Config *server)
{
    req = r;
    srv = server;
    std::cout << "SERVER ROOT: " << srv->getRoot() << " " << srv->getPort() << std::endl;
    matchRoute();
    try
    {
        if (req.getReqLine().getMethod() == GET)
        {
            std::cout << "GET " << foundRoute << std::endl;

            get();
        }
        else if (req.getReqLine().getMethod() == POST)
        {
            // std::cout << "POST AND NOT DELETE" << std::endl;
            post();
        }
        else if (req.getReqLine().getMethod() == DELETE)
        {
            // std::cout << "DELETE REEEEEEEEEEEEEEEEE" << std::endl;
            Delete();
        }
    }
    catch (Server::ServerException &e) {
		body = this->srv->getErrorPage(e.getStatus());
        response = e.createHTTPErrorHeader(body.length()) + body;
		throw Server::ServerException(e.what(), response, e.getStatus());
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

void checkSlash(std::stringstream &resourcePath, std::string root, std::string &routeRoot, std::string &path)
{
    resourcePath << root;
    std::cout << "|" << root << "|" << std::endl;
    if (!routeRoot.empty() && routeRoot.front() != '/' && resourcePath.str().back() != '/')
        resourcePath << "/";
    resourcePath << routeRoot;
    if (!path.empty() && path.front() != '/' && resourcePath.str().back() != '/')
        resourcePath << "/";

    resourcePath << path;
    // std::cout << "in Slash " << resourcePath.str() << std::endl;
}

void Response::checkResource()
{
    std::stringstream resourcePath;
    checkSlash(resourcePath, srv->getRoot(), matchedRoute.root, req.getReqLine().getReqTarget());
    reqResourcePath = resourcePath.str();
    if (matchedRoute.path == "/" && isDirectory(reqResourcePath))
    {
        if (matchedRoute.index.empty())
            reqResourcePath +=  srv->fileIndex;
        else
            reqResourcePath +=  matchedRoute.index;
        checkFile(reqResourcePath);
    }
    else if (isDirectory(reqResourcePath))
    {
        if (matchedRoute.index.empty())
        {
            throw Server::ServerException("forbidden", 403);
        }
        else
        {
            reqResourcePath +=  matchedRoute.index;
            checkFile(reqResourcePath);
        }
    }
    else if (!isDirectory(reqResourcePath))
        checkFile(reqResourcePath);
}
void Response::DeletecheckResource()
{
    std::stringstream resourcePath;
    checkSlash(resourcePath, srv->getRoot(), matchedRoute.root, req.getReqLine().getReqTarget());
    reqResourcePath = resourcePath.str();
    if (isDirectory(reqResourcePath))
    {
        if (!matchedRoute.index.empty())
        {
            reqResourcePath +=  matchedRoute.index;
            checkFile(reqResourcePath);
        }
    }
    else
        checkFile(reqResourcePath);

}
bool Response::checkCgiResource()
{
    if (matchedRoute.cgis.size() == 0)
        return false;
    cgi c(req, reqResourcePath);
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
        std::cout << "REQUEST " << req.getReqLine().getReqTarget() << " MATCHED ROUTE " << matchedRoute.path << std::endl;
        if (find(matchedRoute.allowedMethods.begin(), matchedRoute.allowedMethods.end(), "GET") == matchedRoute.allowedMethods.end())
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
}

void Response::checkIndexed()
{
    std::stringstream resourcePath;
    resourcePath << this->srv->getRoot(); 
    if (req.getReqLine().getReqTarget().front() != '/' && resourcePath.str().back() != '/')
        resourcePath << "/";
    reqResourcePath = resourcePath.str();
    if (isDirectory(reqResourcePath))
    {
        if (this->srv->getIndex().empty())
            throw Server::ServerException("forbidden", 403);
        else
        {
            reqResourcePath +=  this->srv->getIndex();
            checkFile(reqResourcePath);
        }
    }
    else
        checkFile(reqResourcePath);
}
bool Response::checkUploadRoute()
{
    if (matchedRoute.upload)
    {
        // req.save call function to save the body
        std::stringstream resourcePath;
        resourcePath << this->srv->getRoot();
        if (matchedRoute.root.front() != '/' && resourcePath .str().back() != '/')
            resourcePath << "/";
        resourcePath << matchedRoute.root;
        // std::cout << "UPLOAD PATH :" << resourcePath.str() << std::endl;
        req.getReqBody().saveFile(resourcePath.str());
        return true;
    }
    return false;
}
void Response::post()
{
    if (foundRoute == false)
    {
        throw Server::ServerException("404 not found", 404);
    }
    else if (foundRoute)
    {
        if (find(matchedRoute.allowedMethods.begin(), matchedRoute.allowedMethods.end(), "POST") == matchedRoute.allowedMethods.end())
        {
            throw Server::ServerException("Method not allowed ", 405);
        }
        checkResource();
        if (checkUploadRoute())
        {
            header = "HTTP/1.1 201 Created\r\nContent-Type: text/html\r\nContent-Length: ";
            body = "content created";
            std::stringstream lengthStr;
            lengthStr << body.length();
            response = header + lengthStr.str() + "\r\n\r\n" + body;
            return ;
        }
        if (checkCgiResource())
            return;
        throw Server::ServerException("forbidden", 403); 
    }
}
#include <unistd.h>

int Response::deleteContent()
{
    if (isDirectory(reqResourcePath))
    {
        throw Server::ServerException("Conflict", 409);
    }
    else
    {
        return std::remove(reqResourcePath.c_str()) == 0;
    }
    return 0;
}
void Response::Delete()
{
    if (foundRoute == false)
    {
        throw Server::ServerException("404 not found", 404);
    }
    else if (foundRoute)
    {
        if (find(matchedRoute.allowedMethods.begin(), matchedRoute.allowedMethods.end(), "DELETE") == matchedRoute.allowedMethods.end())
        {
            throw Server::ServerException("Method not allowed ", 405);
        }
        DeletecheckResource();
        if (checkCgiResource())
            return;
        if (deleteContent() == 1)
        {
            header = "HTTP/1.1 204 No Content\r\nContent-Type: text/html\r\nContent-Length: ";
            body = "file deleted";
            std::stringstream lengthStr;
            lengthStr << body.length();
            response = header + lengthStr.str() + "\r\n\r\n" + body;
            return ;
        }
        throw Server::ServerException("Internal Server Error", 500); 
    }
}


