
#include "parser.hpp"

void parser(std::vector<tokens> &tk)
{
    std::vector<tokens>::iterator end = tk.end();
    std::vector<tokens>::iterator it = tk.begin();
    while (it != end)
    {
        if (it->token == word)
        {
            if (it->value == "server")
                parseServer(it, end);
        }
        if (it != end)
            it++;
    }
}
bool checkLastFourChars(std::string str,const std::string suffix) {
    if (str.size() < 6) {
        return false;
    }
    return str.substr(str.size() - 5) == suffix;
}

void parseConfig(std::string path)
{
    std::ifstream file(path.c_str());

    if (!file.is_open() || !checkLastFourChars(path, ".conf"))
    {
        throw Server::ServerException("Config file not valid:" + path);
    }
    std::vector<tokens> tk = Tokenizer(file);
    parser(tk);
}
