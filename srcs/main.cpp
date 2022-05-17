#include "webserv.hpp"

int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		try
		{
			Config				conf(argv[1]);
			std::set<int>		servers = setup_serv(1000, conf); // listener (server) fds
			run_serv(servers, conf);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return (1);
		}
	}
	std::cerr << "Error: Please provide a configuration file" << std::endl;
	return (1);
}
