#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "header.hpp"
#include "request.hpp"
#include "webserv.hpp"
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <map>
#include <set>
#include <list>
#include "config.hpp"

#define PORT 8000
#define	BUFFSIZE 300000

int	check(int val, std::string msg)
{
	if (val == -1)
	{
		std::cout << msg << std::endl;
		exit(1);
	}
	return val;
}

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
		//start listening
		check(bind(*ret.first, (struct sockaddr*)&sockaddr, sizeof(sockaddr)),
			"Failed to bind to port " + to_string((*it)->getPort()));
		check(listen(*ret.first, backlog), "Failed to listen on socket");
	}

	return (sock_set);
}

int	accept_connection(int socket_fd)
{
	int	client_fd;
	int	addrlen = sizeof(sockaddr_in);
	sockaddr_in	client_addr;

	check(client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen),
		"Failed to grab connection");
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	// std::cout << "address = " << inet_ntoa(client_addr.sin_addr) << std::endl;

	return (client_fd);
}

int	read_connection(int client_fd, std::map<int, std::string>& requests)
{
	char buff[BUFFSIZE] = {0};

	int ret = recv(client_fd, buff, BUFFSIZE, 0);

	if (ret == 0 || ret == -1)
	{
		close(client_fd);
		requests.erase(client_fd);
		if (!ret)
			std::cerr << "\rConnection was closed by client." << std::endl;
		else
			std::cerr << "\rRead error, closing connection." << std::endl;
		return -1;
	}

	requests[client_fd] += std::string(buff);

	size_t i = requests[client_fd].find("\r\n\r\n");
	if (i != std::string::npos)
	{
		if (requests[client_fd].find("Content-Length: ") == std::string::npos)
			return 0;
		
		size_t len = std::atoi(requests[client_fd].substr(requests[client_fd].find("Content-Length: ") + 16, 10).c_str());
		if (requests[client_fd].size() >= len + i + 4)
			return 0;
		else
			return 1;
	}
	return 1;
}

int write_connection(int client_fd, std::map<int, std::string>& requests)
{
	static std::map<int, size_t> sent;

	if (!sent.count(client_fd))
		sent[client_fd] = 0;
	
	std::string str = requests[client_fd].substr(sent[client_fd], BUFFSIZE);
	int ret = send(client_fd, str.c_str(), str.size(), 0);

	if (ret == -1)
	{
		close(client_fd);
		requests.erase(client_fd);
		sent[client_fd] = 0;
		return -1;
	}
	else
	{
		sent[client_fd] += ret;
		if (sent[client_fd] >= requests[client_fd].size())
		{
			requests.erase(client_fd);
			sent[client_fd] = 0;
			return 0;
		}
		else
			return 1;
	}
}

void	handle_connection(int client_fd)
{
	char buff[BUFFSIZE] = {0};
	std::string	response;

	memset(buff, '\0', BUFFSIZE);

	int	bytes_read = recv(client_fd, buff, BUFFSIZE, 0);
	check(bytes_read, "read error");
	std::cout << buff << std::endl;

	Request req(buff);

	response = parse(req);

	int len = response.size();
	int sent = 0;
	while (len > 0)
	{
		int bytes_sent = send(client_fd, response.c_str() + sent, len, 0);

		if (bytes_sent == 0)
			break;
		if (bytes_sent > 0)
		{
			len -= bytes_sent;
			sent += bytes_sent;
		}
	}

	close(client_fd);
}

void	run_serv(std::set<int> servers)
{
	fd_set	master, read_fds, write_fds;
	// int		client_fd;
	std::map<int, std::string> requests;
	std::set<int> clients;
	std::vector<int> ready;

	FD_ZERO(&master); //zero out master
	for (std::set<int>::iterator it = servers.begin(); it != servers.end(); it++)
		FD_SET(*it, &master); //add socket_fd to current set
	//master = set of fds that we're watching
	//read_fds = tmp that will contain all the set of fds that are ready after we give it to select

	int	fd_max = *servers.rbegin(); //used to reduce the amount of times we're gonna go through the for loop

	struct timeval	tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	while (1)
	{
		int ret = 0;
		read_fds = master; //read_fds is a tmp because select is destructive

		FD_ZERO(&write_fds);
		for (std::vector<int>::iterator it = ready.begin(); it != ready.end(); it++)
			FD_SET(*it, &write_fds);

		//select(range of fd to check, set of fds to check for read, for write, for errors, timeout value)
		ret = check(select(fd_max + 1, &read_fds, &write_fds, NULL, &tv), "Select error");

		for (std::vector<int>::iterator it = ready.begin(); ret && it != ready.end(); it++)
		{
			if (FD_ISSET(*it, &write_fds))
			{
				int ret = write_connection(*it, requests);

				if (ret == 0)
					ready.erase(it);
				else if (ret == -1)
				{
					FD_CLR(*it, &master);
					FD_CLR(*it, &read_fds);
					clients.erase(*it);
					ready.erase(it);
				}
				ret = 0;
				break;
			}
		}

		for (std::set<int>::iterator it = clients.begin(); ret && it != clients.end(); it++)
		{
			if (FD_ISSET(*it, &read_fds))
			{
				int ret = read_connection(*it, requests);
				// std::cout << "ret = " << ret << std::endl;

				if (ret == 0)
				{
					Request req(requests[*it].c_str());
					// std::cout << to_string(req.get_method()) << std::endl;

					requests[*it] = parse(req);
					ready.push_back(*it);
				}
				else if (ret == -1)
				{
					FD_CLR(*it, &master);
					FD_CLR(*it, &read_fds);
					clients.erase(*it);
				}
				ret = 0;
				break;
			}
		}

		for (std::set<int>::iterator it = servers.begin(); ret && it != servers.end(); it++)
		{
			if (FD_ISSET(*it, &read_fds))
			{
				int socket = accept_connection(*it);

				if (socket != -1)
				{
					FD_SET(socket, &master);
					clients.insert(socket);
					if (socket > fd_max)
						fd_max = socket;
				}
				ret = 0;
				break;
			}
		}

		// for (int i = 0; i <= fd_max; i++)
		// {
		// 	if (FD_ISSET(i, &read_fds)) //check if fd i is ready
		// 	{
		// 		if (sockets.count(i))
		// 		{
		// 			client_fd = accept_connection(i);
		// 			FD_SET(client_fd, &master); //add new connection to set of fds we're watching
		// 			if (client_fd > fd_max)
		// 				fd_max = client_fd;
		// 			break ;
		// 		}
		// 		else
		// 		{
		// 			handle_connection(i);
		// 			FD_CLR(i, &master);
		// 		}
		// 	}
		// }
	}
}

int	main()
{
	Config				conf("default.conf");
	std::set<int>		sockets = setup_serv(1000, conf);
	run_serv(sockets);
}
