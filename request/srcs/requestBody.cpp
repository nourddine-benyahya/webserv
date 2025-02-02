#include "requestBody.hpp"

void requestBody::parseContentDisposition(const std::string &line)
{
    std::istringstream stream(line);
    std::string part;

    while (std::getline(stream, part, ';')) {
        size_t pos = part.find('=');
        if (pos == std::string::npos) {
            pos = part.find(':');
        }
        if (pos != std::string::npos) {
            std::string key = part.substr(0, pos);
            std::string value = part.substr(pos + 1);
            trim(key);
            trim(value);
            if (value[0] == '"')
            {
                value.erase(0, 1);
                value.erase(value.size() - 1);
            }
            formFields[key] = value;
        }
    }
    if (line.find("Content-Type") != std::string::npos)
    {
        size_t pos = line.find(':');
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        trim(key);
        trim(value);
        formFields[key] = value;
    }
}

void requestBody::save_formfield(std::istringstream &stream)
{

    std::string line;
    std::getline(stream, line);
    parseContentDisposition(line);
    std::getline(stream, line);
    if (line.find("Content-Type") != std::string::npos)
    {
        parseContentDisposition(line);

        std::getline(stream, line);
    }
}

void requestBody::saveFile()
{
    // Step 1: Open a file with the given fileName in write mode
    std::string fileName = DATA_DIR + formFields["filename"];
    std::ofstream outFile(fileName, std::ios::binary);

    // Step 2: Check if the file is open
    if (outFile.is_open())
    {
        // Step 3: Write the contents of fileBuffer to the file
        outFile.write(fileBuffer.data(), fileBuffer.size());

        // Step 4: Close the file
        outFile.close();
    }
    else
    {
        // Handle the error if the file cannot be opened
        throw "Failed to open the file.";
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


dataType requestBody::getType()
{
    return type;
}

std::string requestBody::getFilePath()
{
    return filePath;
}

std::vector<char> &requestBody::getFileBuffer()
{
    return fileBuffer;
}

std::map<std::string, std::string> &requestBody::getFormFields()
{
    return formFields;
}


requestBody::requestBody(std::istringstream &stream, requestHeader header)
{
    // size_t read = 0;
    std::string boundary;
    std::map<std::string, std::string> headerMap = header.getHeader();

    if (headerMap.find("Content-Type") != headerMap.end())
        setType(headerMap["Content-Type"]);
    else
        this->type = NONE;

    if (getType() == NONE)
        return;

    if (getType() == FORM_DATA) {
        std::getline(stream, boundary);
        save_formfield(stream);
    }
    // Parse Content-Length safely
    if (headerMap.find("Content-Length") != headerMap.end()) {
        const char* cl_str = headerMap["Content-Length"].c_str();
        char* end;
        long content_length = strtol(cl_str, &end, 10);
        if (*end != '\0' || content_length < 0)
            return; // Invalid length


        fileBuffer.resize(content_length);
        stream.read(&fileBuffer[0], content_length);
        // Resize if fewer bytes were read
        if (stream.gcount() < content_length) {
            fileBuffer.resize(stream.gcount());
        }
        if (getType() == FORM_DATA)
        {
            size_t boundary_size = boundary.length();

            fileBuffer.pop_back();
            fileBuffer.pop_back();
            fileBuffer.pop_back();
            while (boundary_size > 0)
            {
                fileBuffer.pop_back();
                boundary_size--;
            }
            fileBuffer.pop_back();
            fileBuffer.pop_back();
        }
    }
}


