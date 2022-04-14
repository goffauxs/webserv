#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
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

// std::string	request_post(Request const &req)
// {
	
// }

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

std::string	parse(Request const &req)
{
	switch (req.get_method())
	{
		case GET:
			return (request_get(req));
		// case POST:
		// 	return (request_post(req));
		case DELETE:
			return (request_delete(req));
		default:
			return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 30\n\nThis method is not yet handled");
	}
}
