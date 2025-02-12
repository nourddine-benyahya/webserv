#include "response.hpp"
#include <sys/stat.h>
#include <unistd.h>

void Response::matchRoute()
{
    std::cout << req.getReqLine().getReqTarget() << std::endl;
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
        std::cout << "root route found" << std::endl;
        matchedRoute = srv->routes.find("/")->second;
        foundRoute = true;
    }
}
Response::Response(request r, Server::Config *server)
{
    req = r;
    srv = server;
    matchRoute();
    try
    {
        if (req.getReqLine().getMethod() == GET)
        {
            get();
        }
        else if (req.getReqLine().getMethod() == POST)
        {
            post();
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

    if (!routeRoot.empty() && routeRoot.front() != '/' && resourcePath.str().back() != '/')
        resourcePath << "/";
    resourcePath << routeRoot;
    if (!path.empty() && path.front() != '/' && resourcePath.str().back() != '/')
        resourcePath << "/";

    resourcePath << path;
    std::cout << "in Slash " << resourcePath.str() << "|" << path << "|" << std::endl;
}

void Response::checkResource()
{
    std::stringstream resourcePath;
    checkSlash(resourcePath, srv->getRoot(), matchedRoute.root, req.getReqLine().getReqTarget());
    reqResourcePath = resourcePath.str();
    std::cout << "Checking resource: " << reqResourcePath << std::endl;
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
    // std::cout << "final"<< reqResourcePath << std::endl;
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
        std::cout << "UPLOAD PATH :" << resourcePath.str() << std::endl;
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
        if (std::find(matchedRoute.allowedMethods.begin(), matchedRoute.allowedMethods.end(), "POST") == matchedRoute.allowedMethods.end())
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

void Response::Delete()
{

}

std::string Response::listDir(std::string path) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    std::stringstream result;

    if ((dir = opendir(path.c_str())) == NULL) {
        perror("opendir");
        return "";
    }

    result << "<ul>";

    while ((entry = readdir(dir)) != NULL) {
        std::string fullPath = path + "/" + entry->d_name;
        if (stat(fullPath.c_str(), &info) != 0) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
                continue;
            result << "<li>Directory: <a href=\"#\" onclick=\"toggleVisibility('" << fullPath << "')\">" << entry->d_name << "</a>";
            result << "<div id=\"" << fullPath << "\" style=\"display:none;\">";
            result << listDir(fullPath); // Recursively list the directory contents
            result << "</div></li>";
        } else {
            result << "<li>File: <a href=\"" << fullPath << "\">" << entry->d_name << "</a></li>";
        }
    }

    result << "</ul>";
    closedir(dir);
    return result.str();
}



