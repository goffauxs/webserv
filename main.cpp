#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "utils.hpp"
#include "location_config.hpp"

int main()
{
	std::string test = "server {\nallowed_methods GET POST PUT DELETE\nroot server/\nlocation /put_test {\nroot ./YoupiBanane/put_here\nallowed_methods PUT\n}\nlocation /post_test {\nroot ./YoupiBanane/post_test\nallowed_methods POST\n}\n}\n";
	std::stringstream testStream(test);
	std::map<std::string, LocationConfig> _locations;

	std::string buffer;
	while (std::getline(testStream, buffer))
	{
		std::cout << "-- BUFFER -- [" << buffer << "]" << std::endl;
		trim(buffer);
		if (buffer == "}")
			break;
		std::stringstream lineStream(buffer);
		std::string directive;
		lineStream >> directive;
		std::string rest;
		std::string path;
		switch(directive_from_string(directive))
		{
		case location:
			rest = testStream.str().substr(testStream.tellg());
			rest.erase(rest.find_first_of('}'));
			lineStream >> path;
			_locations.insert(std::make_pair(path, LocationConfig(path, rest)));
			break;
		default:
			break;
		}
	}
	for (std::map<std::string, LocationConfig>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		std::cout << "affected URI: " << it->second.getPath() << std::endl;
		std::cout << "root directory: " << it->second.getRoot() << std::endl;
		std::cout << "index file: " << it->second.getIndex() << std::endl;
		std::cout << "allowed methods:";
		for (std::set<Method>::iterator itr = it->second.getAllowedMethods().begin(); itr != it->second.getAllowedMethods().end(); itr++)
			std::cout << " " << to_string(*itr);
		std::cout << std::endl;
		std::cout << "is autoindexed?: " << std::boolalpha << it->second.isAutoIndexed() << std::endl;
		std::cout << "is uploadable?: " << std::boolalpha << it->second.isUploadable() << std::endl;
	}
}
