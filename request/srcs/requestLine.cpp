#include "requestLine.hpp"

void requestLine::setMethod(const std::string method)
{
    int i;
    std::string methods[] = {"GET", "HEAD", "OPTIONS", "TRACE", "PUT", "DELETE", "POST", "PATCH", "CONNECT"};
    std::string tmp = method;

    trim(tmp);
    toupper(tmp);
    for (i = 0; i < 9 && tmp != methods[i]; i++){}
    if (i == 8 && tmp != methods[8])
        throw "Invalid Method";
    this->method = (MethodType)i;
}

void requestLine::setReqTarget(const std::string url)
{
    std::string tmp = url;
    trim(tmp);
    if (!urlFormat(tmp))
        throw "Invalid URL";
    this->reqTarget = tmp;
}

void requestLine::setHttpVers(const std::string vers)
{
    std::string tmp = vers;
    trim(tmp);
    if (tmp != "HTTP/1.1")
        throw "Invalid HTTP Version";
    this->httpVers = tmp;
}


requestLine::requestLine(const std::string requestLine)
{
    std::string tmp = requestLine;
    trim(tmp);

    std::string method = tmp.substr(0, tmp.find(" "));
    std::string url = tmp.substr(tmp.find(" ") + 1, tmp.rfind(" ") - tmp.find(" ") - 1);
    std::string vers = tmp.substr(tmp.rfind(" ") + 1);

    setMethod(method);
    setReqTarget(url);
    setHttpVers(vers);
}

std::string requestLine::getMethod()
{
    std::string methods[] = {"GET", "HEAD", "OPTIONS", "TRACE", "PUT", "DELETE", "POST", "PATCH", "CONNECT"};
    return methods[this->method];
}

std::string requestLine::getReqTarget()
{
    return this->reqTarget;
}

std::string requestLine::getHttpVers()
{
    return this->httpVers;
}

