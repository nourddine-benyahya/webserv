#pragma once

#include "requestHeader.hpp"
#include "parsFuncs.hpp"

struct formData
{
    dataType type;
    std::string filePath;
    std::vector<char> fileBuffer;
    std::map<std::string, std::string> formFields;
};

class requestBody
{
    private :
        std::string fullBody;
        dataType type;
        std::vector<formData> body;
        // void save_formfield(std::istringstream &stream);
        // void parseContentDisposition(const std::string &line);
        void setType(const std::string type);
        void parsBodyPart(std::string bodyPart);

    public :
        //constructors
        requestBody(){};
        requestBody(std::istringstream &stream, requestHeader header);
        //getters
        std::string &getFullBody();
        void saveFile(const std::string& filePath);
        // dataType &getType();
        // std::string &getFilePath();
        // std::vector<char> &getFileBuffer();
        // std::map<std::string, std::string> &getFormFields();
        // void saveFile();
};