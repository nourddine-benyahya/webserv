#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include "response.hpp"
// #include 
void handleRequest(int clientSock)
{
    char buffer[1048];

    int byteRead = recv(clientSock, buffer, 1047, 0);
    if (byteRead < 0)
    {
        std::cerr << "error with the recv function" << std::endl;
        // exit(1);
        return;
    }
    buffer[byteRead] = 0;

    std::string bu = buffer;
    std::istringstream request (bu);
    std::string line;
    while (std::getline(request, line))
    {
        std::cout << "line : " << line << std::endl;
        if (line.substr(0, 3) == "GET")
        {
            Response res;
            res.method = "GET";
            res.GET(line, clientSock);
        }
        else if (line.substr(0, 4) == "POST")
        {
            Response res;
            res.method = "POST";
            res.POST(line, clientSock);
        }
    }
}
int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Failed to create socket\n";
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(80);

    if (bind(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Bind failed\n";
        close(sock);
        return -1;
    }
    int i = 1;
    while (true)
    {

    if (listen(sock, 5) < 0) {
        std::cerr << "Listen failed\n";
        close(sock);
        return -1;
    }

    std::cout << "Server listening on port 6969...\n";

    sockaddr_in clientAddress;
    socklen_t clientAddrLen = sizeof(clientAddress);
    int clientSock = accept(sock, (struct sockaddr*)&clientAddress, &clientAddrLen);
    if (clientSock < 0) {
        std::cerr << "Accept failed\n";
        close(sock);
        return -1;
    }

    std::string num(1, i++ + '0');
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello Client";
    handleRequest(clientSock);
    close(clientSock);
    }
    close(sock);
    return 0;
}