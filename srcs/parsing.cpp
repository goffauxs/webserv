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

std::string	get_error_response(Request const &req, size_t error_code)
{
	std::string	error_page = req.get_location().getErrorPage(error_code);
	std::cout << "error_page = " << error_page << std::endl;

	std::ifstream	errFile(error_page);
	std::ostringstream	stream;
	stream << errFile.rdbuf();
	std::string	body = stream.str();
	std::cout << "body = " << body << std::endl;

	std::string	full_code;
	switch (error_code)
	{
		case 404:
			full_code = "404 Not Found";
			break;
		case 405:
			full_code = "405 Method Not Allowed";
			break;
		case 501:
			full_code = "501 Not Implemented";
			break;
	}

	return ("HTTP/1.1" + full_code + "\nContent-Type: text/html\nContent-Length: " + to_string(body.length()) + "\n\n" + body);
}

std::string	request_get(Request const &req)
{
	std::string	path = "server";
	std::string	action;

	if (*req.get_resource().rbegin() == '/')
	{
		if (req.get_location().getIndex() != "")
			action = "/" + req.get_location().getIndex();
		else if (req.get_location().isAutoIndexed())
		{
			std::string	res;
			res = autoindex_gen(path + req.get_resource(), req.get_resource());
			if (res != "")
				return ("HTTP/1.1 200 OK\nContent-Length: " + to_string(res.length()) + "\nContent-type:text/html\n\n" + res);
		}
	}
	else
	{
		if (req.get_resource().find("?") != std::string::npos)
		{
			action = req.get_resource().substr(0, req.get_resource().find("?") - 1);

			std::string res = exec_cgi("cgi-bin/get.py", req, req.get_location());

			size_t	len = res.substr(res.find("\n\n") + 2).length();
			return ("HTTP/1.1 200 OK\nContent-Length: " + to_string(len) + "\n" + res);
		}
		else
			action = req.get_resource();
	}
	std::ifstream	ifs((path + action).c_str());
	if (ifs && action != "")
	{
		std::ostringstream	stream;
		stream << ifs.rdbuf();
		std::string	body = stream.str();

		std::map<std::string, std::string> headers(req.get_headers());
		std::string	accept;
		std::map<std::string, std::string>::iterator it = headers.find("Accept");
		if (it != headers.end())
		{
			accept = it->second;
		}
		std::string mime = mime_select(action.substr(std::min(action.rfind("."), action.length())));
		return ("HTTP/1.1 200 OK\nContent-Type: " + mime
		+ "\nContent-Length: " + to_string(body.length()) + "\n\n" + body);
	}
	return (get_error_response(req, 404));
}

std::string	request_delete(Request const &req)
{
	std::string	path = "server";

	if (!remove((path + req.get_resource()).c_str()))
		return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nFile deleted");
	else
		return (get_error_response(req, 404));
}

std::string	request_post(Request const &req)
{
	std::string res = exec_cgi("cgi-bin/upload.py", req, req.get_location());

	return ("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(res.length()) + "\n\n" + res);
}

std::string	parse(Request const &req)
{
	switch (req.get_method())
	{
		case GET:
			if (req.get_location().getAllowedMethods().count(GET))
				return (request_get(req));
			return (get_error_response(req, 405));
		case POST:
			if (req.get_location().getAllowedMethods().count(POST))
				return (request_post(req));
			return (get_error_response(req, 405));
		case DELETE:
			if (req.get_location().getAllowedMethods().count(DELETE))
				return (request_delete(req));
			return (get_error_response(req, 405));
		default:
			return (get_error_response(req, 501));
	}
}
