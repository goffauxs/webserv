#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include "header.hpp"
#include "request.hpp"
#include "utils.hpp"
#include "webserv.hpp"

std::string	request_get(Request const &req)
{
	std::string	path = "server";
	std::string	action;

	if (req.get_resource().find("?") != (size_t)-1)
	{
		action = req.get_resource().substr(0, req.get_resource().find("?") - 1);
		std::string	res = exec_cgi(path + "/cgi-bin/test.py", req);
	}
	else
		action = req.get_resource();
	std::ifstream	ifs(path + action);
	if (ifs)
	{
		std::ostringstream	stream;
		stream << ifs.rdbuf();
		std::string	body = stream.str();

		std::vector<Header> headers = req.get_headers();
		std::string	accept;
		for (std::vector<Header>::iterator it = headers.begin(); it != headers.end(); it++)
			if (it->get_key() == "Accept")
			{
				accept = it->get_value();
				break ;
			}
		size_t	start = 0;
		size_t	end = std::min(accept.find(",", start), accept.find("\n", start));
		return ("HTTP/1.1 200 OK\nContent-Type: " + accept.substr(start, end - start)
		+ "\nContent-Length: " + std::to_string(body.length()) + "\n\n" + body);
	}
	else
	{
		std::ifstream	errFile("errors/404.html");
		std::ostringstream	stream;
		stream << errFile.rdbuf();
		std::string	body = stream.str();

		return ("HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(body.length()) + "\n\n" + body);
	}
}

std::string	request_delete(Request const &req)
{
	std::string	path = "server";

	if (!remove((path + req.get_resource()).c_str()))
	{
		return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nFile deleted");
	}
	else
	{
		std::ifstream	errFile("errors/404.html");
		std::ostringstream	stream;
		stream << errFile.rdbuf();
		std::string	body = stream.str();

		return ("HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(body.length()) + "\n\n" + body);
	}
}

std::string	request_post(Request const &req)
{
	std::string	buffString = req.get_request();
	size_t	posBound = buffString.find("boundary=") + 9;
	std::string	boundary = buffString.substr(posBound, buffString.find("\r", posBound) - posBound);
	std::cout << "bound = "<< boundary << std::endl;

	std::string	secBound = boundary;
	secBound.insert(0, "--");
	size_t	posSecBound = buffString.find(secBound);
	size_t	posCT = buffString.find("Content-Type", posSecBound);
	size_t	posStart = buffString.find("\n", posCT);
	posStart += 3;

	size_t	size = req.get_contentLength() - (posStart - posSecBound) - (secBound.size() + 6);
	std::cout << "size = " << size << std::endl;
	char	*content = new char[size];

	size_t j = 0;
	for (size_t i = posStart; i < posStart + size; i++)
	{
		content[j] = req.get_request()[i];
		j++;
	}
	std::cout << std::endl;

	FILE	*file = fopen("test.png", "w");
	write(fileno(file), content, size);

	return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 30\n\nThis method is not yet handled");
}

std::string	parse(Request const &req)
{
	switch (req.get_method())
	{
		case GET:
			return (request_get(req));
		case POST:
			return (request_post(req));
		case DELETE:
			return (request_delete(req));
		default:
			return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 26\n\nThis method is not handled");
	}
}
