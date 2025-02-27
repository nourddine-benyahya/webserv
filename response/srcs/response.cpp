#include <sys/stat.h>
#include "response.hpp"
#include <unistd.h>
static std::map<std::string, std::string> mimeTypes;
//PROBLEM IN MATCH ROUTE
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
void initializeMimeTypes() {
    mimeTypes[".html"] = "text/html";
    mimeTypes[".css"] = "text/css";
    mimeTypes[".js"] = "application/javascript";
    mimeTypes[".json"] = "application/json";
    mimeTypes[".png"] = "image/png";
    mimeTypes[".jpg"] = "image/jpeg";
    mimeTypes[".jpeg"] = "image/jpeg";
    mimeTypes[".gif"] = "image/gif";
}
std::string getContentType(const std::string& extension) {
    if (mimeTypes.empty()) {
        initializeMimeTypes();
    }

    std::map<std::string, std::string>::iterator it = mimeTypes.find(extension);
    if (it != mimeTypes.end()) {
        return it->second;
    }
    return "text/plain";
}
std::string Mytrim(std::string str)
{
    std::string whiteSpaces(" \t\f\v\n\r/");

    while(whiteSpaces.find(str[0]) != std::string::npos) str.erase(str.begin());
    while(whiteSpaces.find(str[str.size() - 1]) != std::string::npos) str.erase(str.size() - 1);
    return (str);
}
std::map<std::string, Route>::iterator myFind(std::map<std::string, Route>& m, const std::string& path)
{
    std::map<std::string, Route>::iterator it = m.begin();
    while (it != m.end())
    {
        if (Mytrim(it->first) == Mytrim(path))
            return it;
        ++it;
    }
    return m.end();
}
void Response::matchRoute()
{
    foundRoute = false;
    std::map<std::string , Route>::iterator it = srv->routes.begin();
    // it = srv->routes.find((req.getReqLine().getReqTarget()));
    it = myFind(srv->routes, req.getReqLine().getReqTarget());
    if (it != srv->routes.end())
    {
        matchedRoute = it->second;
        foundRoute = true;
    }
    else if (srv->routes.find("/") != srv->routes.end())
    {
        matchedRoute = srv->routes.find("/")->second;
        foundRoute = true;
    }
    std::cout << "matched route :" << matchedRoute.path << std::endl;
}
Response::Response(request r, Server::Config *server)
{
    req = r;
    srv = server;
    indexed = false;
    matchRoute();
    try
    {
        if (checkRedir() == true)
        {
            return;
        }
        if (req.getReqLine().getMethod() == GET)
        {
            get();
        }
        else if (req.getReqLine().getMethod() == POST)
        {
            post();
        }
        else if (req.getReqLine().getMethod() == DELETE)
        {
            Delete();
        }
        else
        {
            throw Server::ServerException("Method not allowed", 405);
        }
    }
    catch (Server::ServerException &e) {
		body = this->srv->getErrorPage(e.getStatus());
        response = e.createHTTPErrorHeader(body.length()) + body;
		throw Server::ServerException(e.what(), response, e.getStatus());
	}
}
void Response::checkFile(std::string fileName)
{
    std::ifstream resource(fileName.c_str());
    if (!resource.is_open())
    {
        if (!indexed)
            throw Server::ServerException("file not found:" + fileName, 404);
        else
            throw Server::ServerException("forbidden" + fileName, 403);

    }
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
    if (!routeRoot.empty() && routeRoot[0] != '/' && resourcePath.str()[resourcePath.str().size() - 1] != '/')
        resourcePath << "/";
    resourcePath << routeRoot;
    if (!path.empty() && path[0] != '/' && resourcePath.str()[resourcePath.str().size() - 1] != '/')
        resourcePath << "/";
    if (resourcePath.str()[resourcePath.str().size() - 1] == '/' && path == "/")
        return ;
    resourcePath << path;
}
void Response::redirectToFolder()
{
    std::stringstream resourcePath;
    if (req.getReqLine().getMethod() == GET)
        resourcePath << "HTTP/1.1 301 Moved Permanently\r\n";
    else
        resourcePath << "HTTP/1.1 308 Permanent Redirect\r\n";
    resourcePath << "Location: http://" + srv->getName();
    resourcePath << ":";
    resourcePath << srv->getPort();
    resourcePath << req.getReqLine().getReqTarget() << "/\r\n";
    resourcePath << "Content-Length: 0\r\n\r\n";
    response = resourcePath.str();
}
bool checkExistence(std::string &path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return false;
    return true;
}
bool fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

