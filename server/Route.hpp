#pragma once 


#include <iostream>
#include <string>
#include <vector>
#include <map>
class Route
{
    std::string path;
    bool upload;
    std::map<std::string ext, std::string path>cgis;
    std::vector<std::string>allowedMethods;
};