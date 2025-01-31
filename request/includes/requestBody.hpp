#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include "requestHeader.hpp"
#include "requestLine.hpp"

class requestHeader;

enum dataType
{
    TEXT,
    XML,
    HTML,
    JSON,
    JAVA_SCRIPT,
    OCTET_STREAM,
    URLENCODED,
    FORM_DATA,
    NONE,
};

class requestBody
{
    private :
        dataType type;
        std::string fileName;
        std::vector<char> fileBuffer;
    public :
        std::map<std::string, std::string> formFields;
        requestBody(){};
        requestBody(std::istringstream &stream, requestHeader header);
        std::vector<char> &getData();
        void setType(const std::string type);
        dataType getType();
        void parseContentDisposition(const std::string &line);
        void save_formfield(std::istringstream &stream, size_t &read);
};