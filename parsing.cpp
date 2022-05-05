#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.hpp"
#include "request.hpp"
#include "utils.hpp"
#include "webserv.hpp"
#include "config.hpp"
#include "location_config.hpp"

std::string	request_get(Request const &req)
{
	std::string	path = "server";
	std::string	action;

	if (1/*autoindex is on*/)
	{
		std::string	res;
		res = autoindex_gen(path + req.get_resource(), req.get_resource());
		if (res != "")
			return ("HTTP/1.1 200 OK\nContent-Length: " + to_string(res.length()) + "\nContent-type:text/html\n" + res);
	}
	if (req.get_resource().find("?") != (size_t)-1)
	{
		action = req.get_resource().substr(0, req.get_resource().find("?") - 1);
		Config conf("default.conf"); //FOR TEST ONLY

		std::string res = exec_cgi(path + "/cgi-bin/test.py", req, *conf.getServerList().front()->getLocation("/"));
		std::cout << "res = " << res << std::endl;

		size_t	len = res.substr(res.find("\n\n") + 2).length();
		return ("HTTP/1.1 200 OK\nContent-Length: " + to_string(len) + "\n" + res);
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
		+ "\nContent-Length: " + to_string(body.length()) + "\n\n" + body);
	}
	else
	{
		std::ifstream	errFile("errors/404.html");
		std::ostringstream	stream;
		stream << errFile.rdbuf();
		std::string	body = stream.str();

		return ("HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + to_string(body.length()) + "\n\n" + body);
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

		return ("HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + to_string(body.length()) + "\n\n" + body);
	}
}

std::string	request_post(Request const &req)
{
	Config conf("default.conf"); //FOR TEST ONLY
	std::string res = exec_cgi("server/cgi-bin/upload.py", req, *conf.getServerConfig("8000", "youpi")->getLocation("/"));
	// std::string res = exec_cgi("server/cgi-bin/upload.py", req, conf);
	// std::cout << "res = " << res << std::endl;

	// size_t	len = res.substr(res.find("\n\n") + 2).length();
	return ("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(res.length()) + "\n\n" + res);
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
