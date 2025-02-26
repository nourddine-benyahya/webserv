#include "requestLine.hpp"
#include "exeptions.hpp"
#include <cstdlib> 

bool isValidHex(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

bool isValidPercentEncoding(const std::string &str, size_t pos)
{
    return (pos + 2 < str.size() && isValidHex(str[pos + 1]) && isValidHex(str[pos + 2]));
}

std::string urlDecode(std::string &url)
{
    std::string decodedString;

    size_t i = 0;
    while (i < url.length())
    {
        if (url[i] == '%' && isValidPercentEncoding(url, i))
        {
            char hex[3]  = {url[i+1], url[i+2], '\0'};
            char decodedChar = static_cast<char>(std::strtol(hex,NULL, 16));
            if (decodedChar == '\0')
                throw exeptions(400, "400 Bad Request, Potential security issue (null-byte injection).");
            if (decodedChar == '\r' || decodedChar == '\n')
                throw exeptions(400, "400 Bad Request, Could be an injection attempt (e.g., newline in headers).");
            decodedString += decodedChar;
            i += 2;
        }
        else if (url[i] == '%')
                throw exeptions(400, "400 Bad Request, incorrect encoding.");
        else if (url[i] == '+')
            decodedString += ' ';
        else
            decodedString += url[i];
        i++;
    }
    if (decodedString.find("/..") != std::string::npos || decodedString.find("../") != std::string::npos || decodedString.find("/../") != std::string::npos)
        throw exeptions(403, "403 forbidden, directory traversal attack.");
    return decodedString;
}

void requestLine::setMethod(const std::string method)
{
    int i;
    std::string methods[] = {"GET", "HEAD", "OPTIONS", "TRACE", "PUT", "DELETE", "POST", "PATCH", "CONNECT"};
    std::string tmp = method;

    trim(tmp);
    toupper(tmp);
    for (i = 0; i < 9 && tmp != methods[i]; i++){}
    if (i == 8 && tmp != methods[8])
        throw exeptions(405, "405 Method Not Allowed");

        // throw "Invalid Method";
    this->method = (MethodType)i;
}

void requestLine::setReqTarget(const std::string url)
{
    std::string tmp = url;
    trim(tmp);
    urlFormat(tmp);
    this->reqTarget = tmp;
    this->reqTarget = urlDecode(this->reqTarget);
}

void requestLine::setHttpVers(const std::string vers)
{
    std::string tmp = vers;
    trim(tmp);
    if (tmp != "HTTP/1.1")
        throw exeptions(505, "505 HTTP Version Not Supported");
    this->httpVers = tmp;
}


requestLine::requestLine(const std::string requestLine)
{
    std::string tmp = requestLine;
    trim(tmp);

    std::string method = tmp.substr(0, tmp.find(" "));
    std::string url = tmp.substr(tmp.find(" ") + 1, tmp.rfind(" ") - tmp.find(" ") - 1);
    // std::cout  << "-------original-------" << decodurledUrl << "------------" << std::endl;
    setReqTarget(url);
    std::string decodedUrl = urlDecode(url);
    url = decodedUrl;
    // std::cout  << "-------decoded-------" << decodedUrl << "------------" << std::endl;
    std::string vers = tmp.substr(tmp.rfind(" ") + 1);

    setMethod(method);
    this->FullTarget = url;
    // std::cout << "FullTarget: " << this->FullTarget << std::endl;
    setHttpVers(vers);
    splitParamsFromReqTarget();
}

MethodType &requestLine::getMethod()
{
    return method;
}

std::string &requestLine::getReqTarget()
{
    return this->reqTarget;
}

std::string &requestLine::getHttpVers()
{
    return this->httpVers;
}

std::map<std::string, std::string> &requestLine::getParams()
{
    return this->params;
}

std::string &requestLine::getReqFullTarget()
{
    return this->FullTarget;
}

void requestLine::splitParamsFromReqTarget()
{
    std::string tmp = this->reqTarget;
    std::string key;
    std::string value;
    if (tmp.find("?") == std::string::npos)
        return ;
    this->reqTarget = tmp.substr(0, tmp.find("?"));
    std::string strParams = tmp.substr(tmp.find("?") + 1);

    std::istringstream ss(strParams);

    while (std::getline(ss, key, '&'))
    {
        std::istringstream ss2(key);
        std::getline(ss2, key, '=');
        std::getline(ss2, value);
        this->params[key] = value;
    }
};
