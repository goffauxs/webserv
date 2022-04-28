#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <stack>
#include <list>
#include "utils.hpp"
#include "server_config.hpp"
#include "location_config.hpp"
#include "config.hpp"

int main()
{
	try
	{
		Config config("default.conf");
		ServerConfig conf = config.getServerConfig("8000", "youpi");
		std::cout << "server root: " << conf.getRoot() << std::endl;
		try
		{
			LocationConfig loc = conf.getLocation("/directory/upload.bla");
			std::cout << "cgi_ext: " << loc.getCgiExtenstion() << std::endl;
			std::cout << "root: " << loc.getRoot() << std::endl;
			std::cout << "indext: " << loc.getIndex() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	// for (std::list<ServerConfig>::iterator it = list.begin(); it != list.end(); it++)
	// {
	// 	std::cout << "--- SERVER ---" << std::endl;
	// 	std::cout << "\tserver_name: " << it->getServerName() << std::endl;
	// 	std::cout << "\tindex: " << it->getIndex() << std::endl;
	// 	std::cout << "\troot: " << it->getRoot() << std::endl;
	// 	std::cout << "\thost: " << it->getHost() << std::endl;
	// 	std::cout << "\tport: " << it->getPort() << std::endl;
	// 	std::cout << "\tallowed_methods: ";
	// 	for (std::set<Method>::iterator itr = it->getAllowedMethods().begin(); itr != it->getAllowedMethods().end(); itr++)
	// 		std::cout << to_string(*itr);
	// 	std::cout << std::endl;
	// 	for (std::map<std::string, LocationConfig>::const_iterator itr = it->getLocations().begin(); itr != it->getLocations().end(); itr++)
	// 	{
	// 		std::cout << "\t--- LOCATION ---" << std::endl;
	// 		std::cout << "\t\tpath: " << itr->second.getPath() << std::endl;
	// 		std::cout << "\t\tallowed_methods: ";
	// 		std::set<Method> allowed_methods = itr->second.getAllowedMethods();
	// 		for (std::set<Method>::iterator itr2 = allowed_methods.begin(); itr2 != allowed_methods.end(); itr2++)
	// 			std::cout << to_string(*itr2);
	// 		std::cout << std::endl;
	// 		std::cout << "\t\tautoindex: " << std::boolalpha << itr->second.isAutoIndexed() << std::endl;
	// 		std::cout << "\t\tuploadable: " << std::boolalpha << itr->second.isUploadable() << std::endl;
	// 		std::cout << "\t\tupload_dir: " << itr->second.getUploadDir() << std::endl;
	// 		std::cout << "\t\troot: " << itr->second.getRoot() << std::endl;
	// 		std::cout << "\t\tindex: " << itr->second.getIndex() << std::endl;
	// 	}
	// }
	return 0;
}
