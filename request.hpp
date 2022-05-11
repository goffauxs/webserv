#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <stdlib.h>
#include <iostream>
#include <map>
#include "utils.hpp"

class Request
{
public:
	~Request() { delete [] this->_content; }
	Request(Method method, const std::string& resource, const std::map<std::string, std::string>& headers, Version version = HTTP_1_1)
		: _version(version), _method(method), _resource(resource), _headers(headers) {}
	Request(const char *buff)
		: _content(NULL)
	{
		std::string	request(buff);
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
		requestStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		while (std::getline(requestStream, request_line) && request_line != "\r")
		{
			trim(request_line);
			std::string key, value;
			std::stringstream tmp(request_line);
			std::getline(tmp, key, ':');
			std::getline(tmp, value);
			this->_headers.insert(std::make_pair(key, value));
		}

		//get content-length
		this->_contentLength = 0;
		std::map<std::string, std::string>::iterator it = this->_headers.find("Content-Length");
		if (it != _headers.end())
			this->_contentLength = std::atoi(it->second.c_str());

		//get body
		switch (this->_method)
		{
			case POST:
				_content = new char[_contentLength];
				memcpy(_content, buff + requestStream.tellg(), _contentLength);
				break ;
			default:
				break ;
		}
	}

	Method get_method() const { return this->_method; }
	Version get_version() const { return this->_version; }
	std::string get_resource() const { return this->_resource; }
	std::map<std::string, std::string> get_headers() const { return this->_headers; }
	char	*get_content() const { return this->_content; }
	size_t	get_contentLength() const { return this->_contentLength; }
	
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
	size_t	_contentLength;
};

#endif /* REQUEST_HPP */
