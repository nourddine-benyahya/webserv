#include "request.hpp"

void trim(std::string& str)
{
   while(str[0] == ' ') str.erase(str.begin());
   while(str[str.size() - 1] == ' ') str.pop_back();
}

void requestLine::setMethod(const std::string method)
{
    std::string a[] ={}
}

requestLine::requestLine(const std::string requestLine)
{
    std::string tmp = requestLine;
    trim(tmp);

    std::string method = tmp.substr(0, tmp.find(" "));
    std::string url = tmp.substr(tmp.find(" ") + 1, tmp.rfind(" ") - tmp.find(" ") - 1);
    std::string vers = tmp.substr(tmp.rfind(" ") + 1);

    std::cout << "[" << method << "]" << std::endl;
    std::cout << "[" << url << "]" << std::endl;
    std::cout << "[" << vers << "]" << std::endl;
}