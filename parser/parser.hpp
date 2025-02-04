#pragma once

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