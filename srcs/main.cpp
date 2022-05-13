#include "webserv.hpp"

int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		Config				conf(argv[1]);
		std::set<int>		servers = setup_serv(1000, conf); // listener (server) fds
		run_serv(servers, conf);
	}
	std::cout << "Error: Please provide a configuration file" << std::endl;
	return (1);
}
