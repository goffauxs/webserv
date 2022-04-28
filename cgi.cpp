//Execute cgi
#include "header.hpp"
#include "request.hpp"
#include "webserv.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void		fork_exec(std::string path, int fd[2], int fd_i[2], Request const &req, char **env)
{
	char	*av[3];

	av[0] = (char *)"python";
	av[1] = (char *)"server/cgi-bin/upload.py";/*path.c_str();*/
	av[2] = NULL;
	dup2(fd_i[0], 0);
	dup2(fd[1], 1);
	dup2(fd[0], 0);
	if (req.get_content())
		write(fd_i[0], req.get_content(), req.get_contentLength());
	close(fd_i[0]);
	close(fd_i[1]);
	close(fd[0]);
	close(fd[1]);

	execve("/usr/bin/python", av, env);
	perror("The error is :");
	exit(1);
}

std::string exec_cgi(std::string path, Request const &req)
{
	int			fd[2];
	int			fd_i[2];
	char		tmp[11] = {0};
	std::string	return_string = "";

	pipe(fd);
	pipe(fd_i);
	if (!fork())
	{
		char **env = vec_to_tab(create_env(req));
		fork_exec(path, fd, fd_i, req, env);
	}
	close(fd[1]);
	close(fd_i[0]);
	wait(0);
	int r = read(fd[0], tmp, 10);
	return_string += std::string(tmp);
	while (r > 0)
	{
		r = read(fd[0], tmp, 10);
		tmp[r] = 0;
		return_string += std::string(tmp);
	}
	return (return_string);
}
