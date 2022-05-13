#include "webserv.hpp"

std::set<int>	setup_serv(int backlog, const Config& conf)
{
	std::list<ServerConfig*>	list = conf.getServerList();
	std::set<int>				sock_set;
	int							optval = 1;

	for (std::list<ServerConfig*>::iterator it = list.begin(); it != list.end(); it++)
	{
		sockaddr_in sockaddr;

		//create socket with IPV4, TCP
		std::pair<std::set<int>::iterator, bool> ret;
		ret = sock_set.insert(check(socket(AF_INET, SOCK_STREAM, 0), "Failed to create socket"));

		//make sockets[i] non-blocking
		check(fcntl(*ret.first, F_SETFL, O_NONBLOCK), "Fcntl error");

		//initalize the address struct
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = INADDR_ANY;
		sockaddr.sin_port = htons((*it)->getPort());

		//change socket option to avoid "Failed to bind" error
		check(setsockopt(*ret.first, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "Setsockopt error");

		//bind and start listening
		check(bind(*ret.first, (struct sockaddr*)&sockaddr, sizeof(sockaddr)),
			"Failed to bind to port " + to_string((*it)->getPort()));
		check(listen(*ret.first, backlog), "Failed to listen on socket");
	}

	return (sock_set);
}
