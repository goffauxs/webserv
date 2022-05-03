#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP

#include <set>
#include "utils.hpp"
#include "server_config.hpp"

class LocationConfig : public ServerConfig
{
public:
	LocationConfig(const ServerConfig& other, const std::string& path, const std::string& content);
	~LocationConfig() {}
	LocationConfig(const LocationConfig& other);

	const std::string&		getPath() const;
	bool					isAutoIndexed() const;
	bool					isUploadable() const;
	const std::string&		getCgiExtenstion() const;
	const std::string&		getUploadDir() const;
private:
	LocationConfig() {}
	LocationConfig& operator=(const LocationConfig&) { return *this; }

	std::string			_path;
	bool				_autoindex;
	bool				_upload;
	std::string			_cgi_ext;
	std::string			_upload_dir;
	
};

#endif /* LOCATION_CONFIG_HPP */
