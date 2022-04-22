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
		default:
			break;
		}
	}
}

const std::string& ServerConfig::getHost() const { return _host; }
const std::string& ServerConfig::getPort() const { return _port; }
const std::string& ServerConfig::getRoot() const { return _root; }
const std::string& ServerConfig::getIndex() const { return _index; }
const std::string& ServerConfig::getServerName() const { return _server_name; }
const std::map<std::string, LocationConfig>& ServerConfig::getLocations() const { return _locations; }
const std::set<Method>& ServerConfig::getAllowedMethods() const { return _allowed_methods; }
