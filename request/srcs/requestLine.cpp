#include "requestLine.hpp"

void trim(std::string& str)
{
   std::string whiteSpaces(" \t\f\v\n\r");

   while(whiteSpaces.find(str[0]) != std::string::npos) str.erase(str.begin());
   while(whiteSpaces.find(str[str.size() - 1]) != std::string::npos) str.pop_back();
}

void toupper(std::string& str)
{
    for (int i = 0; i < str.size(); i++)
        str[i] = std::toupper(str[i]);
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
        throw "Invalid Method";
    this->method = (MethodType)i;
}


bool urlFormat(std::string& url) {
    const std::string allowed_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "abcdefghijklmnopqrstuvwxyz"
                                      "0123456789"
                                      "-._~:/?#[]@!$&'()*+,;=";

    if (url.empty() || url[0] != '/' || url.length() > 2048)
        return false;


    for (std::string::const_iterator it = url.begin(); it != url.end(); ++it) {
        if (allowed_chars.find(*it) == std::string::npos)
            return false;
    }

    size_t query_pos = url.find('?');
    size_t fragment_pos = url.find('#');

    if (fragment_pos != std::string::npos && query_pos != std::string::npos && fragment_pos < query_pos) {
        return false;
    }

    if (query_pos != std::string::npos && fragment_pos != std::string::npos && fragment_pos < query_pos)
        return false;

    if (query_pos != std::string::npos && query_pos == url.length() - 1)
        return false;

    if (fragment_pos != std::string::npos && fragment_pos == url.length() - 1)
        return false;

    return true;
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

