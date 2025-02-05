
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
bool isNumeric(std::string s)
{
    for (size_t i = 0; i < s.size(); i++)
    {
        if (!isdigit(s[i]))
            return false;
    }
    return true;
}
void parseErrorPages(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Server::Config &srv)
{
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
    {
        std::cerr << "here" << std::endl;
        it += 2;
    }
    while(it != end && it->token != close_bracket)
    {
        if (it->token == word && isNumeric(it->value))
        {
            int status = std::atoi(it->value.c_str());
            if (it + 1 != end && (it + 1)->token == equal && it + 2 != end && (it + 2)->token == word)
            {
                it++;
                srv.setErrorPage(status, (++it)->value);
            }
        }
        else
        {
            std::cerr << "error with the server " << srv.getName() << " " << isNumeric(it->value)  << it->value<< std::endl;
        }
        if (it != end)
            it++;
    }
    std::cerr << "kharej" << std::endl;
}
void parseServer(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end)
{
    Server::Config srv;
    bool found = false;
    while(it != end)
    {
        if (it->token == word && it->value == "port")
        {
            if (it + 1 != end && (it + 1)->token == equal && it + 2 != end && (it + 2)->token == word)
            {
                it++;
                srv.setPort(std::atoi((++it)->value.c_str()));
            }
        }
        else if (it->token == word && it->value == "index")
        {
            if (it + 1 != end && (it + 1)->token == equal && it + 2 != end && (it + 2)->token == word)
            {
                it++;
                srv.setIndex((++it)->value);
            }
        }
        else if (it->token == word && it->value == "root")
        {
            if (it + 1 != end && (it + 1)->token == equal && it + 2 != end && (it + 2)->token == word)
            {
                it++;
                srv.setRoot((++it)->value);
            }
        }
        else if (it->token == word && it->value == "name")
        {
            if (it + 1 != end && (it + 1)->token == equal && it + 2 != end && (it + 2)->token == word)
            {
                it++;
                srv.setName((++it)->value);
            }
        }
        else if (it->token == word && it->value == "error")
        {
            parseErrorPages(it, end, srv);
        }
        else if (it->token == close_bracket)
        {
            found = true;
            break;
        }
        if (it != end)
        {
            std::cout << "LOGS : " << it->token << " " << it->value << std::endl;
            it++;
            // std::cerr << "tokhalef end" << std::endl;
        }
    }
    if (!found)
        std::cerr << "error a '{' not found" << std::endl;

    srv.build();
}
void parser(std::vector<tokens> &tk)
{
    std::vector<tokens>::iterator end = tk.end();
    std::vector<tokens>::iterator it = tk.begin();
    while (it != end)
    {
        if (it->token == word)
        {
            if (it->value == "server")
            {
                parseServer(it, end);
            }
        }
        std::cout << "server just parsed" << std::endl;
        if (it != end)
        {
            std::cerr << "not end" << std::endl;
            it++;
        }
    }
    
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
}

int main()
{
    parseConfig();
}