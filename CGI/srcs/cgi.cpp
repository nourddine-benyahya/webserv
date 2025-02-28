#include "cgi.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>  

void cgi::saveCgiEnv()
{
    cgiEnv[".php"] = "./cgi-bin/php-cgi";
    cgiEnv[".py"] = "/usr/bin/python3";
    cgiEnv[".sh"] = "/bin/sh";
    cgiEnv[".java"] = "/usr/bin/java";
}


std::string toupper(std::string str)
{
    std::string tmp = str;
    for (size_t i = 0; i < tmp.length(); i++)
    {
        if (tmp[i] == '-')
            tmp[i] = '_';
        tmp[i] = std::toupper(tmp[i]);
    }
    return tmp;
}

void cgi::save_env()
{
    //methods
    switch (req.getReqLine().getMethod()) {
        case GET: env_map["REQUEST_METHOD"] = "GET"; break;
        case HEAD: env_map["REQUEST_METHOD"] = "HEAD"; break;
        case OPTIONS: env_map["REQUEST_METHOD"] = "OPTIONS"; break;
        case TRACE: env_map["REQUEST_METHOD"] = "TRACE"; break;
        case PUT: env_map["REQUEST_METHOD"] = "PUT"; break;
        case DELETE: env_map["REQUEST_METHOD"] = "DELETE"; break;
        case POST: env_map["REQUEST_METHOD"] = "POST"; break;
        case PATCH: env_map["REQUEST_METHOD"] = "PATCH"; break;
        case CONNECT: env_map["REQUEST_METHOD"] = "CONNECT"; break;
        default: env_map["REQUEST_METHOD"] = "UNKNOWN";
    }

    env_map["REQUEST_URI"] = CgiScript;
	env_map["SCRIPT_FILENAME"] = CgiScript;
    env_map["SERVER_PROTOCOL"] = req.getReqLine().getHttpVers();
    env_map["SERVER_SOFTWARE"] = "webserv/1.0";
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["REDIRECT_STATUS"] = "1";
	env_map["SERVER_PORT"] = port.str();

    //params
    std::map<std::string, std::string> params = req.getReqLine().getParams();
    std::string query_string;
    for (std::map<std::string, std::string>::iterator it = params.begin(); it != params.end(); ++it) {
        query_string += it->first + "=" + it->second + "&";
    }
    if (query_string.size() > 0)
        query_string.erase(query_string.size() - 1);
    env_map["QUERY_STRING"] = query_string;

    //all header
    std::map<std::string, std::string> header = req.getReqHeader().getHeader();
    for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it) {
        if (it->first == "Cookie")
            env_map["HTTP_COOKIE"] = it->second;
        else if (it->first == "User-Agent")
            env_map["HTTP_USER_AGENT"] = it->second;
        else if (it->first == "Referer")
            env_map["HTTP_REFERER"] = it->second;
        else
            env_map[toupper(it->first)] = it->second;
    }
}

char **cgi::mapToPtr()
{
    char **envp = new char*[env_map.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::iterator it = env_map.begin(); it != env_map.end(); ++it) {
        std::string tmp = it->first + "=" + it->second;
        envp[i] = new char[tmp.length() + 1];
        strcpy(envp[i], tmp.c_str());
        i++;
    }
    envp[i] = NULL;
    return envp;
}

void cgi::runCgi()
{
    int fd = -1;
    std::string ext = CgiScript.substr(CgiScript.rfind("."));
    if (cgiEnv.find(ext) == cgiEnv.end()) 
        throw Server::ServerException("500 CGI interpreter not set for " + ext, 500);

    std::string commandpath = cgiEnv[ext];
    if (!fileExists(commandpath.c_str()))
        throw Server::ServerException("500 cgi interpreter " + commandpath + " not correct or permition not seted for " + ext, 500);

    char *const argv[] = {(char *)commandpath.c_str(), (char *)CgiScript.c_str(), NULL};

    
    if (req.getReqHeader().getHeader().find("Content-Length") != req.getReqHeader().getHeader().end())
    {
        fd = open("/tmp/.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if (fd == -1)
            throw Server::ServerException("500 error while opening tmp file", 500);
        const std::string &bodydata = req.getReqBody().getFullBody();
            ssize_t written = write(fd, bodydata.c_str(), bodydata.size());
        if (written == -1)
            throw Server::ServerException("500 error while writing in file", 500);
        close(fd);
        fd = open("/tmp/.tmp", O_RDONLY);
        if (fd == -1)
            throw Server::ServerException("500 error while opening tmp file", 500);
    }

    int stdout_pipe[2];
    if (pipe(stdout_pipe) == -1)
        throw Server::ServerException("500 error while piping", 500);

    pid_t pid = fork();
    if (pid < 0)
        throw Server::ServerException("500 error while forking", 500);

    if (pid == 0) { 
        if (fd != -1)
        {
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        dup2(stdout_pipe[1], STDOUT_FILENO);
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);

        char **envp = mapToPtr();
        execve(commandpath.c_str(), argv, envp);
        std::cerr << "Exec failed: " << strerror(errno) << ": " << CgiScript << std::endl;
        exit(-1);
    } 
    else {
        if (fd != -1)
        {
            close(fd);
            std::remove("/tmp/.tmp");
        }
        close(stdout_pipe[1]);

        int status;
        waitpid(pid, &status, 0);
                char buffer[1024];

        std::string response;
        ssize_t bytesRead;
        while ((bytesRead = read(stdout_pipe[0], buffer, sizeof(buffer) - 1)) > 0){
            buffer[bytesRead] = '\0';
            response += buffer;
        }
        close(stdout_pipe[0]);
        if (bytesRead == -1)
            throw Server::ServerException("500 error while reading from pipe", 500);
        if (WIFSIGNALED(status)) {
            throw Server::ServerException("500 CGI process terminated abnormally", 500);
        }
        if (WEXITSTATUS(status) != 0) {
            throw Server::ServerException("500 CGI execution failed", 500);
        }
        

        cgiResponse = "HTTP/1.1 200 OK\r\n";
        cgiResponse += response;
    }
}

void cgi::setCgiEnv(std::string extantion, std::string commandpath)
{
    std::string ext = extantion;
    std::string path = commandpath;
    cgiEnv[ext] = path;
}


cgi::cgi(request request, std::string path, int port) {

    req = request;
    CgiScript = path;
    this->port << port;
    // save exeuatable path
    saveCgiEnv();
    //save env
    save_env();
}
