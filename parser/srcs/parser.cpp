
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

void parseConfig(std::string path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        throw Server::ServerException("Config file not valid:" + path);
    }
    std::vector<tokens> tk = Tokenizer(file);
    parser(tk);
}
