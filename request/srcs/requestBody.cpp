#include "requestBody.hpp"


std::string trim(const std::string &str) {

    //trim from spaces and double qoutes
    size_t first = str.find_first_not_of(" \t\n\r\"");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r\"");
    return str.substr(first, (last - first + 1));
}

void requestBody::setType(const std::string type)
{
    if (type.find("text") != std::string::npos)
        this->type = TEXT;
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
            std::string key = trim(part.substr(0, pos));
            std::string value = trim(part.substr(pos + 1));

            formFields[key] = value;
        }
    }
}

void requestBody::save_formfield(std::istringstream &stream, size_t &read)
{
    std::string line;
    // std::string boundary;
    //read the boundary
    // std::getline(stream, line);
    // read += line.size();
    // boundary = line;
    //read the fist line ,its contain line Content-Disposition: form-data; name="testkey"; filename="test"
    std::getline(stream, line);
    read += line.size();
    parseContentDisposition(line);
    std::getline(stream, line);
    read += line.size();
    std::getline(stream, line);
    read += line.size();
    parseContentDisposition(line);
    for (std::map<std::string, std::string>::iterator it = formFields.begin(); it != formFields.end(); it++) {
        std::cout << "##" << it->first << "##" << it->second << "##" << std::endl;
    }
}

requestBody::requestBody(std::istringstream &stream, requestHeader header)
{
    size_t read = 1;
    std::string boundary;
    std::map<std::string, std::string> headerMap = header.getHeader();

    if (headerMap.find("Content-Type") != headerMap.end())
        setType(headerMap["Content-Type"]);

    if (getType() == NONE)
        return;

    if (getType() == FORM_DATA) {
        std::getline(stream, boundary);
        std::cout << "boundary : *" << boundary << "*" << std::endl;
        read += boundary.size();
        save_formfield(stream, read);
    }
    // Parse Content-Length safely
    if (headerMap.find("Content-Length") != headerMap.end()) {
        const char* cl_str = headerMap["Content-Length"].c_str();
        char* end;
        long content_length = strtol(cl_str, &end, 10);
        if (*end != '\0' || content_length < 0)
            return; // Invalid length
        content_length -= read;


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
            fileBuffer.pop_back();
        }

    }
}

std::vector<char> &requestBody::getData()
{
    return fileBuffer;
}
