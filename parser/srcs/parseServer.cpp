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
        throw std::runtime_error("ConfigFile :Error with syntax 1");
    if (it == end || it->token != word || !isNumeric(it->value))
        throw std::runtime_error("ConfigFile :Error with syntax 2");
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
            throw std::runtime_error("ConfigFile :Error with syntax 3");
        if (it != end)
            it++;
    }
}
void parseCgi(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Route &route)
{
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
        it += 2;
    else
        throw std::runtime_error("ConfigFile :Error with syntax 4");
    while (it != end && it->token != close_bracket)
    {
        std::vector<std::string>v = split(it->value, "=");
        if (v.size() != 2)
            throw std::runtime_error("ConfigFile :Error with syntax 5");
        if (route.cgis.find(v[0]) != route.cgis.end())
            throw std::runtime_error("ConfigFile :duplicated values in cgi");
        route.cgis[v[0]] = v[1];
        it++;
    }
}

void parseAllowedMethods(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Route &route)
{
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
        it += 2;
    else
        throw std::runtime_error("ConfigFile :Error with syntax 6");
    while(it != end && it->token != close_bracket)
    {
        if(it->token == word && (it->value == "POST" || it->value == "GET" || it->value == "DELETE"))
            route.allowedMethods.push_back(it->value);
        else
            throw std::runtime_error("ConfigFile: Method not valid");
        it++;
    }
}

std::string setPathUpload(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Server::Config &srv)
{
    std::string s;
    if (it + 1 != end && (it + 1)->token == equal)
        it++;
    else
        return s;
    if (it + 1 != end && (it + 1)->token == word)
        it++;
    else
        return s;
    if (it->token == word)
        return it->value;
    return s;
}
void parseRoute(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Server::Config &srv)
{
    Route route;
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
        it += 2;
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
                throw std::runtime_error("ConfigFile: Error with upload");
            if (res == "yes")
                route.upload = true;
            else
                route.upload = false;
        }
        else if (it->token == word && it->value == "cgi")
            parseCgi(it, end, route);
        else if (it->token == word && it->value == "allowed_mthods")
            parseAllowedMethods(it, end, route);
        else if (it->token == word && it->value == "root")
        {
            route.root = setPathUpload(it, end, srv);
            if (route.root.empty())
                std::runtime_error("ConfigFile :Error with syntax 7");
        }
        else if (it->token == word && it->value == "index")
        {
            route.index = setPathUpload(it, end, srv);
            if (route.index.empty())
                std::runtime_error("ConfigFile :Error with syntax 8");
        }
        else if (it->token == word && it->value == "list_dirs")
        {
            std::string res = setPathUpload(it, end, srv);
            if (res.empty() || (res != "yes" && res != "no"))
                throw std::runtime_error("ConfigFile: Error with upload");
            if (res == "yes")
                route.list_dirs = true;
            else
                route.list_dirs = false;
        }
        else
            throw std::runtime_error("ConfigFile :Error with syntax 9");
        if (it != end)
            it++;
    }
    if (route.path.empty())
        throw std::runtime_error("ConfigFile: no path specified for location");
    if (srv.routes.find(route.path) != srv.routes.end())
        throw std::runtime_error("ConfigFile: duplicated paths for location");
    srv.routes[route.path] = route;
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
                if (!isNumeric((++it)->value.c_str()))
                    throw std::runtime_error("ConfigFile :Error with syntax 10");
                srv.setPort(std::atoi((it)->value.c_str()));
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
            throw std::runtime_error("ConfigFile :Error with syntax 11");
        if (it != end)
            it++;
    }
    
    srv.build();
}