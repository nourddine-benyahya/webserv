
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
    try
    {
        parseConfig();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}