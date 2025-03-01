#include "requestHeader.hpp"

requestHeader::requestHeader()
{
    header = std::map<std::string, std::string>();
}

void requestHeader::setHeader(const std::string Line)
{
    std::string key = Line.substr(0, Line.find(":"));
    std::string value = Line.substr(Line.find(":") + 2, Line.size() - 1);
    header[key] = value;
}

std::map<std::string, std::string> &requestHeader::getHeader()
{
    return header;
}