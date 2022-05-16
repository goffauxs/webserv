#include "webserv.hpp"

std::string	test_file_exec_cgi(std::string full_path, Request const &req)
{
	std::fstream	file(full_path.c_str());
	if (file)
	{
		std::string res = exec_cgi(full_path, req, req.get_location());
		return ("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(res.length()) + "\n\n" + res);
	}
	else
		return (get_error_response(req, 404));
}

std::string check_ext(Request const &req)
{
	std::string	path = "cgi-bin";
	std::string	action = req.get_resource();

	size_t	dot = action.find(".");
	if (dot == std::string::npos)
		return (get_error_response(req, 400));

	std::string	ext = action.substr(dot);
	if (ext.find('?') != std::string::npos)
	{
		action = req.get_resource().substr(0, req.get_resource().find("?"));
		ext.erase(ext.find('?'));
	}
	if (!req.cgi_exec.count(ext))
		return get_error_response(req, 501);
	if (req.get_location().getCgiExtension() != "")
	{
		if (ext == req.get_location().getCgiExtension())
			return (test_file_exec_cgi(path + action, req));
		else
			return (get_error_response(req, 501));
	}
	else
		return (test_file_exec_cgi(path + action, req));
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
			return check_ext(req);
		}
		else
			action = req.get_resource();
	}
	std::fstream	fs((path + action).c_str());
	if (fs && action != "")
	{
		std::ostringstream	stream;
		stream << fs.rdbuf();
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
	else if (req.get_location().isAutoIndexed())
	{
		std::string	res;
		res = autoindex_gen(path + req.get_resource(), req.get_resource());
		if (res != "")
			return ("HTTP/1.1 200 OK\nContent-Length: " + to_string(res.length()) + "\nContent-type:text/html\n\n" + res);
	}
	return (get_error_response(req, 404));
}

std::string	request_delete(Request const &req)
{
	std::string	path = req.get_location().getRoot();

	if (!remove((path + req.get_resource()).c_str()))
		return ("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nFile deleted");
	else
		return (get_error_response(req, 404));
}

std::string	request_post(Request const &req)
{
	return check_ext(req);
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
