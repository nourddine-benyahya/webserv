#pragma once

#include "requestHeader.hpp"
#include "parsFuncs.hpp"

class requestBody
{
    private :
        dataType type;
        std::string filePath;
        std::vector<char> fileBuffer;
        std::map<std::string, std::string> formFields;
        //private method that help in pars the body
        void save_formfield(std::istringstream &stream, size_t &read);
        void parseContentDisposition(const std::string &line);
        void setType(const std::string type);
    public :
        //constructors
        requestBody(){};
        requestBody(std::istringstream &stream, requestHeader header);
        //getters
        dataType getType();
        std::string getFilePath();
        std::vector<char> &getFileBuffer();
        std::map<std::string, std::string> &getFormFields();
        //save the file if it is a file
        void saveFile();
};