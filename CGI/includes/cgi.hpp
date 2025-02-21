#pragma once

#include "Server.hpp"
#include "request.hpp"
#include "parsFuncs.hpp"
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>





class cgi
{
    private:
        request req;
        std::string cgiResponse;
        std::string CgiScript;
        std::map<std::string, std::string> env_map;
        std::map<std::string, std::string> cgiEnv;
        std::stringstream port;
    public :
        void save_env();
        char **mapToPtr();
        void pathToScript();
        void saveCgiEnv();


        cgi(request req, const std::string path, int port);
        void runCgi();
        void setCgiEnv(std::string interpreter, std::string commandpath);
        std::string &getResponse() { return cgiResponse; }
};

void handlCgi(request req);
