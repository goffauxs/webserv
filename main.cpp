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
#include <string.h>
#include <fstream>
#include <map>
#include <stack>
#include <list>
#include "utils.hpp"
#include "config.hpp"
#include "server_config.hpp"
#include "location_config.hpp"

#define PORT 8007
#define	BUFFSIZE 300000

void	check(int val, std::string msg)
{
	if (val == -1)
	{
		std::cout << msg << std::endl;
		exit(1);
	}
}

int	setup_serv(int backlog)
{
	int socket_fd;

	//create socket with IPV4, TCP
	check((socket_fd = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket");

	//initalize the address struct
	sockaddr_in sockaddr; //https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY; //inaddr_any because any address
	sockaddr.sin_port = htons(PORT);

	//start listening
	check(bind(socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)),
		"Failed to bind to port " + to_string(PORT));
	check(listen(socket_fd, backlog), "Failed to listen on socket");

	return (socket_fd);
}

int	accept_connection(int socket_fd)
{
	int	client_fd;
	int	addrlen = sizeof(sockaddr_in);
	sockaddr_in	client_addr;

	check(client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen),
		"Failed to grab connection");
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
	send(client_fd, response.c_str(), response.size(), 0);

	close(client_fd);
}

int	main()
{
	int	backlog = 100;
	int	socket_fd = setup_serv(backlog);
	int	client_fd;

	fd_set	current_sockets, ready_sockets;

	FD_ZERO(&current_sockets); //zero out current_sockets
	FD_SET(socket_fd, &current_sockets); //add socket_fd to current set
	//current_sockets = set of fds that we're watching
	//ready_sockets = tmp that will contain all the set of fds that are ready after we give it to select

	int	max_socket = socket_fd; //used to reduce the amount of times we're gonna go through the for loop
	while (1)
	{
		//ready_sockets is a tmp because select is destructive
		ready_sockets = current_sockets;

		//select(range of fd to check, set of fds to check for read, for write, for errors, timeout value)
		check(select(max_socket + 1, &ready_sockets, NULL, NULL, NULL), "Select error");

		for (int i = 0; i <= max_socket; i++)
		{
			if (FD_ISSET(i, &ready_sockets)) //check if fd i is ready
			{
				if (i == socket_fd) //this is a new connection
				{
					client_fd = accept_connection(socket_fd);
					FD_SET(client_fd, &current_sockets); //add new connection to set of fds we're watching
					if (client_fd > max_socket)
						max_socket = client_fd;
				}
				else
				{
					handle_connection(i);
					FD_CLR(i, &current_sockets); //remove fd from set of fds we're watching
				}
			}
		}
	}
	return 0;
}
