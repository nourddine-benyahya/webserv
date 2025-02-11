#include "response.hpp"


void Response::matchRoute()
{
    // std::cout << req.getReqLine().getReqTarget() << std::endl;
    // std::map<std::string, Route>::iterator it = srv->routes.begin();
    // while (it != srv->routes.end())
    // {
    //     if (it->first == req.getReqLine().getReqTarget())
    //         matchedRoute = it->second;
    //     it++;
    // }
}
Response::Response(request r, Server::Config *server)
{
    req = r;
    srv = server;
    // matchRoute();
    if (req.getReqLine().getMethod() == GET)
    {
        get();
    }
    else if (req.getReqLine().getMethod() == POST)
    {
        post();
        std::cout << "post method" << std::endl;
    }
}

void Response::get()
{
	try {
		// have to check the permission of the request to throw ServerException with 403
		body = srv->getFile(req.getReqLine().getReqTarget());

		// need to add content-type in the case of images or anyother file type
		header = "HTTP/1.1 200 OK\r\nContent-Length: ";

		std::stringstream lengthStr;
			lengthStr << body.length();
		
		response = header + lengthStr.str() + "\r\n\r\n" + body;
	} catch (Server::ServerException &e) {
		body = this->srv->getErrorPage(e.getStatus());
        response = e.createHTTPErrorHeader(body.length()) + body;
		throw Server::ServerException(e.what(), response, e.getStatus());
	}
}
void Response::post()
{
    req.getReqBody().saveFile("data/");

    cgi c(req, "./data/cgi-files/page.php");

    c.setCgiEnv(".php", "cgi-bin/php-cgi");

    c.runCgi();


    // response = "HTTP/1.1 200 OK\r\n";
    response = c.getResponse();
}

void Response::DELETE()
{

}
void Response::methodFilter()
{
    // if ()
}

