#include "request.hpp"
#include "server_config.hpp"

std::map<std::string, std::string> create_cgi_map()
{
	std::map<std::string, std::string> tmp;
	tmp.insert(std::make_pair(".py", "/usr/bin/python2.7"));
	tmp.insert(std::make_pair(".pl", "/usr/bin/perl"));
	return tmp;
}

std::map<std::string, std::string> Request::cgi_exec = create_cgi_map();

Request::~Request()
{
	if (_method == POST)
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
		trim(value);
		this->_headers.insert(std::make_pair(key, value));
	}

	// get host/port
	int			port = 80;
	std::string	host = "";
	std::map<std::string, std::string>::iterator it = this->_headers.find("Host");
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

	// get server config
	ServerConfig	*serv = conf.getServerConfig(port, host);
	try
	{
		if (_headers.count("Referer") && (_method == POST || _resource.find("?") != std::string::npos))
		{
			std::string host_port = "http://" + _headers["Host"];
			std::string referer = _headers["Referer"].substr(host_port.size());
			_location = serv->getLocation(referer);
		}
		else
			_location = serv->getLocation(_resource);
	}
	catch(const std::exception& e)
	{
		_location = new LocationConfig(*serv);
	}
	delete serv;

	//get content-length
	this->_content_length = 0;
	it = this->_headers.find("Content-Length");
	if (it != _headers.end())
		this->_content_length = std::min(strtoul(it->second.c_str(), NULL, 0), _location->getClientBodyBufferSize());

	//get body
	if (this->_method == POST)
	{
		it = _headers.find("Transfer-encoding");
		if (it != _headers.end() && it->second == "chunked")
			dechunk(buff + requestStream.tellg());
		else
		{
			_content = new char[_content_length];
			memcpy(_content, buff + requestStream.tellg(), _content_length);
		}
	}

}

void Request::dechunk(const char* str)
{
	std::vector<char> ret;
	int chunksize = 0;
	chunksize = strtol(str, NULL, 16);
	chunksize = (_content_length + chunksize < _location->getClientBodyBufferSize()) ? chunksize : _location->getClientBodyBufferSize() - _content_length;
	while (*str != '\n')
		str++;
	str++;
	while (chunksize > 0)
	{
		for (int i = 0; i < chunksize; i++, str++)
			ret.push_back(*str);
		str += 2;
		this->_content_length += chunksize;
		chunksize = strtol(str, NULL, 16);
		while (*str != '\n')
			str++;
		str++;
	}
	this->_content = new char[_content_length];
	memcpy(_content, reinterpret_cast<char*>(&ret[0]), _content_length);
	this->_headers.erase("Transfer-encoding");
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
