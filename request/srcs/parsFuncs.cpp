#include "parsFuncs.hpp"
#include "exeptions.hpp"

bool fileExists(const char *path)
{
    return (access(path, F_OK) == 0 && access(path, X_OK) == 0);
}

std::string trim(std::string& str)
{
   std::string whiteSpaces(" \t\f\v\n\r");

   while(whiteSpaces.find(str[0]) != std::string::npos) str.erase(str.begin());
   while(whiteSpaces.find(str[str.size() - 1]) != std::string::npos) str.pop_back();
   return (str);
}

void toupper(std::string& str)
{
    for (size_t i = 0; i < str.size(); i++)
        str[i] = std::toupper(str[i]);
}

void urlFormat(std::string& url)
{
    const std::string allowed_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "abcdefghijklmnopqrstuvwxyz"
                                      "0123456789"
                                      "-._~:/?#[]@!$&'()*+,;=";

    //check if Request uri contain more that 2048 chars
    if (url.empty() || url[0] != '/' || url.length() > 2048)
        throw exeptions(414, "414 Request-URI Too Long");

    std::cout << "---" << url << "------------" << std::endl;
    for (std::string::const_iterator it = url.begin(); it != url.end(); ++it)
    {
        if (allowed_chars.find(*it) == std::string::npos)
            throw exeptions(400, "400 bad Request 1");
    }

    size_t query_pos = url.find('?');
    size_t fragment_pos = url.find('#');

    if (fragment_pos != std::string::npos && query_pos != std::string::npos && fragment_pos < query_pos)
            throw exeptions(400, "400 bad Request 2");


    if (query_pos != std::string::npos && fragment_pos != std::string::npos && fragment_pos < query_pos)
            throw exeptions(400, "400 bad Request 3");


    if (query_pos != std::string::npos && query_pos == url.length() - 1)
            throw exeptions(400, "400 bad Request 4");


    if (fragment_pos != std::string::npos && fragment_pos == url.length() - 1)
            throw exeptions(400, "400 bad Request 5");
}