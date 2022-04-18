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

std::string	request_get(Request const &req)
{
	std::string	path = "server";

	std::ifstream	ifs((path + req.get_resource()).c_str());
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

std::string	request_post(Request const &req)
{
	FILE	*file = fopen("test.c", "w");

	// size_t	EBound = (req.get_body().find("\r"));
	// std::string	boundary = req.get_body().substr(0, EBound);

	// size_t	start = req.get_body().find("\n", req.get_body().find("Content-Type:"));
	// start += 3;
	// boundary.append("--");
	// size_t	end = req.get_body().find(boundary);
	// std::string cont = req.get_body().substr(start, end - start - 2);
	write(fileno(file), req.get_body().c_str(), 46294);

	return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 30\n\nThis method is not yet handled");
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

std::string	post_meth(Request const &req, char *buff)
{
	std::vector<Header> headers = req.get_headers();
	std::string	contentLength;
	for (std::vector<Header>::iterator it = headers.begin(); it != headers.end(); it++)
		if (it->get_key() == "Content-Length")
		{
			contentLength = it->get_value();
			break ;
		}
	int	CL = atoi(contentLength.c_str());

	char	*content = new char[CL];

	std::string	buffString = buff;
	size_t	posBound = buffString.find("boundary=") + 9;
	std::string	boundary = buffString.substr(posBound, buffString.find("\r", posBound) - posBound);

	std::string	secBound = boundary;
	secBound.insert(0, "--");
	size_t	posSecBound = buffString.find(secBound);
	size_t	posCT = buffString.find("Content-Type", posSecBound);
	size_t	posStart = buffString.find("\n", posCT);
	posStart += 3;

	int j = 0;
	for (int i = posStart; i < posStart + (CL - (posStart - posSecBound) - (secBound.size() + 6)); i++)
	{
		content[j] = buff[i];
		j++;
	}
	std::cout << std::endl;

	FILE	*file = fopen("test.png", "w");
	write(fileno(file), content, CL - (posStart - posSecBound) - (secBound.size() + 6));

	return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 30\n\nThis method is not yet handled");
}

std::string	parse(Request const &req, char *buff)
{
	switch (req.get_method())
	{
		case GET:
			return (request_get(req));
		case POST:
			return (post_meth(req, buff));
			// return (request_post(req));
		case DELETE:
			return (request_delete(req));
		default:
			return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 30\n\nThis method is not yet handled");
	}
}
