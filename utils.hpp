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
	PATCH
};

enum Version
{
	HTTP_1_0,
	HTTP_1_1,
	HTTP_2_0
};

std::vector<std::string> split(const std::string& str, const std::string& delim = " ");
std::string to_string(Method method);
Method method_from_string(const std::string& method);
std::string to_string(Version version);
std::string	to_string(size_t nb);
std::string	to_string(int nb);
Version version_from_string(const std::string& version);
bool is_not_space(char c);
void ltrim(std::string& str);
void rtrim(std::string& str);
void trim(std::string& str);
//std::vector<Header>::iterator find_header(std::vector<Header> vec, std::string key);


#endif /* UTILS_HPP */
