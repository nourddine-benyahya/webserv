#include "cgi.hpp"

std::map<std::string, std::string> save_env(request req)
{
    std::map<std::string, std::string> env_map;

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

    //http version
    env_map["SERVER_PROTOCOL"] = req.getReqLine().getHttpVers();

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
        env_map[it->first] = it->second;
    }

    //request target
    env_map["REQUEST_URI"] = req.getReqLine().getReqTarget();

    //path info
    // if (req.getReqLine().getMethod() == POST)
        env_map["PATH_INFO"] = req.getReqBody().getFormFields()["file_name"];

    return env_map;
}

char **maptochar(std::map<std::string, std::string> env_map)
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

std::string commandToExecute(std::string cgi_script) {
    std::string extansion = cgi_script.substr(cgi_script.find_last_of(".") + 1);
    if (extansion == "py")
        return "python3";
    else if (extansion == "php")
        return "php";
    else if (extansion == "sh")
        return "sh";
    else
        return cgi_script;
}

std::string pathtocommand(std::string command)
{
    if (command == "php")
        return "/usr/bin/php";
    else if (command == "python3")
        return "/usr/bin/python3";
    else if (command == "sh")
        return "/bin/sh";
    else
        return command;
}

// Check if the file exists and is executable
bool fileExists(const char *path) {
    return (access(path, X_OK) == 0);
}
void handlCgi(request req) {
    std::string cgi_script = CGI_DIR + req.getReqLine().getReqTarget();
    if (cgi_script.back() == '/')
        cgi_script.pop_back();

    if (!fileExists(cgi_script.c_str())) {
        std::cerr << "CGI script does not exist or is not executable: " << cgi_script.c_str() << std::endl;
        throw FILE_NOT_FOUND;
        return;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe creation failed\n";
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed\n";
        return;
    }

    if (pid == 0) { // Child process (CGI)
        close(pipefd[0]); // Close read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[1]);

        std::map<std::string, std::string> env_map = save_env(req);
        char **envp = maptochar(env_map);

        // Prepare arguments for execve
        char *const argv[] = {(char *)commandToExecute(cgi_script).c_str()  ,(char *)cgi_script.c_str(), NULL };

        if ((execve(pathtocommand(commandToExecute(cgi_script)).c_str(), argv, envp)) < 0)
        {
            std::cerr << "Exec failed: " << errno << cgi_script << std::endl;
            exit(-1);
        }
    } else { // Parent process
        close(pipefd[1]); // Close write end

        char buffer[1024];
        std::string response;
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';  // Ensure proper string termination
            response += buffer;
        }
        close(pipefd[0]);

        int status;
        waitpid(pid, &status, 0); // Wait for CGI process
        if (WEXITSTATUS(status) != 0)
            throw SCRIPT_EXEC_ERROR;

        std::cout << "CGI Response:\n" << response << std::endl;
    }
}
