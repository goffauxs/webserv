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

std::vector<int>	setup_serv(int backlog, int numb_servs, int ports[3])
{
	// int 		sockets[numb_servs];
	std::vector<int>	sockets;
	for (int i = 0; i < numb_servs; i++)
		sockets.push_back(ports[i]);
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

	return (sockets);
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
	char *buff = new char[BUFFSIZE];
	std::string	response;

	memset(buff, '\0', BUFFSIZE);

	int	bytes_read = recv(client_fd, buff, BUFFSIZE, 0);
	check(bytes_read, "read error");
	std::cout << buff << std::endl;

	Request req(buff);
	std::vector<Header> headers = req.get_headers();
	/* prints headers */
	// switch (req.get_method())
	// {
	// 	case GET:
	// 		std::cout << to_string(req.get_method()) << " - " << req.get_resource() << " - " << to_string(req.get_version()) << std::endl;
	// 		for (std::vector<Header>::iterator it = headers.begin(); it != headers.end(); it++)
	// 			std::cout << "\t\"" << it->get_key() << "\": \"" << it->get_value() << "\"" << std::endl;
	// 		std::cout << std::endl;
	// 		break;
	// 	default:
	// 		break;
	// }
	// std::cout << "body = " << req.get_body() << std::endl;

	response = parse(req);

	//Send a message to the connection
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

	// int bytes_sent = send(client_fd, response.c_str(), response.size(), 0);

	close(client_fd);
}

void	run_serv(int numb_servs, std::vector<int> sockets)
{
	fd_set	reading_master, reading_sockets,/* writing_master, writing_sockets*/;
	int		client_fd;

	FD_ZERO(&reading_master); //zero out reading_master
	// FD_ZERO(&writing_master);
	for (int i = 0; i < numb_servs; i++)
		FD_SET(sockets[i], &reading_master); //add socket_fd to current set
	//reading_master = set of fds that we're watching
	//reading_sockets = tmp that will contain all the set of fds that are ready after we give it to select

	int	max_socket = sockets[numb_servs - 1]; //used to reduce the amount of times we're gonna go through the for loop

	struct timeval	tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	while (1)
	{
		reading_sockets = reading_master; //reading_sockets is a tmp because select is destructive

		//select(range of fd to check, set of fds to check for read, for write, for errors, timeout value)
		check(select(max_socket + 1, &reading_sockets, NULL, NULL, &tv), "Select error");

		for (int i = 0; i <= max_socket; i++)
		{
			if (FD_ISSET(i, &reading_sockets)) //check if fd i is ready
			{
				for (int j = 0; j < numb_servs; j++) //check on every port
				{
					if (i == sockets[j]) //this is a new connection
					{
						client_fd = accept_connection(sockets[j]);
						FD_SET(client_fd, &reading_master); //add new connection to set of fds we're watching
						if (client_fd > max_socket)
							max_socket = client_fd;
						break ;
					}
					else if (j == numb_servs - 1) //enter this if we tried every port
					{
						handle_connection(i);
						FD_CLR(i, &reading_master); //remove fd from set of fds we're watching
					}
				}
			}
		}
	}
}

int	main()
{
	int					numb_servs = 3; //get from config
	int					ports[3] = {7000, 8000, 9000}; //get from config
	std::vector<int>	sockets = setup_serv(1000, numb_servs, ports);
	run_serv(numb_servs, sockets);
	// int	client_fd;

	// fd_set	current_sockets, ready_sockets;

	// FD_ZERO(&current_sockets); //zero out current_sockets
	// FD_SET(socket_fd, &current_sockets); //add socket_fd to current set
	// //current_sockets = set of fds that we're watching
	// //ready_sockets = tmp that will contain all the set of fds that are ready after we give it to select

	// int	max_socket = socket_fd; //used to reduce the amount of times we're gonna go through the for loop
	// while (1)
	// {
	// 	//ready_sockets is a tmp because select is destructive
	// 	ready_sockets = current_sockets;

	// 	//select(range of fd to check, set of fds to check for read, for write, for errors, timeout value)
	// 	check(select(max_socket + 1, &ready_sockets, NULL, NULL, NULL), "Select error");

	// 	for (int i = 0; i <= max_socket; i++)
	// 	{
	// 		if (FD_ISSET(i, &ready_sockets)) //check if fd i is ready
	// 		{
	// 			if (i == socket_fd) //this is a new connection
	// 			{
	// 				client_fd = accept_connection(socket_fd);
	// 				FD_SET(client_fd, &current_sockets); //add new connection to set of fds we're watching
	// 				if (client_fd > max_socket)
	// 					max_socket = client_fd;
	// 			}
	// 			else
	// 			{
	// 				handle_connection(i);
	// 				FD_CLR(i, &current_sockets); //remove fd from set of fds we're watching
	// 			}
	// 		}
	// 	}
	// }
}
