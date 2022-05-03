#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <stack>
#include <list>
#include "utils.hpp"
#include "config.hpp"
#include "server_config.hpp"
#include "location_config.hpp"

int main()
{
	try
	{
		Config config("default.conf");
		ServerConfig* conf = config.getServerConfig("8000", "youpi");
		std::cout << "server root: " << conf->getRoot() << std::endl;
		try
		{
			LocationConfig* loc = conf->getLocation("/directory/upload.bla");
			std::cout << "cgi_ext: " << loc->getCgiExtenstion() << std::endl;
			std::cout << "root: " << loc->getRoot() << std::endl;
			std::cout << "index: " << loc->getIndex() << std::endl;
			std::cout << "uploadable?: " << std::boolalpha << loc->isUploadable() << std::endl;
			std::cout << "upload_dir: " << loc->getUploadDir() << std::endl;
			std::cout << "error 404: " << loc->getErrorPage(404) << std::endl;
			std::cout << "error 500: " << loc->getErrorPage(500) << std::endl;
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
	return 0;
}
