#include "cgi.hpp"

void cgi::saveCgiEnv()
{
    envComPath tmp;
    tmp.interpreter = "./cgi-bin/php-cgi";
    tmp.commandpath = "./cgi-bin/php-cgi";
    cgiEnv["php"] = tmp;

    tmp.interpreter = "python3";
    tmp.commandpath = "/usr/bin/python3";
    cgiEnv["py"] = tmp;

    tmp.interpreter = "sh";
    tmp.commandpath = "/bin/sh";
    cgiEnv["sh"] = tmp;

    tmp.interpreter = "java";
    tmp.commandpath = "/usr/bin/java";
    cgiEnv["java"] = tmp;
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
	env_map["PATH_INFO"] = CgiScript;
	env_map["SCRIPT_FILENAME"] = CgiScript;
	env_map["PATH_TRANSLATED"] = req.getReqLine().getReqTarget();
    env_map["SERVER_PROTOCOL"] = req.getReqLine().getHttpVers();
    env_map["SERVER_SOFTWARE"] = "webserv/1.0";
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["REDIRECT_STATUS"] = "1";
	env_map["SERVER_PORT"] = "80";
	env_map["REMOTE_HOST"] = "localhost";

    //params
    std::map<std::string, std::string> params = req.getReqLine().getParams();
    std::string query_string;
    for (std::map<std::string, std::string>::iterator it = params.begin(); it != params.end(); ++it) {
        query_string += it->first + "=" + it->second + "&";
    }
    query_string.pop_back();
    env_map["QUERY_STRING"] = query_string;

    //all header
    std::map<std::string, std::string> header = req.getReqHeader().getHeader();
    for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it) {
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


void cgi::pathToScript()
{
    CgiScript = CGI_DIR + req.getReqLine().getReqTarget();
    if (CgiScript.back() == '/')
        CgiScript.pop_back();
    if (!fileExists(CgiScript.c_str())) {
        std::cerr << "CGI script does not exist or is not executable: " << CgiScript.c_str() << std::endl;
        throw "File not found 1";
    }
}

void cgi::runCgi()
{
    std::string ext = CgiScript.substr(CgiScript.rfind(".") + 1);

    if (cgiEnv.find(ext) == cgiEnv.end()) {
        std::cerr << "CGI interpreter not found for extension: " << ext << std::endl;
        throw "File not found 2";
    }

    std::string interpreter = cgiEnv[ext].interpreter;
    std::string commandpath = cgiEnv[ext].commandpath;

    if (!fileExists(commandpath.c_str())) {
        std::cerr << "CGI interpreter does not exist or is not executable: " << commandpath.c_str() << std::endl;
        throw "File not found 3";
    }
    char *const argv[] = {(char *)interpreter.c_str()  ,(char *)CgiScript.c_str(), NULL};

    int stdout_pipe[2], stdin_pipe[2];
    if (pipe(stdout_pipe) == -1 || pipe(stdin_pipe) == -1)
    {
        std::cerr << "Pipe creation failed\n";
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed\n";
        return;
    }

    if (pid == 0) { 
        close(stdin_pipe[1]); 
        dup2(stdin_pipe[0], STDIN_FILENO);
        close(stdin_pipe[0]);

        close(stdout_pipe[0]);
        dup2(stdout_pipe[1], STDOUT_FILENO);
        close(stdout_pipe[1]);


        char **envp = mapToPtr();
        if ((execve(commandpath.c_str(), argv, envp)) < 0)
        {
            std::cerr << "Exec failed: " << errno << CgiScript << std::endl;
            exit(-1);
        }
    } 
    else
    {
        close(stdin_pipe[0]);
        close(stdout_pipe[1]);

        if (req.getReqHeader().getHeader().find("Content-Length") != req.getReqHeader().getHeader().end()) {
            write(stdin_pipe[1], req.getReqBody().getFullBody().c_str(), req.getReqBody().getFullBody().size());
        }
        close(stdin_pipe[1]);

        char buffer[1024];
        std::string response;
        ssize_t bytesRead;
        while ((bytesRead = read(stdout_pipe[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            response += buffer;
        }
        close(stdout_pipe[0]);

        int status;
        waitpid(pid, &status, 0); // Wait for CGI process
        if (WEXITSTATUS(status) == -1)
            throw "Exit failed";
            // throw strerror(errno);
        cgiResponse = response;
    }
}

void cgi::setCgiEnv(std::string interpreter, std::string commandpath)
{
    envComPath tmp;
    tmp.interpreter = interpreter;
    tmp.commandpath = commandpath;
    cgiEnv[interpreter] = tmp;
}


cgi::cgi(request request) {

    req = request;

    // save url of cgi script
    pathToScript();

    // save exeuatable path
    saveCgiEnv();


    //save env
    save_env();
}
