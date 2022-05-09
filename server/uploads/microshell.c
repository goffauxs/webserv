#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_cmd
{
	char			**args;
	int				pipe;
	int				fd[2];
	struct s_cmd	*next;
}	t_cmd;

int	ft_strlen(char *str)
{
	int	i = 0;

	while (str[i])
		i++;
	return (i);
}

void	print_err(char *str)
{
	write(2, str, ft_strlen(str));
}

void	ft_quit(void)
{
	print_err("error: fatal\n");
	exit(1);
}

t_cmd	*new_node(char **argv, int *i)
{
	int	j = 0;
	t_cmd	*ret = malloc(sizeof(t_cmd));

	if (!ret)
		ft_quit();
	if (!(ret->args = malloc(sizeof(char *) *200)))
		ft_quit();
	ret->pipe = 0;
	ret->fd[0] = 0;
	ret->fd[1] = 0;
	ret->next = NULL;
	while (argv[*i] && strcmp(argv[*i], ";") && strcmp(argv[*i], "|"))
		ret->args[j++] = argv[(*i)++];
	ret->args[j] = 0;
	if (argv[*i] && !strcmp(argv[*i], "|"))
		ret->pipe = 1;
	return (ret);
}

int	cd(char **args)
{
	if (args[1] == 0 || args[2] != 0)
	{
		print_err("error: cd: bad arguments\n");
		return (1);
	}
	if (chdir(args[1]))
	{
		print_err("error: cd: cannot change directory to ");
		print_err(args[1]);
		print_err("\n");
		return (1);
	}
	return (0);
}

int	exec(t_cmd *cmd, char **env)
{
	int	pid;

	if (!strcmp(cmd->args[0], "cd"))
		return (cd(cmd->args));
	if ((pid = fork()) == -1)
		ft_quit();
	if (pid == 0)
	{
		if (cmd->fd[1])
			dup2(cmd->fd[1], 1);
		if (cmd->fd[0])
			dup2(cmd->fd[0], 0);
		execve(cmd->args[0], cmd->args, env);
		print_err("error: cannot execute ");
		print_err(cmd->args[0]);
		print_err("\n");
		exit(1);
	}
	waitpid(pid, 0, 0);
	if (cmd->fd[1])
		close(cmd->fd[1]);
	if (cmd->fd[0])
		close(cmd->fd[0]);
	return (0);
}

void	do_stuff(t_cmd *cmds, char **env)
{
	int	pipefd[2];
	t_cmd	*tmp = cmds;

	while (tmp)
	{
		if (tmp->pipe)
		{
			if (pipe(pipefd) == -1)
				ft_quit();
			tmp->fd[1] = pipefd[1];
			tmp->next->fd[0] = pipefd[0];
		}
		exec(tmp, env);
		tmp = tmp->next;
	}
}

void	free_stuff(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		free(cmds->args);
		free(cmds);
		cmds = tmp;
	}
}

int	main(int argc, char **argv, char **env)
{
	int	i = 0;
	t_cmd	*cmds = NULL;
	t_cmd	*tmp;

	while (++i < argc)
	{
		if (!strcmp(argv[i], ";"))
			continue;
		if (!cmds)
		{
			cmds = new_node(argv, &i);
			tmp = cmds;
		}
		else
		{
			tmp->next = new_node(argv, &i);
			tmp = tmp->next;
		}
	}
	do_stuff(cmds, env);
	free_stuff(cmds);
	return (0);
}
