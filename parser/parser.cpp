
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
        if (line[line.size() - 1] != '{' && line[line.size() - 1] != ',')
            throw std::runtime_error("error in end of line");
        content += line;
    }
    if (content.empty())
        throw std::runtime_error("config file is empty");
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
    // std::cerr << "kharej" << std::endl;
}
std::string setPathUpload(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Server::Config &srv)
{


    std::string s;
    if (it + 1 != end && (it + 1)->token == equal)
    {
        it++;
    }
    else
        return s;
    if (it + 1 != end && (it + 1)->token == word)
    {
        it++;
    }
    else
    {
        return s;
    }
    if (it->token == word)
        return it->value;
    return s;
}

void parseAllowedMethods(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Route &route)
{
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
    {
        it += 2;
    }
    else
        throw std::runtime_error("error with config file");
    while(it != end && it->token != close_bracket)
    {
        if(it->token == word && (it->value == "POST" || it->value == "GET" || it->value == "DELETE"))
            route.allowedMethods.push_back(it->value);
        else
        {
            throw std::runtime_error("method not valid");
        }
        it++;
    }
}

void parseCgi(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Route &route)
{
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
    {
        it += 2;
    }
    else
        throw std::runtime_error("error with config file 1");
    while (it != end && it->token != close_bracket)
    {
        std::vector<std::string>v = split(it->value, "=");
        if (v.size() != 2)
            throw std::runtime_error("error with the config file");
        if (route.cgis.find(v[0]) != route.cgis.end())
            throw std::runtime_error("duplicated values in cgi");
        route.cgis[v[0]] = v[1];
        it++;
    }
}
void parseRoute(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Server::Config &srv)
{
    Route route;
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
    {
        it += 2;
    }
    while (it != end && it->token != close_bracket)
    {
        if (it->token == word && it->value == "path")
        {
            route.path = setPathUpload(it, end, srv);
            if (route.path.empty())
                std::cout << "error with the config file";
        }
        else if (it->token == word && it->value == "upload")
        {
            std::string res = setPathUpload(it, end, srv);
            if (res.empty() || (res != "yes" && res != "no"))
                throw std::runtime_error("error with upload");
            if (res == "yes")
                route.upload = true;
            else
                route.upload = false;
        }
        else if (it->token == word && it->value == "cgi")
        {
            parseCgi(it, end, route);
        }
        else if (it->token == word && it->value == "allowed_mthods")
        {
            parseAllowedMethods(it, end, route);
        }
        else if (it->token == word && it->value == "root")
        {
            route.root = setPathUpload(it, end, srv);
            if (route.root.empty())
                std::runtime_error("error with the config file");
        }
        else if (it->token == word && it->value == "index")
        {
            route.index = setPathUpload(it, end, srv);
            if (route.index.empty())
                std::runtime_error("error with the config file");
        }
        else if (it->token == word && it->value == "list_dirs")
        {
            std::string res = setPathUpload(it, end, srv);
            if (res.empty() || (res != "yes" && res != "no"))
                throw std::runtime_error("error with upload");
            if (res == "yes")
                route.list_dirs = true;
            else
                route.list_dirs = false;
        }
        if (it != end)
            it++;
    }
    if (route.path.empty())
        throw std::runtime_error("no path specified for location");
    if (srv.routes.find(route.path) != srv.routes.end())
        throw std::runtime_error("duplicated paths for location");
    srv.routes[route.path] = route;
}
void parseBodyLimit(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Server::Config &srv)
{
    it++;
    if (it != end && it->token == equal)
    {
        it++;
    }
    else
        throw std::runtime_error("error with the config file");
    if (it == end || it->token != word || !isNumeric(it->value))
        throw std::runtime_error("error with the config file");
    srv.body_limit = std::atoi(it->value.c_str());
    
}

void parseServer(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end)
{
    Server::Config srv;
    bool found = false;
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
    {
        it += 2;
    }
    while(it != end && it->token != close_bracket)
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
        else if (it->token == word && it->value == "route")
        {
            parseRoute(it, end, srv);
        }
        else if (it->token == word && it->value == "body_limit")
        {
            parseBodyLimit(it, end, srv);
        }
        else if (it->token == close_bracket)
        {
            found = true;
            break;
        }
        else
            throw std::runtime_error("unknown token in config file");
        if (it != end)
        {
            it++;
        }
    }

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
        if (it != end)
        {
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

// int main()
// {
//     try
//     {
//         parseConfig();
//     }
//     catch(std::exception &e)
//     {
//         std::cerr << e.what() << std::endl;
//     }
// }