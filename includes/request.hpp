#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <stdlib.h>
#include <iostream>
#include <map>
#include <limits>
#include <string.h>
#include "utils.hpp"
#include "config.hpp"
#include "location_config.hpp"

class Request
{
public:
	~Request();
	Request(const char *buff, Config& conf);

	Method								get_method() const;
	Version								get_version() const;
	std::string							get_method_string() const;
	std::string							get_version_string() const;
	const std::string&					get_resource() const;
	std::map<std::string, std::string>	get_headers() const;
	const char*							get_content() const;
	size_t								get_content_length() const;
	const LocationConfig&				get_location() const;
	
	class InvalidRequest : std::exception
	{
		const char* what() const throw() { return "Invalid Request"; }
	};
private:
	Request() {}
	Request(const Request&) {}
	Request& operator=(const Request&) { return *this; }

	Version _version;
	Method _method;
	std::string _resource;
	std::map<std::string, std::string> _headers;
	char	*_content;
	size_t	_content_length;
	LocationConfig	*_location;
};

#endif /* REQUEST_HPP */