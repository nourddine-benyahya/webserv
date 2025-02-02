#include "../request/includes/response.hpp"

Response::Response(request r)
{
    req = r;
    req.printRequestLine();
    req.printRequestHeader();
    req.printRequestBody();
}

void Response::GET(std::string line, int clientSock)
{
    std::string reques = line.substr(4, line.length() - 4);
    size_t len = reques.find(" ");

    std::string url = reques.substr(1, len - 1);
    std::cout << "URL |" << url << "|" << std::endl;
    std::cout << "get method requesuested !" << std::endl;
    std::ifstream file(url);
    if (url == "favicon.ico")
        return ;
    if (!file.is_open())
    {
        std::ifstream file404("404.html");
        if (!file404.is_open())
        {
            std::cerr << "error opening 404.html" << std::endl;
            return;
        }
        std::cerr << "error with the file" << std::endl;
        std::ostringstream fileContent;
        fileContent << file404.rdbuf();
        body = fileContent.str();
        header = "HTTP/1.1 404 KO\r\nContent-Length: ";
        std::stringstream ss;
        ss << body.length();
        std::string lengthStr = ss.str();
        std::string response = header + lengthStr + "\r\n\r\n" + body;
        send(clientSock, response.c_str(), response.length(), 0);
        file404.close();
        return ;
    }
    std::string str;
    std::ostringstream fileContent;
    fileContent << file.rdbuf();
    body = fileContent.str();
    header = "HTTP/1.1 200 OK\r\nContent-Length: ";
    std::stringstream ss;
    ss << body.length();
    std::string lengthStr = ss.str();
    std::string response = header + lengthStr + "\r\n\r\n" + body;
    send(clientSock, response.c_str(), response.length(), 0);
}
void Response::POST(std::string line, int clientSocket)
{
    (void)line;
    std::string response =  "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/html\r\n"
                            "Connection: close\r\n\r\n"
                            "<html><body><h1>Form Submitted Successfully</h1></body></html>";
    
    send(clientSocket, response.c_str(), response.length(), 0);

    close(clientSocket);
}

void Response::DELETE()
{

}
void Response::methodFilter()
{
    // if ()
}

