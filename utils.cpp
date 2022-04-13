#include "utils.hpp"

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
		return GET;
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

bool is_not_space(char c) { return !std::isspace(c); }
void ltrim(std::string& str) { str.erase(str.begin(), std::find_if(str.begin(), str.end(), is_not_space)); }
void rtrim(std::string& str) { str.erase(std::find_if(str.rbegin(), str.rend(), is_not_space).base(), str.end()); }
void trim(std::string& str) { ltrim(str); rtrim(str); }
