#include "server_config.hpp"

ServerConfig::ServerConfig(const std::string& content)
{
	std::stringstream contentStream(content);
	contentStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::string buffer;
	while (std::getline(contentStream, buffer))
	{
		trim(buffer);
		std::stringstream lineStream(buffer);
		std::string directive;
		lineStream >> directive;
		std::string rest;
		std::string path;
		std::string host_str;
		switch(directive_from_string(directive))
		{
		case location:
			rest = contentStream.str().substr(contentStream.tellg());
			rest.erase(rest.find_first_of('}'));
			contentStream.seekg(rest.size(), std::ios_base::cur);
			lineStream >> path;
			this->_locations.insert(std::make_pair(path, LocationConfig(path, rest)));
			break;
		case root:
			lineStream >> this->_root;
			break;
		case default_index:
			lineStream >> this->_index;
		case allowed_methods:
			while (!lineStream.eof())
			{
				std::string tmp;
				lineStream >> tmp;
				this->_allowed_methods.insert(method_from_string(tmp));
			}
			break;
		case server_name:
			lineStream >> this->_server_name;
			break;
		case directive_listen:
			std::getline(lineStream, this->_host, ':');
			lineStream >> this->_port;
			break;
		case client_body_buffer_size:
			lineStream >> this->_client_body_buffer_size;
			break;
		default:
			break;
		}
	}
}

ServerConfig& ServerConfig::operator=(const ServerConfig& rhs)
{
	if (this != &rhs)
	{
		_root = rhs._root;
		_index = rhs._index;
		_server_name = rhs._server_name;
		_host = rhs._host;
		_port = rhs._port;
		_client_body_buffer_size = rhs._client_body_buffer_size;
		_allowed_methods = rhs._allowed_methods;
		_locations = rhs._locations;
	}
	return *this;
}

ServerConfig::ServerConfig(const ServerConfig& other)
	: _root(other._root), _index(other._index), _server_name(other._server_name),
		_host(other._host), _port(other._port), _client_body_buffer_size(other._client_body_buffer_size),
		_allowed_methods(other._allowed_methods), _locations(other._locations)
{
}

const std::string& ServerConfig::getHost() const { return _host; }
const std::string& ServerConfig::getPort() const { return _port; }
const std::string& ServerConfig::getRoot() const { return _root; }
const std::string& ServerConfig::getIndex() const { return _index; }
const std::string& ServerConfig::getServerName() const { return _server_name; }
const std::map<std::string, LocationConfig>& ServerConfig::getLocationMap() const { return _locations; }
const std::set<Method>& ServerConfig::getAllowedMethods() const { return _allowed_methods; }
size_t ServerConfig::getClientBodyBufferSize() const	{ return this->_client_body_buffer_size; }

const LocationConfig& ServerConfig::getLocation(const std::string& path) const
{
	std::map<std::string, LocationConfig>::const_iterator it = this->_locations.find(path);
	if (it != this->_locations.end())
		return it->second;
	else
		throw NotFoundLocation();
}
