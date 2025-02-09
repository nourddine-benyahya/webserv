#include "response.hpp"

Response::Response(request r, Server::Config *srv)
{
    req = r;
    this->srv = srv;
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

    cgi c(req);

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