void checkRPerm(std::string &path)
{
    if (access(path.c_str(), R_OK) != 0)
            throw Server::ServerException("Iternal Server Error", 500);
}
bool Response::checkResource()
{

    std::stringstream resourcePath;
    checkSlash(resourcePath, srv->getRoot(), matchedRoute.root, req.getReqLine().getReqTarget());
    reqResourcePath = resourcePath.str();
    if (isDirectory(reqResourcePath))
    {
        std::string t1 = reqResourcePath + matchedRoute.index;
        std::string t2 = reqResourcePath + srv->fileIndex;
        if (req.getReqLine().getReqTarget()[req.getReqLine().getReqTarget().size() - 1] != '/')
        {
            redirectToFolder();
            return true;
        }
        else if (!matchedRoute.index.empty() && fileExists(t1) && access(t1.c_str(), R_OK) == 0)
        {
            if (reqResourcePath[reqResourcePath.size() - 1] != '/' &&  matchedRoute.index[0] != '/')
                reqResourcePath += "/";
            reqResourcePath +=  matchedRoute.index;
            indexed = true;
            checkFile(reqResourcePath);
        }
        else if (!srv->fileIndex.empty() && fileExists(t2) && access(t2.c_str(), R_OK) == 0)
        {
            if (reqResourcePath[reqResourcePath.size() - 1] != '/' &&  srv->fileIndex[0] != '/')
                reqResourcePath += "/";
            reqResourcePath +=  srv->fileIndex;
            indexed = true;
            checkFile(reqResourcePath);
        }
        else if (checkIndexed())
            return false;
        else if (req.getReqLine().getMethod() == GET && matchedRoute.list_dirs)
        {
            header = "HTTP/1.1 200 OK\r\nContent-Length: ";
            body = listDir(reqResourcePath);
            std::stringstream lengthStr;
            lengthStr << body.length();
            response = header + lengthStr.str() + "\r\n\r\n" + body;
            return true;
        }
        else
        {
            throw Server::ServerException("forbidden", 403);
        }
    }
    if (fileExists(reqResourcePath))
    {
        checkRPerm(reqResourcePath);
    }
    else
        throw Server::ServerException("file not found:" + reqResourcePath, 404);
    return false;
}
void Response::DeletecheckResource()
{
    std::stringstream resourcePath;
    checkSlash(resourcePath, srv->getRoot(), matchedRoute.root, req.getReqLine().getReqTarget());
    reqResourcePath = resourcePath.str();
    if (isDirectory(reqResourcePath))
    {
        std::string tmp = reqResourcePath + matchedRoute.index;
        std::string tmp2 = reqResourcePath + srv->fileIndex;
        if (!matchedRoute.index.empty() && fileExists(tmp))
        {
            reqResourcePath +=  matchedRoute.index;
            indexed = true;
        }
        else if (!srv->fileIndex.empty() && fileExists(tmp2))
        {
            reqResourcePath +=  srv->fileIndex;
            indexed = true;
        }
    }
    if (fileExists(reqResourcePath))
    {
        if (access(reqResourcePath.c_str(), R_OK) != 0)
            throw Server::ServerException("Iternal Server Error", 500);
    }
    else
        throw Server::ServerException("file not found: " + reqResourcePath, 404);
}

bool Response::checkCgiResource()
{
    if (matchedRoute.hasCgi == false)
        return false;
    int port = srv->getPort();
    cgi c(req, reqResourcePath, port);
    //set env
    for (std::map<std::string, std::string>::iterator it = matchedRoute.cgis.begin() ; it != matchedRoute.cgis.end(); it++)
    {
        c.setCgiEnv(it->first, it->second);
    }
    c.runCgi();
    response = c.getResponse();
    return true;
}
std::string getContent(std::string fileName)
{
    std::ifstream resource(fileName.c_str());
    if (!resource.is_open())
        throw Server::ServerException("file not found: " + fileName, 404);
    std::stringstream content;
    content << resource.rdbuf();
    return content.str();
}
bool Response::checkRedir()
{
    if (matchedRoute.redir.empty())
        return false;
    std::stringstream resourcePath;
    resourcePath << "HTTP/1.1 301 Moved Permanently\r\n";
    resourcePath << "Location: http://" + srv->getName();
    resourcePath << ":";
    resourcePath << srv->getPort();
    resourcePath << matchedRoute.redir << "\r\n";
    resourcePath << "Content-Length: 0\r\n\r\n";
    response = resourcePath.str();
    return true;
}
void Response::get()
{
    if (foundRoute == false)
    {
        std::stringstream resourcePath;
        checkSlash(resourcePath, srv->getRoot(), matchedRoute.root, req.getReqLine().getReqTarget());
        reqResourcePath = resourcePath.str();
        if (isDirectory(reqResourcePath))
        {
            if (req.getReqLine().getReqTarget()[req.getReqLine().getReqTarget().size() - 1] != '/')
            {

                redirectToFolder();
                return ;
            }
        }
        throw Server::ServerException("file not found: " + reqResourcePath, 404);
    }
    else if (foundRoute)
    {
        if (find(matchedRoute.allowedMethods.begin(), matchedRoute.allowedMethods.end(), "GET") == matchedRoute.allowedMethods.end())
            throw Server::ServerException("Method not allowed", 405);
        if (checkResource())
            return ;
        if (checkCgiResource())
            return;
        std::string extension = reqResourcePath.substr(reqResourcePath.find_last_of('.'));
        std::string contentType = getContentType(extension);
        header = "HTTP/1.1 200 OK\r\nContent-Type: " + contentType + "\r\nContent-Length: ";
        body = getContent(reqResourcePath);
        std::stringstream lengthStr;
        lengthStr << body.length();
        response = header + lengthStr.str() + "\r\n\r\n" + body;
    }
}

