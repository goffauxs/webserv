#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

# include <map>
# include <set>
# include "utils.hpp"

class LocationConfig;

class ServerConfig
{
public:
	ServerConfig(const std::string& content);
	ServerConfig(const ServerConfig& other);
	ServerConfig& operator=(const ServerConfig& rhs);
	virtual ~ServerConfig();

	const std::string& 								getRoot() const;
	const std::string& 								getIndex() const;
	const std::string& 								getServerName() const;
	const std::string&								getHost() const;
	int												getPort() const;
	size_t											getClientBodyBufferSize() const;
	std::string										getErrorPage(size_t error_code) const;
	std::map<size_t, std::string>					getErrorPages() const;
	LocationConfig*									getLocation(const std::string& path) const;
	const std::map<std::string, LocationConfig*>&	getLocationMap() const;
	const std::set<Method>&							getAllowedMethods() const;
	bool											isAutoIndexed() const;

	class NotFoundLocation : public std::exception
	{
		const char* what() const throw()
		{
			return "No applicable location found";
		}
	};
	class DuplicateLocationException : public std::exception
	{
	private:
		std::string _str;
	public:
		DuplicateLocationException(const std::string& path) : _str("Error: Duplicate location \"" + path + "\"") {}
		virtual ~DuplicateLocationException() throw() { return; }
		const char* what() const throw() { return _str.c_str(); }
	};
	class InvalidDirective : public std::exception
	{
	private:
		std::string _str;
	public:
		InvalidDirective(const std::string& directive) : _str("Error: Unknown directive \"" + directive + "\"") {}
		virtual ~InvalidDirective() throw() { return; }
		const char* what() const throw() { return _str.c_str(); }
	};
protected:
	ServerConfig() {}

	std::string								_root;
	std::string								_index;
	std::string								_server_name;
	std::string								_host;
	int										_port;
	bool									_autoindex;
	size_t									_client_body_buffer_size;
	std::map<size_t, std::string>			_error_pages;
	std::set<Method>						_allowed_methods;
	std::map<std::string, LocationConfig*>	_locations;
};

#endif /* SERVER_CONFIG_HPP */
