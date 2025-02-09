#include "parser.hpp"

std::vector<std::string> split(const std::string& str, const std::string& delimiters) {
    std::vector<std::string> result;
    std::string token;

    for (size_t i = 0; i < str.size(); ++i) {
        
        if (delimiters.find(str[i]) != std::string::npos) {
            if (!token.empty()) {  
                result.push_back(token);
                token.clear();
            }
        } else {
            token += str[i];
        }
    }
    if (!token.empty()) {
        result.push_back(token);
    }
    return result;
}

std::vector<tokens> Tokenizer(std::ifstream &file)
{
    std::string line;
    std::string content;

    while(std::getline(file, line))
    {
        if (line.empty())
            continue;
        if ((line[line.size() - 1] != '{' && line[line.size() - 1] != ',') || 
            (line[line.size() - 1] == ',' && line.find(',') != line.size() - 1) ||
            (line[line.size() - 1] == '{' && line.find('{') != line.size() - 1))
            {
                throw std::runtime_error("ConfigFile :Error in end of line");
            }
        
        content += line;
    }
    if (content.empty())
        throw std::runtime_error("ConfigFile: Error empty");
    std::vector<tokens> tks;
    std::vector<std::string> elems = split(content, " \n,");

    for (size_t i = 0; i < elems.size(); i++)
    {
        if (elems[i].size() == 1 && elems[i][0] == '{')
        {
            tokens t(open_bracket, elems[i]);
            tks.push_back(t);
        }
        else if (elems[i].size() == 1 && elems[i][0] == '}')
        {
            tokens t(close_bracket, elems[i]);
            tks.push_back(t);
        }
        else if (elems[i].size() == 1 && elems[i][0] == ':')
        {
            tokens t(equal, elems[i]);
            tks.push_back(t);
        }
        else if (elems[i].size() >= 1)
        {
            tokens t(word, elems[i]);
            tks.push_back(t);
        }
        else
            std::cout << "error token not found" << std::endl;
    }
    return tks;
}