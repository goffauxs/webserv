#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "server_config.hpp"
#include "utils.hpp"

class Config
{
public:
	Config(const std::string& path);
	Config(const Config& other);
	Config& operator=(const Config& other);

	const ServerConfig& getServerConfig() const;
private:
	void advance_to_next_bracket(std::ifstream& stream);
};

#endif /* CONFIG_HPP */
