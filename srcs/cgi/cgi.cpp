#include "webserv.hpp"
#include <sys/types.h>
#include <sys/wait.h>

void		fork_exec(std::string path, int fd_in[2],int fd_out[2], Request const &req, char **env)
{
	char	*av[3];
	// (void)req;

	std::string tmp = req.get_resource().substr(req.get_resource().find('.'));
	if (tmp.find('?') != std::string::npos)
		tmp.erase(tmp.find('?'));
	av[0] = (char *)req.cgi_exec[tmp].c_str();
	av[1] = (char *)path.c_str();
	av[2] = NULL;
	dup2(fd_in[0], 0);
	dup2(fd_out[1], 1);
	close(fd_out[0]);
	close(fd_out[1]);
	close(fd_in[1]);
	close(fd_in[0]);
	execve(av[0], av, env);
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
		char **env = vec_to_tab(create_env(path, req, conf));
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