bool Response::checkIndexed()
{
    std::stringstream resourcePath;
    resourcePath << reqResourcePath; 
    if (reqResourcePath[reqResourcePath.size() - 1] != '/')
        resourcePath << "/";
    resourcePath << "index.html";
    std::ifstream resource(resourcePath.str().c_str());
    if (!resource.is_open())
    {
        return false;
    }
    reqResourcePath = resourcePath.str();

    resource.close();
    return true;
}
bool checkDirUrl(std::string dir, std::string target)
{
    if (isDirectory(dir + "/" + target))
        return true;
    return false;
}
bool Response::checkUploadRoute()
{
    if (matchedRoute.upload)
    {
        // req.save call function to save the body
        std::stringstream resourcePath;
        resourcePath << this->srv->getRoot();
        if (matchedRoute.root[0] != '/' && resourcePath.str()[resourcePath.str().size() - 1] != '/')
        {
            resourcePath << "/";
        }
        resourcePath << matchedRoute.root;
        if (resourcePath.str()[resourcePath.str().size() - 1] != '/' &&  req.getReqLine().getReqTarget()[0] != '/')
            resourcePath << "/";
        if (checkDirUrl(resourcePath.str(), req.getReqLine().getReqTarget()))
        {
            resourcePath << req.getReqLine().getReqTarget();
            if (resourcePath.str()[resourcePath.str().size() - 1] != '/')
                resourcePath << "/";
        }
        if (fileExists(resourcePath.str()) && access(resourcePath.str().c_str(), R_OK) == 0)
        {
            if (req.getReqBody().saveFile(resourcePath.str()))
                throw Server::ServerException("Bad Request", 400);
        }
        else if (fileExists(resourcePath.str()) && access(resourcePath.str().c_str(), R_OK) != 0)
            throw Server::ServerException("Iternal server error", 500);
        else
        {
            throw Server::ServerException("file not found", 404);
        }
        return true;
    }
    return false;
}
void Response::post()
{
    std::stringstream resourcePath;
    checkSlash(resourcePath, srv->getRoot(), matchedRoute.root, req.getReqLine().getReqTarget());
    reqResourcePath = resourcePath.str();
    if (isDirectory(reqResourcePath))
    {
        if (req.getReqLine().getReqTarget()[req.getReqLine().getReqTarget().size() - 1] != '/')
        {
            redirectToFolder();
            return ;
        }
    }
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
        if (checkUploadRoute())
        {
            header = "HTTP/1.1 201 Created\r\nContent-Type: text/html\r\nContent-Length: ";
            body = "content created";
            std::stringstream lengthStr;
            lengthStr << body.length();
            response = header + lengthStr.str() + "\r\n\r\n" + body;
            return ;
        }
        checkResource();
        if (checkCgiResource())
            return;
        throw Server::ServerException("forbidden", 403); 
    }
}

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
    std::stringstream resourcePath;
    checkSlash(resourcePath, srv->getRoot(), matchedRoute.root, req.getReqLine().getReqTarget());
    reqResourcePath = resourcePath.str();
    if (isDirectory(reqResourcePath))
    {
        if (req.getReqLine().getReqTarget()[req.getReqLine().getReqTarget().size() - 1] != '/')
        {

            redirectToFolder();
            return ;
        }
    }
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
                    << "\" style=\"pointer-events: none\">" << entry->d_name << "</a></li>";
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

