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
#include "utils.hpp"
#include "webserv.hpp"
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <map>
#include <set>

#define PORT 8000
#define	BUFFSIZE 300000

void	check(int val, std::string msg)
{
	if (val == -1)
	{
		std::cout << msg << std::endl;
		exit(1);
	}
}

std::set<int>	setup_serv(int backlog, int numb_servs, int ports[3])
{
	// int 		sockets[numb_servs];
	std::vector<int>	sockets(numb_servs);
	std::set<int>		sock_set;
	sockaddr_in	sockaddr[numb_servs]; //https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
	int			optval = 1;

	for (int i = 0; i < numb_servs; i++)
	{
		//create socket with IPV4, TCP
		check((sockets[i] = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket");
		//make sockets[i] non-blocking
		check(fcntl(sockets[i], F_SETFL, O_NONBLOCK), "Fcntl error");
		//initalize the address struct
		sockaddr[i].sin_family = AF_INET;
		sockaddr[i].sin_addr.s_addr = INADDR_ANY;
		sockaddr[i].sin_port = htons(ports[i]);
		//change socket option to avoid "Failed to bind" error
		check(setsockopt(sockets[i], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "Setsockopt error");
		//start listening
		check(bind(sockets[i], (struct sockaddr*)&sockaddr[i], sizeof(sockaddr[i])),
			"Failed to bind to port " + to_string(ports[i]));
		check(listen(sockets[i], backlog), "Failed to listen on socket");
	}

	for (std::vector<int>::iterator it = sockets.begin(); it != sockets.end(); it++)
		sock_set.insert(*it);

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

void	run_serv(std::set<int> sockets)
{
	fd_set	master, read_fds;
	int		client_fd;

	FD_ZERO(&master); //zero out master
	for (std::set<int>::iterator it = sockets.begin(); it != sockets.end(); it++)
		FD_SET(*it, &master); //add socket_fd to current set
	//master = set of fds that we're watching
	//read_fds = tmp that will contain all the set of fds that are ready after we give it to select

	int	fd_max = *sockets.rbegin(); //used to reduce the amount of times we're gonna go through the for loop

	struct timeval	tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	while (1)
	{
		read_fds = master; //read_fds is a tmp because select is destructive

		//select(range of fd to check, set of fds to check for read, for write, for errors, timeout value)
		check(select(fd_max + 1, &read_fds, NULL, NULL, &tv), "Select error");

		for (int i = 0; i <= fd_max; i++)
		{
			if (FD_ISSET(i, &read_fds)) //check if fd i is ready
			{
				if (sockets.count(i))
				{
					client_fd = accept_connection(i);
					FD_SET(client_fd, &master); //add new connection to set of fds we're watching
					if (client_fd > fd_max)
						fd_max = client_fd;
					break ;
				}
				else
				{
					handle_connection(i);
					FD_CLR(i, &master);
				}
			}
		}
	}
}

int	main()
{
	const int			numb_servs = 3; //get from config
	int					ports[numb_servs] = {7000, 8000, 9000}; //get from config
	std::set<int>		sockets = setup_serv(1000, numb_servs, ports);
	run_serv(sockets);
}
