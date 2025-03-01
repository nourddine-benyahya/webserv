#pragma once

#include <string>

class exeptions
{
    private :
        int status;
        std::string msg;
        exeptions(){};
    public :
        exeptions(int status, std::string msg) : status(status), msg(msg) {};
        int &getStatus() {return status;};
        std::string  &getMsg() {return msg;};
};