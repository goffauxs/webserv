#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <stdlib.h>
#include <iostream>
#include <map>
#include "utils.hpp"

class Request
{
public:
	Request(Method method, const std::string& resource, const std::map<std::string, std::string>& headers, Version version = HTTP_1_1)
		: _version(version), _method(method), _resource(resource), _headers(headers) {}
	Request(char *buff)
		: _content(NULL)
	{
		this->_request = buff;
		std::string	request = buff;
		std::stringstream requestStream(request);
		std::string request_line;

		std::string methodString, protocol;
		requestStream >> methodString;
		_method = method_from_string(methodString);
		if (_method == INVALID_METHOD)
			throw InvalidRequest();
		requestStream >> _resource;
		requestStream >> protocol;
		_version = version_from_string(protocol);
		

		while (std::getline(requestStream, request_line) && request_line != "\r")
		{
			trim(request_line);
			std::string key, value;
			std::stringstream tmp(request_line);
			std::getline(tmp, key, ':');
			tmp >> value;
			this->_headers.insert(std::make_pair(key, value));
		}

		//get content-length
		this->_content_length = 0;
		char *endptr = NULL;
		std::map<std::string, std::string>::iterator it = this->_headers.find("Content-Length");
		if (it != _headers.end())
			this->_content_length = strtol(it->second.c_str(), &endptr, 0);
		if (endptr != '\0')
			throw InvalidRequest();

		// get body
		switch (this->_method)
		{
			case POST:
				{
					requestStream.read(_content, _content_length);
				}
				break ;
			default:
				break ;
		}
	}

	Method get_method() const { return this->_method; }
	Version get_version() const { return this->_version; }
	std::string get_resource() const { return this->_resource; }
	std::map<std::string, std::string> get_headers() const { return this->_headers; }
	char	*get_request() const { return this->_request; }
	char	*get_content() const { return this->_content; }
	size_t	get_content_length() const { return this->_content_length; }
	
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
	char	*_request;
	char	*_content; //to delete in destructor or we can use vector of char but less convenient
	size_t	_content_length;
};

#endif /* REQUEST_HPP */
