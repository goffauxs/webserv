#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <map>
#include <set>
#include <arpa/inet.h>
#include "location_config.hpp"
#include "utils.hpp"

class ServerConfig
{
public:
	ServerConfig(const std::string& content);

	const std::string& 								getRoot() const;
	const std::string& 								getIndex() const;
	const std::string& 								getServerName() const;
	const std::string&								getHost() const;
	const std::string&								getPort() const;
	const LocationConfig&							getLocation(const std::string& path) const;
	const std::map<std::string, LocationConfig>&	getLocationMap() const;
	const std::set<Method>&							getAllowedMethods() const;

	class NotFoundLocation : public std::exception
	{
		const char* what() const throw()
		{
			return "No applicable location found";
		}
	};
private:
	std::string 							_root;
	std::string								_index;
	std::string 							_server_name;
	std::string								_host;
	std::string								_port;
	std::set<Method>						_allowed_methods;
	std::map<std::string, LocationConfig>	_locations;
};

#endif /* SERVER_CONFIG_HPP */
