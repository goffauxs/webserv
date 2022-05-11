#include "location_config.hpp"
#include <iostream>

LocationConfig::LocationConfig(const ServerConfig& other, const std::string& path, const std::string& content)
	: _path(path), _autoindex(false), _upload(false)
{
	this->_root = other.getRoot();
	this->_index = other.getIndex();
	this->_server_name = other.getServerName();
	this->_host = other.getHost();
	this->_port = other.getPort();
	this->_client_body_buffer_size = other.getClientBodyBufferSize();
	this->_error_pages = other.getErrorPages();
	this->_allowed_methods = other.getAllowedMethods();
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
		switch(directive_from_string(directive))
		{
		case allowed_methods:
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
		case upload_dir:
			lineStream >> this->_upload_dir;
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
		default:
			break;
		}
	}
}

LocationConfig::LocationConfig(const LocationConfig& other)
	: _path(other._path), _autoindex(other._autoindex), _upload(other._upload), _cgi_ext(other._cgi_ext), _upload_dir(other._upload_dir)
{
	this->_root = other.getRoot();
	this->_index = other.getIndex();
	this->_server_name = other.getServerName();
	this->_host = other.getHost();
	this->_port = other.getPort();
	this->_client_body_buffer_size = other.getClientBodyBufferSize();
	this->_error_pages = other.getErrorPages();
	this->_allowed_methods = other.getAllowedMethods();
}

const std::string& 		LocationConfig::getPath() const					{ return this->_path; }
bool 					LocationConfig::isAutoIndexed() const			{ return this->_autoindex; }
bool 					LocationConfig::isUploadable() const			{ return this->_upload; }
const std::string&		LocationConfig::getUploadDir() const			{ return this->_upload_dir; }
const std::string&		LocationConfig::getCgiExtenstion() const 		{ return this->_cgi_ext; }
