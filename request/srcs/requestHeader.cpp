#include "requestHeader.hpp"

requestHeader::requestHeader()
{
    header = std::map<std::string, std::string>();
}

void requestHeader::setHeader(const std::string header)
{
    std::string key;
    std::string value;
    std::istringstream headerStream(header);
    while (std::getline(headerStream, key, ':') && std::getline(headerStream, value)) {
        this->header[key] = value;
    }
}