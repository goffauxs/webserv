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
		case listen:
			std::getline(lineStream, host_str, ':');
			this->_host.s_addr = inet_addr(host_str.c_str());
			lineStream >> this->_port;
			break;
		default:
			break;
		}
	}
}