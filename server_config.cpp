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
		std::string directive;
		lineStream >> directive;
		switch(directive_from_string(directive))
		{
		case location:
			std::string rest = lineStream.str().substr(lineStream.tellg());
			rest.erase(rest.find_first_of('}'));
		}
	}
}