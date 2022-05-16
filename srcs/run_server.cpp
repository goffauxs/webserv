#include "webserv.hpp"

int write_connection(int client_fd, std::map<int, std::vector<char> >& requests)
{
	static std::map<int, size_t> sent;

	if (!sent.count(client_fd))
		sent[client_fd] = 0;
	
	size_t size = std::min((unsigned long)BUFFSIZE, requests[client_fd].size() - sent[client_fd]);
	int ret = send(client_fd, &requests[client_fd][sent[client_fd]], size, 0);

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

int	read_connection(int client_fd, std::map<int, std::vector<char> >& requests)
{
	char buff[BUFFSIZE + 1] = {0};

	int ret = recv(client_fd, buff, BUFFSIZE, 0);

	if (ret == 0 || ret == -1)
	{
		close(client_fd);
		requests.erase(client_fd);
		// if (!ret)
		// 	std::cerr << "\rConnection was closed by client." << std::endl;
		// else
		// 	std::cerr << "\rRead error, closing connection." << std::endl;
		return -1;
	}

	for (int i = 0; i < ret; i++)
		requests[client_fd].push_back(buff[i]);

	size_t i = vector_find(requests[client_fd], "\r\n\r\n") - requests[client_fd].begin();
	if (i != requests[client_fd].size())
	{
		if (vector_find(requests[client_fd], "Content-Length: ") == requests[client_fd].end())
			return 0;
		
		std::string tmp(vector_find(requests[client_fd], "Content-Length: ") + 16, vector_find(requests[client_fd], "Content-Length: ") + 26);
		size_t len = std::atoi(tmp.c_str());
		if (requests[client_fd].size() >= len + i + 4)
			return 0;
		else
			return 1;
	}
	return 1;
}

int	accept_connection(int socket_fd)
{
	int	client_fd;
	int	addrlen = sizeof(sockaddr_in);
	sockaddr_in	client_addr;

	check(client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen),
		"Failed to grab connection");
	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	return (client_fd);
}

void	run_serv(std::set<int> servers, Config& conf)
{
	fd_set	master, read_fds, write_fds; // master = all clients we're watching
	std::map<int, std::vector<char> > requests; // full request for each client
	std::set<int> clients; // client fds
	std::vector<int> ready; // clients that have been read completely and that can receive a response

	FD_ZERO(&master); // zero out master
	for (std::set<int>::iterator it = servers.begin(); it != servers.end(); it++)
		FD_SET(*it, &master); // add socket_fd to current set

	int	fd_max = *servers.rbegin(); // used for the range in select

	struct timeval	tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	while (1)
	{
		int ret = 0;
		read_fds = master; // read_fds is a tmp because select is destructive

		FD_ZERO(&write_fds);
		for (std::vector<int>::iterator it = ready.begin(); it != ready.end(); it++)
			FD_SET(*it, &write_fds);

		// select(range of fd to check, set of fds to check for read, for write, for errors, timeout value)
		ret = check(select(fd_max + 1, &read_fds, &write_fds, NULL, &tv), "Select error");

		// goes through every ready client and writes one if it's in the write set filled by select
		for (std::vector<int>::iterator it = ready.begin(); ret && it != ready.end(); it++)
		{
			if (FD_ISSET(*it, &write_fds))
			{
				int ret2 = write_connection(*it, requests);

				if (ret2 == 0)
					ready.erase(it);
				else if (ret2 == -1)
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

		// goes through every client and reads one if it's in the read set filled by select
		for (std::set<int>::iterator it = clients.begin(); ret && it != clients.end(); it++)
		{
			if (FD_ISSET(*it, &read_fds))
			{
				int ret2 = read_connection(*it, requests);

				if (ret2 == 0)
				{
					requests[*it].push_back('\0');
					char* tmp = reinterpret_cast<char*>(&requests[*it][0]);
					Request req(tmp, conf);

					std::string str = parse(req);
					std::vector<char> vec(str.begin(), str.end());
					requests[*it] = vec;
					ready.push_back(*it);
				}
				else if (ret2 == -1)
				{
					FD_CLR(*it, &master);
					FD_CLR(*it, &read_fds);
					clients.erase(*it);
				}
				ret = 0;
				break;
			}
		}

		// goes through every listener (server fd) and, if it's in the read set filled by select, accepts a new connection
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
	}
}
