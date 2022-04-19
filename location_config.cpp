#include "location_config.hpp"

LocationConfig::LocationConfig(const std::string& content)
	: _upload(false), _autoindex(false)
{
	std::stringstream contentStream(content);
	contentStream >> this->_path; // extract path
	contentStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore characters after path until end of line
	
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

const std::string& 		LocationConfig::getPath() const				{ return this->_path; }
const std::set<Method>&	LocationConfig::getAllowedMethods() const	{ return this->_allowed_methods; }
bool 					LocationConfig::isAutoIndexed() const		{ return this->_autoindex; }
bool 					LocationConfig::isUploadable() const		{ return this->_upload; }
const std::string&		LocationConfig::getUploadDir() const		{ return this->_upload_dir; }
const std::string& 		LocationConfig::getRoot() const				{ return this->_root; }
const std::string& 		LocationConfig::getIndex() const			{ return this->_index; }
