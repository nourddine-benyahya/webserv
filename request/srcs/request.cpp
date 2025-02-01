#include "request.hpp"

void request::printRequestLine()
{
    std::cout << std::endl << "-------------------------------------------request Line---------------------------------" << std::endl;
    std::cout << "Method: ~" << reqLine.getMethod() << "~" << std::endl;
    std::cout << "Request Target: ~" << reqLine.getReqTarget() << "~" << std::endl;
    std::cout << "HTTP Version: ~" << reqLine.getHttpVers() << "~"  << std::endl;
    std::cout << "Params: " << std::endl;
    std::map<std::string, std::string> params = reqLine.getParams();
    for (std::map<std::string, std::string>::iterator it = params.begin(); it != params.end(); it++) {
        std::cout << "~" << it->first << "~   :   ~" << it->second << "~" << std::endl;
    }
}

void request::printRequestHeader()
{
    std::cout << std::endl << "-------------------------------------------request header-------------------------------" << std::endl;
    std::map<std::string, std::string> header = reqHeader.getHeader();
    std::cout << "key   " << "  value" << std::endl;
    for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); it++) {
        std::cout << "~" << it->first << "~ : ~" << it->second << "~" << std::endl;
    }
}

void request::printRequestBody()
{
    std::cout << std::endl << "-------------------------------------------request body---------------------------------" << std::endl;
    std::cout  << std::endl << "----------body type----------- " << std::endl;
    if (reqBody.getType() == NONE)
        std::cout << "NONE" << std::endl;
    else if (reqBody.getType() == TEXT)
        std::cout << "TEXT" << std::endl;
    else if (reqBody.getType() == XML)
        std::cout << "XML" << std::endl;
    else if (reqBody.getType() == HTML)
        std::cout << "HTML" << std::endl;
    else if (reqBody.getType() == JSON)
        std::cout << "JSON" << std::endl;
    else if (reqBody.getType() == JAVA_SCRIPT)
        std::cout << "JAVA_SCRIPT" << std::endl;
    else if (reqBody.getType() == OCTET_STREAM)
        std::cout << "OCTET_STREAM" << std::endl;
    else if (reqBody.getType() == URLENCODED)
        std::cout << "URLENCODED" << std::endl;
    else if (reqBody.getType() == FORM_DATA)
        std::cout << "FORM_DATA" << std::endl;
    else if (reqBody.getType() == TEXT)
        std::cout << "TEXT" << std::endl;
    else
        std::cout << "Type: UNKNOWN" << std::endl;

    std::cout << std::endl << "--------------body formFields-------------" << std::endl;
    std::map<std::string, std::string> reqBodyd = reqBody.getFormFields();
    for (std::map<std::string, std::string>::iterator it = reqBodyd.begin(); it != reqBodyd.end(); it++) {
        std::cout << "~" << it->first << "~   :   ~" << it->second << "~" << std::endl;
    }
    std::cout << std::endl << "-------------body data----------" << std::endl;
    int i;
    std::cout  << "~";
    std::vector<char> datae = reqBody.getFileBuffer();
    for (i = 0; i < datae.size(); i++)
    {
        std::cout << datae[i] ;
    }
    std::cout << "~" << std::endl;
}

void request::printFullRequest(const std::string request)
{
    std::cout << std::endl << "-------------------------------------------start request---------------------------------" << std::endl;
    std::cout << request << std::endl;
    std::cout << std::endl << "-------------------------------------------end request---------------------------------" << std::endl;
}


void saveFile(const std::string &fileName, const std::vector<char> &fileBuffer)
{
    // Step 1: Open a file with the given fileName in write mode
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


request::request(const std::string request) {
    std::string line;

    // Put the request in a stream and read it line by line
    std::istringstream requestStream(request);


    try {

        // Read the request line
        std::getline(requestStream, line);
        reqLine = requestLine(line);

        // Read the headers
        // reqHeader = requestHeader();
        while (std::getline(requestStream, line) && !line.empty() && line != "\r") {
            trim(line);
            reqHeader.setHeader(line);
        }

        // Read the body
        reqBody = requestBody(requestStream, reqHeader);
        

        //print full request
        printFullRequest(request);

        //print request Line
        printRequestLine();

        //print request header
        printRequestHeader();

        //print request body
        // printRequestBody();

        //save file if it is a file
        if (reqBody.getType() == FORM_DATA && reqBody.getFormFields().find("filename") != reqBody.getFormFields().end())
            reqBody.saveFile();


    } catch(const char *e) {
        std::cerr << e << '\n';
    }
}
