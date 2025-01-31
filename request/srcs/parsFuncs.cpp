#include "parsFuncs.hpp"

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