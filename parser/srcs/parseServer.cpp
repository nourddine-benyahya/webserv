#include "parser.hpp"

bool isNumeric(std::string s)
{
    for (size_t i = 0; i < s.size(); i++)
    {
        if (!isdigit(s[i]))
            return false;
    }
    return true;
}

void parseBodyLimit(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Server::Config &srv)
{
    it++;
    if (it != end && it->token == equal)
        it++;
    else
        throw std::runtime_error("error with the config file");
    if (it == end || it->token != word || !isNumeric(it->value))
        throw std::runtime_error("error with the config file");
    srv.body_limit = std::atoi(it->value.c_str());
    
}

void parseErrorPages(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Server::Config &srv)
{
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
        it += 2;
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
            throw std::runtime_error("error in config file");
        if (it != end)
            it++;
    }
}

void parseServer(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end)
{
    Server::Config srv;
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
        it += 2;
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
            parseErrorPages(it, end, srv);
        else if (it->token == word && it->value == "route")
            parseRoute(it, end, srv);
        else if (it->token == word && it->value == "body_limit")
            parseBodyLimit(it, end, srv);
        else if (it->token == close_bracket)
            break;
        else
            throw std::runtime_error("unknown token in config file");
        if (it != end)
            it++;
    }
    srv.build();
}