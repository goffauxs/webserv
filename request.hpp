#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <stdlib.h>
#include <iostream>
#include "header.hpp"

class Request
{
public:
	Request(Method method, const std::string& resource, const std::vector<Header>& headers, Version version = HTTP_1_1)
		: _version(version), _method(method), _resource(resource), _headers(headers) {}
	Request(char *buff)
	{
		this->_request = buff;
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

		//get content-length
		std::vector<Header> headers = this->_headers;
		for (std::vector<Header>::iterator it = headers.begin(); it != headers.end(); it++)
			if (it->get_key() == "Content-Length")
			{
				this->_contentLength = atoi(it->get_value().c_str());
				break ;
			}

		//get body
		this->_content = NULL;
		size_t	i;
		size_t	j = 0;
		switch (this->_method)
		{
			case POST:
				i = request.find("\r\n\r\n");
				i += 4;
				j = 0;
				this->_content = new char[this->_contentLength + 1];
				while (j < this->_contentLength)
					this->_content[j++] = buff[i++];
				this->_content[j] = '\0';
				break ;
			default:
				break ;
		}
	}

	Method get_method() const { return this->_method; }
	Version get_version() const { return this->_version; }
	std::string get_resource() const { return this->_resource; }
	std::vector<Header> get_headers() const { return this->_headers; }
	char	*get_request() const { return this->_request; }
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
	char	*_request;
	char	*_content; //to delete in destructor or we can use vector of char but less convenient
	size_t	_contentLength;
};

#endif /* REQUEST_HPP */
