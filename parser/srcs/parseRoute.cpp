#include "parser.hpp"

void parseCgi(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Route &route)
{
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
        it += 2;
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

void parseAllowedMethods(std::vector<tokens>::iterator &it, std::vector<tokens>::iterator &end, Route &route)
{
    it++;
    if (it != end && (it)->token == equal && it + 1 != end && (it + 1)->token == open_bracket)
        it += 2;
    else
        throw std::runtime_error("error with config file");
    while(it != end && it->token != close_bracket)
    {
        if(it->token == word && (it->value == "POST" || it->value == "GET" || it->value == "DELETE"))
            route.allowedMethods.push_back(it->value);
        else
            throw std::runtime_error("method not valid");
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
                throw std::runtime_error("error with upload");
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
        else
            throw std::runtime_error("syntaxe error in config file");
        if (it != end)
            it++;
    }
    if (route.path.empty())
        throw std::runtime_error("no path specified for location");
    if (srv.routes.find(route.path) != srv.routes.end())
        throw std::runtime_error("duplicated paths for location");
    srv.routes[route.path] = route;
}