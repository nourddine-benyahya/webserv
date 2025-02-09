#pragma once

#include "Server.hpp"
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
enum TOKENS
{
    word,
    open_bracket = '{',
    close_bracket = '}',
    equal = ':'
};
class tokens
{
    public :
        tokens(TOKENS token, std::string value)
        {
            this->token = token;
            this->value = value;
        }
        TOKENS token;
        std::string value;
};
void parseConfig();
std::vector<tokens> Tokenizer(std::ifstream &file);
std::vector<std::string> split(const std::string& str, const std::string& delimiters);
// void parseRoute(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Server::Config &srv);
void parseServer(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end);
