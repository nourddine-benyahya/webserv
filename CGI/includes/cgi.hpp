#pragma once

#include "request.hpp"
#include "parsFuncs.hpp"
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>


struct envComPath
{
    std::string interpreter;
    std::string commandpath;
};


class cgi
{
    private:
        bool err;
        request req;
        std::string cgiResponse;
        std::string CgiScript;
        std::map<std::string, std::string> env_map;
        std::map<std::string, envComPath> cgiEnv;
    public :
        void save_env();
        char **mapToPtr();
        void pathToScript();
        void saveCgiEnv();


        cgi(request req);
        void runCgi();
        void setCgiEnv(std::string interpreter, std::string commandpath);
        std::string &getResponse() { return cgiResponse; }
};

void handlCgi(request req);
