#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "header.hpp"
#include "utils.hpp"

class Request
{
public:
	Request(Method method, const std::string& resource, const std::vector<Header>& headers, Version version = HTTP_1_1)
		: _version(version), _method(method), _resource(resource), _headers(headers) {}
	Request(const std::string& request)
	{
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
	}

	Method get_method() const { return this->_method; }
	Version get_version() const { return this->_version; }
	std::string get_resource() const { return this->_resource; }
	std::vector<Header> get_headers() const { return this->_headers; }

	std::string serialize() const
	{
		std::string request;
		request = to_string(this->_method) + " " + this->_resource + " " + to_string(this->_version) + LINE_END;
		for (const Header& header : this->_headers)
		{
			request += header.serialize();
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
};

#endif /* REQUEST_HPP */
