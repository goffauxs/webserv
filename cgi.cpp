//Execute cgi
#include "header.hpp"
#include "request.hpp"
#include "webserv.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "location_config.hpp"

void		fork_exec(std::string path, int fd_in[2],int fd_out[2], Request const &req, char **env)
{
	char	*av[3];

	av[0] = (char *)"python";
	av[1] = (char *)/*"server/cgi-bin/upload.py";*/path.c_str();
	av[2] = NULL;
	dup2(fd_in[0], 0);
	dup2(fd_out[1], 1);
	if (req.get_content())
		write(fd_in[1], req.get_content(), req.get_contentLength());
	close(fd_out[0]);
	close(fd_out[1]);
	close(fd_in[1]);
	close(fd_in[0]);
	execve("/usr/bin/python", av, env);
	perror("The error is :");
	exit(1);
}

std::string exec_cgi(std::string path, Request const &req, LocationConfig conf)
{
	int			fd_in[2];
	int			fd_out[2];
	char		tmp[11] = {0};
	std::string	return_string = "";

	pipe(fd_in);
	pipe(fd_out);
	if (!fork())
	{
		char **env = vec_to_tab(create_env(req, conf));
		fork_exec(path, fd_in,fd_out, req, env);
	}
	close(fd_in[1]);
	close(fd_in[0]);
	close(fd_out[1]);
	wait(0);
	int r = read(fd_out[0], tmp, 10);
	return_string += std::string(tmp);
	while (r > 0)
	{
		r = read(fd_out[0], tmp, 10);
		tmp[r] = 0;
		return_string += std::string(tmp);
	}
	return (return_string);
}
