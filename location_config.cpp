#include "location_config.hpp"

LocationConfig::LocationConfig(const std::string& path, const std::string& content)
	: _path(path), _autoindex(false), _upload(false)
{
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
		default:
			break;
		}
	}
}

LocationConfig::LocationConfig(const LocationConfig& other)
	: _path(other._path), _allowed_methods(other._allowed_methods), _autoindex(other._autoindex), _upload(other._upload), _upload_dir(other._upload_dir), _root(other._root), _index(other._index)
{
}

const std::string& 		LocationConfig::getPath() const				{ return this->_path; }
const std::set<Method>&	LocationConfig::getAllowedMethods() const	{ return this->_allowed_methods; }
bool 					LocationConfig::isAutoIndexed() const		{ return this->_autoindex; }
bool 					LocationConfig::isUploadable() const		{ return this->_upload; }
const std::string&		LocationConfig::getUploadDir() const		{ return this->_upload_dir; }
const std::string& 		LocationConfig::getRoot() const				{ return this->_root; }
const std::string& 		LocationConfig::getIndex() const			{ return this->_index; }
