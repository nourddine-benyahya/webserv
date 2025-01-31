#pragma once

#include "requestHeader.hpp"
#include "parsFuncs.hpp"

class requestBody
{
    private :
        dataType type;
    public :
        std::vector<char> fileBuffer;
        std::map<std::string, std::string> formFields;
        requestBody(){};
        requestBody(std::istringstream &stream, requestHeader header);
        std::vector<char> &getData();
        void setType(const std::string type);
        dataType getType();
        void parseContentDisposition(const std::string &line);
        void save_formfield(std::istringstream &stream, size_t &read);
};