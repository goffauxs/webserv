#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <stdlib.h>
#include "header.hpp"

class Request
{
public:
	Request(Method method, const std::string& resource, const std::vector<Header>& headers, Version version = HTTP_1_1)
		: _version(version), _method(method), _resource(resource), _headers(headers) {}
	Request(char *buff)
	{
		this->_content = buff;
		std::string	request = buff;
		std::stringstream requestStream(request);
		std::string request_line;
		std::getline(requestStream, request_line);
		trim(request_line);

		std::vector<std::string> segments = split(request_line);
		this->_method = method_from_string(segments[0]);
		this->_resource = segments[1];
		this->_version = version_from_string(segments[2]);

		while (std::getline(requestStream, request_line) && request_line != "\r")
		{
			trim(request_line);
			Header tmp(request_line);
			this->_headers.push_back(tmp);
		}

		std::vector<Header> headers = this->_headers;
		for (std::vector<Header>::iterator it = headers.begin(); it != headers.end(); it++)
			if (it->get_key() == "Content-Length")
			{
				this->_contentLength = atoi(it->get_value().c_str());
				break ;
			}

		this->_body = requestStream.str().substr(requestStream.tellg());


	}

	Method get_method() const { return this->_method; }
	Version get_version() const { return this->_version; }
	std::string get_resource() const { return this->_resource; }
	std::vector<Header> get_headers() const { return this->_headers; }
	std::string	get_body() const { return this->_body; }
	char	*get_content() const { return this->_content; }
	size_t	get_contentLength() const { return this->_contentLength; }

	std::string serialize() const
	{
		std::string request;
		request = to_string(this->_method) + " " + this->_resource + " " + to_string(this->_version) + LINE_END;
		for (std::vector<Header>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
		{
			request += it->serialize();
		}
		request += LINE_END;
		return request;
	}
private:
	Request() {}
	Request(const Request&) {}
	Request& operator==(const Request&) { return *this; }

	Version _version;
	Method _method;
	std::string _resource;
	std::vector<Header> _headers;
	char	*_content;
	size_t	_contentLength;
	std::string	_body;
};

#endif /* REQUEST_HPP */
