#pragma once 


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Server.hpp"

class Route
{
    public :
        Route() {};
        ~Route() {};
        std::string path;
        bool upload;
        std::map<std::string, std::string>cgis;
        std::vector<std::string>allowedMethods;
        std::string root;
        std::string index;
        bool list_dirs;
};