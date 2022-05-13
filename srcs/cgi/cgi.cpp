#include "webserv.hpp"

void		fork_exec(std::string path, int fd_in[2],int fd_out[2], Request const &req, char **env)
{
	char	*av[3];
	(void)req;

	av[0] = (char *)"python";
	av[1] = (char *)path.c_str();
	av[2] = NULL;
	dup2(fd_in[0], 0);
	dup2(fd_out[1], 1);
	close(fd_out[0]);
	close(fd_out[1]);
	close(fd_in[1]);
	close(fd_in[0]);
	execve("/usr/bin/python2.7", av, env);
	perror("The error is :");
	exit(1);
}

std::string exec_cgi(std::string path, Request const &req, const LocationConfig& conf)
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
	dup2(fd_in[0], 0);
	if (req.get_content())
		write(fd_in[1], req.get_content(), req.get_content_length());
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
