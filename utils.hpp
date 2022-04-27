#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include <vector>
#include <sstream>

const static std::string LINE_END = "\r\n";

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
	PATCH,
	INVALID_METHOD
};

enum Version
{
	HTTP_1_0,
	HTTP_1_1,
	HTTP_2_0
};

enum Directive
{
	allowed_methods,
	autoindex,
	upload,
	upload_dir,
	root,
	default_index,
	server_name,
	directive_listen,
	location,
	server,
	client_body_buffer_size,
	cgi_ext,
	INVALID_DIRECTIVE
};

std::vector<std::string> split(const std::string& str, const std::string& delim = " ");
std::string to_string(Method method);
Method method_from_string(const std::string& method);
std::string to_string(Version version);
Version version_from_string(const std::string& version);
std::string to_string(Directive directive);
Directive directive_from_string(const std::string& directive);
bool is_not_space(char c);
void ltrim(std::string& str);
void rtrim(std::string& str);
void trim(std::string& str);


#endif /* UTILS_HPP */
