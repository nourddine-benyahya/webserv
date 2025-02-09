#include "requestBody.hpp"


// Helper function to save file content
static void saveFile(const std::string& filePath, const std::vector<char>& data)
{
    std::ofstream outFile(filePath.c_str(), std::ios::binary);
    if (outFile)
    {
        if (!data.empty())
            outFile.write(&data[0], data.size());
    }
}

void requestBody::setType(const std::string type)
{
    if (type.find("text") != std::string::npos)
        this->type = TEXT;
    else if (type.find("xml") != std::string::npos)
        this->type = XML;
    else if (type.find("html") != std::string::npos)
        this->type = HTML;
    else if (type.find("json") != std::string::npos)
        this->type = JSON;
    else if (type.find("javascript") != std::string::npos)
        this->type = JAVA_SCRIPT;
    else if (type.find("octet-stream") != std::string::npos)
        this->type = OCTET_STREAM;
    else if (type.find("urlencoded") != std::string::npos)
        this->type = URLENCODED;
    else if (type.find("form-data") != std::string::npos)
        this->type = FORM_DATA;
    else
        this->type = NONE;
}

std::string &requestBody::getFullBody()
{
    return fullBody;
}

void requestBody::parsBodyPart(std::string bodyPart)
{
    //data that body part contains
    formData data;
    std::string filePath;
    dataType type;
    std::vector<char> fileBuffer;
    std::map<std::string, std::string> formFields;

    std::istringstream stream(bodyPart);
    std::string line;

    //parse first line
    std::getline(stream, line);
    line.pop_back();
    line.push_back(';');
    int pos = 0;
    while ((pos = line.find(";")) != std::string::npos)
    {
        std::string tmp = line.substr(0, pos);
        if (line.find(":") != std::string::npos)
        {
            std::string key = tmp.substr(0, tmp.find(":"));
            std::string value = tmp.substr(tmp.find(":") + 2, tmp.length() - tmp.find(":") - 2);
            formFields[trim(key)] = trim(value);
        }
        else
        {
            std::string key = tmp.substr(0, tmp.find("="));
            std::string value = tmp.substr(tmp.find("=") + 2, tmp.length() - tmp.find("=") - 3);
            if (value[value.length() - 1] == '"' && value[0] == '"')
            {
                value.pop_back();
                value.erase(0, 1);
            }
            formFields[trim(key)] = trim(value);
        }
        line.erase(0, pos + 1);
    }

    //parse second line
    std::getline(stream, line);
    if (line.find("Content-Type") == std::string::npos)
        type = TEXT;
    else
    {
        type = OCTET_STREAM;
        std::getline(stream, line);
        //read all remiding lines with out split it with new line just read it as it is
        std::istreambuf_iterator<char> begin(stream), end;
        fileBuffer.assign(begin, end);

    }
    data.type = type;
    data.formFields = formFields;
    data.fileBuffer = fileBuffer;

    if (data.type == OCTET_STREAM)
    {
        std::string fileName = data.formFields["filename"];
        std::string filePath = DATA_DIR + fileName;
        saveFile(filePath, data.fileBuffer); // Use helper function
        data.filePath = filePath;
    }
    body.push_back(data);
}

requestBody::requestBody(std::istringstream &stream, requestHeader header)
{
    setType(header.getHeader()["Content-Type"]);
    if (type == NONE)
        return;
    
    fullBody = stream.str();
    fullBody = fullBody.substr(fullBody.find("\r\n\r\n") + 4);

    //save boundary
    std::string boundary = header.getHeader()["Content-Type"];
    boundary = boundary.substr(boundary.find("boundary=") + 9);

    std::string tmp = fullBody;
    std::vector<std::string> bodyParts;

    //split body into parts
    size_t pos = 0;
    pos = tmp.find(boundary);
    tmp.erase(0, pos + boundary.length() + 2);
    while ((pos = tmp.find(boundary)) != std::string::npos)
    {
        bodyParts.push_back(tmp.substr(0, pos - 4));
        tmp.erase(0, pos + boundary.length() + 2);
    }
    for (size_t i = 0; i < bodyParts.size(); i++)
    {
        parsBodyPart(bodyParts[i]);
    }
}


