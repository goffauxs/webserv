#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP

#include <set>
#include "utils.hpp"

class LocationConfig
{
public:
	LocationConfig(const std::string& path, const std::string& content);
	LocationConfig(const LocationConfig& other);

	const std::string&		getPath() const;
	const std::set<Method>&	getAllowedMethods() const;
	bool					isAutoIndexed() const;
	bool					isUploadable() const;
	const std::string&		getUploadDir() const;
	const std::string&		getRoot() const;
	const std::string&		getIndex() const;
private:
	LocationConfig() {}
	LocationConfig& operator=(const LocationConfig&) { return *this; }

	std::string			_path;
	std::set<Method>	_allowed_methods;
	bool				_autoindex;
	bool				_upload;
	std::string			_upload_dir;
	std::string			_root;
	std::string			_index;
};

#endif /* LOCATION_CONFIG_HPP */
