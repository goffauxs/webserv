//Execute cgi
#include "header.hpp"
#include "request.hpp"
#include "webserv.hpp"
#include <iostream>
#include <unistd.h>
std::vector<Header>::iterator	find_header(std::vector<Header> vec, std::string key);

void		fork_exec(std::string path,int fd[2], int fd_i[2], std::string body, char **env)
{
	char	*av[3];

	av[0] = (char *)"python";
	av[1] = (char *)"server/cgi-bin/test.py";//&path[0];
	av[2] = NULL;
	int i = 0;
	while (env[i])
	{
		std::cout << "env[" << i << "] = " << env[i] << std::endl;
		i++;
	}
	dup2(fd_i[0], 0);
	dup2(fd[1], 1);
	dup2(fd[0], 0);
	write(1, body.c_str(), body.size() + 1);
	close(fd_i[0]);
	close(fd_i[1]);
	close(fd[0]);
	close(fd[1]);

	execve("/usr/bin/python", av, env);
	perror("execve error:");
	exit(1);
}

struct timeval	zero_time()
{
	struct timeval	ret;

	ret.tv_sec = 0;
	ret.tv_usec = 0;
	return (ret);
}

// std::string	read_cgi(int fd)
// {
// 	char		tmp[11];
// 	std::string	return_value = "";
// 	fd_set	sockets;
// 	struct timeval	timer = zero_time();
	
// 	FD_ZERO(&sockets);
// 	FD_SET(fd, &sockets);
// 	int r = read(fd, tmp, 10);
// 	tmp[r] = '\0';
// 	std::cout << "tmp 0 = " << tmp << std::endl;
// 	while (r == 10)
// 	{
// 		return_value += std::string(tmp);
// 		int se = select(fd + 1, &sockets, NULL, NULL, &timer);
// 		timer = zero_time();
// 		if (se > 0)
// 			r = read(fd, tmp, 10);
// 		else
// 			r = -1;
// 		std::cout << "tmp = " << tmp << std::endl;
// 		tmp[r] = 0;
// 	}
// 	if (r != -1)
// 		return_value += std::string(tmp);
// 	return (return_value);
// }

// std::string exec_cgi(std::string path, Request const &req) //Check if body is ready to send
// {
// 	int			fd[2];
// 	int			fd_i[2];
// 	std::string	return_string = "";

// 	pipe(fd);
// 	pipe(fd_i);
// 	int pid = fork();
// 	if (!pid)
// 	{
// 		char **env = vec_to_tab(create_env(req));
// 		fork_exec(path, fd, fd_i, req.get_body(), env);		
// 	}
// 	close(fd[1]);
// 	close(fd_i[0]);
// 	wait(0);
// 	return_string = read_cgi(fd[0]);
// 	std::cout << "Return of the script :" << return_string << std::endl;
// 	return (return_string);
// }

std::string exec_cgi(std::string path, Request const &req) //Check if body is ready to send
{
	int			fd[2];
	int			fd_i[2];
	char		tmp[11];
	
	std::string	return_string = "";
	fd_set	sockets;
	struct timeval	timer = zero_time();
	memset(tmp, 0, 11);
	FD_ZERO(&sockets);
	pipe(fd);
	pipe(fd_i);
	if (!fork())
	{
		char **env = vec_to_tab(create_env(req));
		fork_exec(path, fd, fd_i, req.get_body(), env);		
	}
	close(fd[1]);
	close(fd_i[0]);
	FD_SET(fd[0], &sockets);
	wait(0);
	int r = read(fd[0], tmp, 1);
	read(fd[0], tmp, 10);
	for (int i = 0; i < 10; i++)
	{
		std::cout << +tmp[i] << " ";
	}
	std::cout << std::endl;
	while (r == 10)
	{
		return_string += std::string(tmp);
		int se = select(fd[0] + 1, &sockets, NULL, NULL, &timer);
		timer = zero_time();
		if (se > 0)
			r = read(fd[0], tmp, 10);
		else
			r = -1;
		tmp[r] = 0;
	}
	if (r != -1)
		return_string += std::string(tmp);
	std::cout << "Return of the script " << return_string << std::endl;
	return (return_string);
}
