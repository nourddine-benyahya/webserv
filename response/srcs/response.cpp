#include <sys/stat.h>
#include "response.hpp"
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
    DIR* dir;
    struct dirent* entry;
    struct stat info;
    std::ostringstream result;

    if ((dir = opendir(path.c_str())) == NULL) {
        throw Server::ServerException("forbidden", 403);
        return "";
    }
    result << "<div id=\"directory-listing\">";
    result << "<ul>";

    while ((entry = readdir(dir)) != NULL) {
        std::string fullPath = path + "/" + entry->d_name;
        if (stat(fullPath.c_str(), &info) != 0) {
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
                continue;
            result << "<li><b onclick=\"toggleDir(event)\">/" << entry->d_name << " >> </b>"
                   << "<ul style=\"display:none;\">" << listDir(fullPath) << "</ul>"
                   << "</li>";
        } else {
            result << "<li><a href=\"" << fullPath 
                   << "\">" << entry->d_name << "</a></li>";
        }
    }

    result << "</ul>";
    result << "</div>";
    closedir(dir);
	result << "<style>"
           << "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f0f0f0; }"
           << "#directory-listing { margin: 0; padding: 0; list-style-type: none; }"
           << "#directory-listing ul { margin: 0; padding: 0; list-style-type: none; }"
           << "#directory-listing li { margin: 5px 0; padding: 5px; border: 1px solid #ddd; border-radius: 4px;  background-color: #FFF5EE;}"
           << "#directory-listing li a { text-decoration: none; color: #4682B4; }"
           << "#directory-listing li a:hover { text-decoration: underline; }"
           << "#directory-listing li b { font-weight: bold; color: #191970; }"
           << "#directory-listing li ul { margin-left: 20px; border-left: 2px solid #ddd; padding-left: 10px; }"
           << "#directory-listing li ul li { border: none; padding: 2px 0; }"
           << "</style>";
	
	result << "<script>"
           << "function toggleDir(event) {"
           << "  var nextUl = event.target.nextElementSibling;"
           << "  if (nextUl.style.display === 'none' || nextUl.style.display === '') {"
           << "    nextUl.style.display = 'block';"
           << "  } else {"
           << "    nextUl.style.display = 'none';"
           << "  }"
           << "}"
           << "</script>";
    return result.str();
}


