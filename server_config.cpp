#include "server_config.hpp"

ServerConfig::ServerConfig(const std::string& content)
{
	std::stringstream contentStream(content);
	contentStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::string buffer;
	while (std::getline(contentStream, buffer))
	{
		trim(buffer);
		if (buffer == "}")
			break;
		std::stringstream lineStream(buffer);
		/* TODO figure out how to empty the stringstream up to a certain 
			delimiter and send it to the LocationConfig constructor */
	}
}