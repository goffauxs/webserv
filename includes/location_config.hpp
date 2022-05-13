#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP

# include <set>
# include "server_config.hpp"
# include "utils.hpp"

class LocationConfig : public ServerConfig
{
public:
	LocationConfig(const ServerConfig& other, const std::string& content);
	virtual ~LocationConfig() {}
	LocationConfig(const LocationConfig& other);
	LocationConfig(const ServerConfig& other);

	bool					isUploadable() const;
	const std::string&		getCgiExtenstion() const;
private:
	LocationConfig() {}
	LocationConfig& operator=(const LocationConfig&) { return *this; }

	bool				_upload;
	std::string			_cgi_ext;
};

#endif /* LOCATION_CONFIG_HPP */
