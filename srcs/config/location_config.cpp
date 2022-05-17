#include "location_config.hpp"

LocationConfig::LocationConfig(const ServerConfig& other, const std::string& content)
	: _upload(false)
{
	this->_root = other.getRoot();
	this->_index = other.getIndex();
	this->_server_name = other.getServerName();
	this->_host = other.getHost();
	this->_port = other.getPort();
	this->_client_body_buffer_size = other.getClientBodyBufferSize();
	this->_error_pages = other.getErrorPages();
	this->_allowed_methods = other.getAllowedMethods();
	this->_autoindex = false;
	std::stringstream contentStream(content);
	
	std::string buffer;
	while (std::getline(contentStream, buffer))
	{
		trim(buffer);
		if (buffer == "}")
			break;
		std::stringstream	lineStream(buffer);
		std::string			directive;
		lineStream >> directive;
		if (directive[0] == '#')
			continue;
		switch(directive_from_string(directive))
		{
		case allowed_methods:
			this->_allowed_methods.clear();
			while (!lineStream.eof())
			{
				std::string tmp;
				lineStream >> tmp;
				this->_allowed_methods.insert(method_from_string(tmp));
			}
			break;
		case autoindex:
			this->_autoindex = true;
			break;
		case upload:
			this->_upload = true;
			break;
		case root:
			lineStream >> this->_root;
			break;
		case default_index:
			lineStream >> this->_index;
			break;
		case client_body_buffer_size:
			lineStream >> this->_client_body_buffer_size;
			break;
		case cgi_ext:
			lineStream >> this->_cgi_ext;
			break;
		case INVALID_DIRECTIVE:
			throw InvalidDirective(directive);
		default:
			break;
		}
	}
}

LocationConfig::LocationConfig(const LocationConfig& other)
	: _upload(other._upload), _cgi_ext(other._cgi_ext)
{
	this->_root = other.getRoot();
	this->_index = other.getIndex();
	this->_server_name = other.getServerName();
	this->_host = other.getHost();
	this->_port = other.getPort();
	this->_client_body_buffer_size = other.getClientBodyBufferSize();
	this->_error_pages = other.getErrorPages();
	this->_allowed_methods = other.getAllowedMethods();
	this->_autoindex = other.isAutoIndexed();
}

LocationConfig::LocationConfig(const ServerConfig& other)
	: _upload(false), _cgi_ext("")
{
	this->_root = other.getRoot();
	this->_index = other.getIndex();
	this->_server_name = other.getServerName();
	this->_host = other.getHost();
	this->_port = other.getPort();
	this->_client_body_buffer_size = other.getClientBodyBufferSize();
	this->_error_pages = other.getErrorPages();
	this->_allowed_methods = other.getAllowedMethods();
	this->_autoindex = other.isAutoIndexed();
}

bool 					LocationConfig::isUploadable() const			{ return this->_upload; }
const std::string&		LocationConfig::getCgiExtension() const 		{ return this->_cgi_ext; }
