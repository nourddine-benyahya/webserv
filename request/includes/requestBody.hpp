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
        void setType(const std::string type);
        void parsBodyPart(std::string bodyPart);

    public :
        //constructors
        requestBody(){};
        requestBody(std::istringstream &stream, requestHeader header);
        //getters
        std::string &getFullBody();
        // save content of the file
        void saveFile(const std::string& filePath);
};