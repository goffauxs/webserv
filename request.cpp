#include "request.hpp"
#include "server_config.hpp"

Request::~Request()
{
	delete [] this->_content;
	delete this->_location;
}

Request::Request(const char* buff, Config& conf)
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

	int			port = 80;
	std::string	host = "";
	it = this->_headers.find("Host");
	if (it != _headers.end())
	{
		std::string	str = it->second;
		if (str.find(":") == std::string::npos)
		{
			port = 80;
			host = str;
		}
		else
		{
			host = str.substr(0, str.find(":"));
			port = std::atoi(str.substr(str.find(":") + 1).c_str());
		}
	}
	ServerConfig	*serv = conf.getServerConfig(port, host);
	try
	{
		_location = serv->getLocation(_resource);
	}
	catch(const std::exception& e)
	{
		_location = new LocationConfig(*serv);
	}
	delete serv;
}

Method Request::get_method() const { return _method; }
Version Request::get_version() const { return _version; }
std::string Request::get_method_string() const { return to_string(_method); }
std::string Request::get_version_string() const { return to_string(_version); }
const std::string& Request::get_resource() const { return _resource; }
std::map<std::string, std::string> Request::get_headers() const { return _headers; }
const char* Request::get_content() const { return _content; }
size_t Request::get_content_length() const { return _content_length; }
const LocationConfig&	Request::get_location() const { return *_location; }
