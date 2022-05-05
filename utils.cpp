#include "utils.hpp"
#include <algorithm>

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	std::string strCopy = str;

	size_t pos = 0;
	std::string token;

	while ((pos = strCopy.find(delim)) != std::string::npos)
	{
		token = strCopy.substr(0, pos);
		strCopy.erase(0, pos + delim.length());

		tokens.push_back(token);
	}
	if (strCopy.length() > 0)
		tokens.push_back(strCopy);
	return tokens;
}

std::string concat(const std::vector<std::string>& strings, const std::string& delim)
{
	std::string res;

	for (size_t i = 0; i < strings.size(); i++)
	{
		res += strings[i];

		if ((i + 1) != strings.size())
			res += delim;
	}
	return res;
}

std::string to_string(Method method)
{
	switch (method)
	{
	case GET:
		return "GET";
	case HEAD:
		return "HEAD";
	case POST:
		return "POST";
	case PUT:
		return "PUT";
	case DELETE:
		return "DELETE";
	case TRACE:
		return "TRACE";
	case OPTIONS:
		return "OPTIONS";
	case CONNECT:
		return "CONNECT";
	case PATCH:
		return "PATCH";
	default:
		return "INVALID";
	}
}

Method method_from_string(const std::string& method)
{
	if (method == to_string(GET))
		return GET;
	else if (method == to_string(HEAD))
		return HEAD;
	else if (method == to_string(POST))
		return POST;
	else if (method == to_string(PUT))
		return PUT;
	else if (method == to_string(DELETE))
		return DELETE;
	else if (method == to_string(TRACE))
		return TRACE;
	else if (method == to_string(OPTIONS))
		return OPTIONS;
	else if (method == to_string(CONNECT))
		return CONNECT;
	else if (method == to_string(PATCH))
		return PATCH;
	else
		return INVALID_METHOD;
}

std::string to_string(Version version)
{
	switch (version)
	{
	case HTTP_1_0:
		return "HTTP/1.0";
	case HTTP_1_1:
		return "HTTP/1.1";
	case HTTP_2_0:
		return "HTTP/2.0";
	}
}

Version version_from_string(const std::string& version)
{
	if (version == to_string(HTTP_1_0))
		return HTTP_1_0;
	else if (version == to_string(HTTP_1_1))
		return HTTP_1_1;
	else if (version == to_string(HTTP_2_0))
		return HTTP_2_0;
	else
		return HTTP_1_1;
}

std::string	to_string(size_t nb)
{
	std::ostringstream convert;

	convert << nb;
	return (convert.str());
}

std::string	to_string(int nb)
{
	std::ostringstream convert;

	convert << nb;
	return (convert.str());
}

std::string to_string(Directive directive)
{
	switch(directive)
	{
	case allowed_methods:
		return "allowed_methods";
	case autoindex:
		return "autoindex";
	case upload:
		return "upload";
	case upload_dir:
		return "upload_dir";
	case root:
		return "root";
	case default_index:
		return "index";
	case server_name:
		return "server_name";
	case directive_listen:
		return "listen";
	case location:
		return "location";
	case server:
		return "server";
	case client_body_buffer_size:
		return "client_body_buffer_size";
	case cgi_ext:
		return "cgi_ext";
	case error_page:
		return "error_page";
	default:
		return "invalid";
	}
}

Directive directive_from_string(const std::string& directive)
{
	if (directive == to_string(allowed_methods))
		return allowed_methods;
	else if (directive == to_string(autoindex))
		return autoindex;
	else if (directive == to_string(upload))
		return upload;
	else if (directive == to_string(upload_dir))
		return upload_dir;
	else if (directive == to_string(root))
		return root;
	else if (directive == to_string(default_index))
		return default_index;
	else if (directive == to_string(server_name))
		return server_name;
	else if (directive == to_string(directive_listen))
		return directive_listen;
	else if (directive == to_string(location))
		return location;
	else if (directive == to_string(server))
		return server;
	else if (directive == to_string(client_body_buffer_size))
		return client_body_buffer_size;
	else if (directive == to_string(cgi_ext))
		return cgi_ext;
	else if (directive == to_string(error_page))
		return error_page;
	else
		return INVALID_DIRECTIVE;
}

bool is_not_space(char c) { return !std::isspace(c); }
void ltrim(std::string& str) { str.erase(str.begin(), std::find_if(str.begin(), str.end(), is_not_space)); }
void rtrim(std::string& str) { str.erase(std::find_if(str.rbegin(), str.rend(), is_not_space).base(), str.end()); }
void trim(std::string& str) { ltrim(str); rtrim(str); }
