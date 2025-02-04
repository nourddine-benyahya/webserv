#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
enum TOKENS
{
    word,
    open_bracket = '{',
    close_bracket = '}',
    equal = ':'
};

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
class tokens
{
    public :
        tokens(TOKENS token, std::string value)
        {
            this->token = token;
            this->value = value;
        }
        TOKENS token;
        std::string value;
};

std::vector<tokens> Tokenizer(std::ifstream &file)
{
    std::string line;
    std::string content;

    while(std::getline(file, line))
    {
        content += line;

    }
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
void parser(std::vector<tokens> tk)
{
    
}
void parseConfig()
{
    std::ifstream file("default.conf");
    if (!file.is_open())
    {
        std::cerr << "error with the file" << std::endl;
        return;
    }
    std::vector<tokens> tk = Tokenizer(file);
    parser(tk);
    // for (std::vector<tokens>::iterator it = tk.begin(); it != tk.end(); it++)
    // {
    //     std::cout << "token => |" << it->token << "| value => |" << it->value << "|" << std::endl; 
    // }
}

int main()
{
    parseConfig();
}