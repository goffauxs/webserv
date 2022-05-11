#include "request.hpp"

Request::Request(Method method, const std::string& resource, const std::map<std::string, std::string>& headers, Version version = HTTP_1_1)
	: _version(version), _method(method), _resource(resource), _headers(headers)
{
}

Request::~Request() { delete [] this->_content; }

Request::Request(const char* buff)
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
	this->_content_length = 0;
	std::map<std::string, std::string>::iterator it = this->_headers.find("Content-Length");
	if (it != _headers.end())
		this->_content_length = std::atoi(it->second.c_str());

	//get body
	switch (this->_method)
	{
		case POST:
			_content = new char[_content_length];
			memcpy(_content, buff + requestStream.tellg(), _content_length);
			break ;
		default:
			break ;
	}
}

Method Request::get_method() const { return _method; }
Version Request::get_version() const { return _version; }
std::string Request::get_method_string() const { return to_string(_method); }
std::string Request::get_version_string() const { return to_string(_version); }
const std::string& Request::get_resource() const { return _resource; }
std::map<std::string, std::string> Request::get_headers() const { return _headers; }
const char* Request::get_content() const { return _content; }
size_t Request::get_content_length() const { return _content_length; }