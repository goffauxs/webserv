#ifndef UTILS_HPP
# define UTILS_HPP
# include <string>
# include <vector>

const static std::string LINE_END = "\r\n";

std::vector<std::string> split(const std::string& str, const std::string& delim = " ")
{
	std::vector<std::string> tokens;
	std::string strCopy = str;

	size_t pos = 0;
	std::string token;

	while ((pos = strCopy.find(delim)) != std::string::npos)
	{
		token = strCopy.substr(0, pos);
		strCopy.substr(0, pos + delim.length());

		tokens.push_back(token);
	}
	if (strCopy.length() > 0)
		tokens.push_back(strCopy);
	return tokens;
}

std::string concat(const std::vector<std::string>& strings, const std::string& delim = " ")
{
	std::string res;

	for (int i = 0; i < strings.size(); i++)
	{
		res += strings[i];

		if ((i + 1) != strings.size())
			res += delim;
	}
	return res;
}

enum Method
{
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	TRACE,
	OPTIONS,
	CONNECT,
	PATCH
};

std::string to_string(Method method)
{
	switch (method)
	{
	case Method::GET:
		return "GET";
	case Method::HEAD:
		return "HEAD";
	case Method::POST:
		return "POST";
	case Method::PUT:
		return "PUT";
	case Method::DELETE:
		return "DELETE";
	case Method::TRACE:
		return "TRACE";
	case Method::OPTIONS:
		return "OPTIONS";
	case Method::CONNECT:
		return "CONNECT";
	case Method::PATCH:
		return "PATCH";
	}
}

Method method_from_string(const std::string& method)
{
	if (method == to_string(Method::GET))
		return Method::GET;
	else if (method == to_string(Method::HEAD))
		return Method::HEAD;
	else if (method == to_string(Method::POST))
		return Method::POST;
	else if (method == to_string(Method::PUT))
		return Method::PUT;
	else if (method == to_string(Method::DELETE))
		return Method::DELETE;
	else if (method == to_string(Method::TRACE))
		return Method::TRACE;
	else if (method == to_string(Method::OPTIONS))
		return Method::OPTIONS;
	else if (method == to_string(Method::CONNECT))
		return Method::CONNECT;
	else if (method == to_string(Method::PATCH))
		return Method::PATCH;
}

enum Version
{
	HTTP_1_0,
	HTTP_1_1,
	HTTP_2_0
};

std::string to_string(Version version)
{
	switch (version)
	{
	case Version::HTTP_1_0:
		return "HTTP/1.0";
	case Version::HTTP_1_1:
		return "HTTP/1.1";
	case Version::HTTP_2_0:
		return "HTTP/2.0";
	}
}

Version version_from_string(const std::string& version)
{
	if (version == to_string(Version::HTTP_1_0))
		return Version::HTTP_1_0;
	else if (version == to_string(Version::HTTP_1_1))
		return Version::HTTP_1_1;
	else if (version == to_string(Version::HTTP_2_0))
		return Version::HTTP_2_0;
}

bool is_not_space(char c) { return !std::isspace(c); }
void ltrim(std::string& str) { str.erase(str.begin(), std::find_if(str.begin(), str.end(), is_not_space)); }
void rtrim(std::string& str) { str.erase(std::find_if(str.rbegin(), str.rend(), is_not_space).base(), str.end()); }
void trim(std::string& str) { ltrim(str); rtrim(str); }

#endif /* UTILS_HPP */
