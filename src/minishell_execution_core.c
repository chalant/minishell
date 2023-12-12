#include "minishell.h"

// WIFSIGNALED isn't true even for 'ctrl-c' and 'ctrl-\'
int	get_exit_status(pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 2);
	if (WIFSIGNALED(status))
		return (WTERMSIG(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	redirect_in(t_command *command)
{
	if (dup2(command->input, STDIN_FILENO) < 0)
		return (ms_perror("dup", NULL, NULL, errno) - 2);
	return (0);
}

int	redirect_out(t_command *command)
{
	if (dup2(command->output, STDOUT_FILENO) < 0)
		return (ms_perror("dup", NULL, NULL, errno) - 2);
	return (1);
}

int	pipe_out(t_command *command, int pipe_[2])
{
	if (command->output > 0)
		return (redirect_out(command));
	else if (command->output < 0)
		return (-1);
	else if (pipe_[1] == -1)
		return (0);
	else if (dup2(pipe_[1], STDOUT_FILENO) < 0)
		return (ms_perror("dup out", NULL, NULL, errno) - 2);
	return (0);
}

int	pipe_in(t_command *command, int pipe_[2])
{
	if (command->input > 0)
		return (redirect_in(command));
	else if (command->input < 0)
		return (-1);
	else if (pipe_[0] == -1)
		return (0);
	else if (dup2(pipe_[0], STDIN_FILENO) < 0)
		return (ms_perror("dup in", NULL, NULL, errno) - 2);
	return (0);
}

int	pipe_io(t_command *command, int in_pipe[2], int out_pipe[2])
{
	if (pipe_in(command, in_pipe) < 0)
		return (-1);
	if (pipe_out(command, out_pipe) < 0)
		return (-1);
	return (0);
}

static void	fill_in_status(char *status, t_token *token, int *j)
{
	if (!token->string[*j])
		return ;
	token->string[*j] = *status;
	(*j)++;
	status++;
	if (*status)
	{
		token->string[*j] = *status;
		(*j)++;
		status++;
	}
	else
		ms_shift_strings(token->string, token->mask_exp, *j);
	if (*status)
	{
		token->string[*j] = *status;
		(*j)++;
		status++;
	}
	else
		ms_shift_strings(token->string, token->mask_exp, *j);
}

static char *make_argi(t_command *command, int i)
{
	t_token	*token;
	int		j;
	char	*status;

	token = ft_darray_get(command->arguments, i - 1);
	if (token->flags & IS_SPECIAL)
	{
		status = ft_itoa((unsigned char) command->context->status);
		if (!status)
			return (NULL);
		j = 0;
		while (token->string[j])
		{
			while (token->string[j] && token->mask_exp[j] != '3')
				j++;
			fill_in_status(status, token, &j);
		}
		free(status);
	}
	return (token->string);
}

// returns NULL on malloc fail
char	**make_arguments(t_command *command)
{
	int			nargs;
	int			i;
	char		**arguments;

	nargs = 2;
	if (command->arguments)
		nargs = command->arguments->size + 2;
	arguments = malloc(sizeof(char *) * nargs);
	if (!arguments)
		return (NULL);
	arguments[0] = command->command_name;
	i = 0;
	while (++i < nargs - 1)
	{
		arguments[i] = make_argi(command, i);
		if (!arguments[i])
		{
			ft_clear_ds(arguments);
			return (NULL);
		}
	}
	arguments[i] = NULL;
	return (arguments);
}

int	execute_builtin(t_command *command, int in_fd, int out_fd)
{
	int		status;
	char	**arguments;
	(void)in_fd;

	status = 0;
	arguments = make_arguments(command);
	if (strcmp(command->command_name, "echo") == 0)
		status = ms_echo(arguments, out_fd);
	else if (strcmp(command->command_name, "cd") == 0)
		status = ms_cd(command->context, arguments, out_fd);
	else if (strcmp(command->command_name, "pwd") == 0)
		status = ms_pwd(out_fd);
	else if (strcmp(command->command_name, "export") == 0)
		status = ms_export(command->context, arguments);
	else if (strcmp(command->command_name, "env") == 0)
		status = ms_env();
	else if (strcmp(command->command_name, "unset") == 0)
		status = ms_unset(command->context, arguments);
	else if (strcmp(command->command_name, "exit") == 0)
		status = ms_exit(command->context, arguments);
	free(arguments);
	return (status);
}

//creates arguments in child process to avoid unnecessary copies.
int	launch_execve(t_command *command)
{
	extern char	**environ;
	char		**arguments;

	arguments = make_arguments(command);
	//todo: display error
	if (!arguments)
		exit(1);
	execve(command->command_name, arguments, environ);
	free(arguments);
	ms_perror("execve", NULL, NULL, errno);
	exit(127);
}

int	execute_simple_command(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;
	int		status;
	(void)parent;

	//not in a processs
	if (command->command_flags & MS_BUILTIN && !(command->command_flags & MS_FORKED))
	{
		if (command->output)
			return (execute_builtin(command, command->input, command->output));
		else
			return (execute_builtin(command, command->input, STDOUT_FILENO));
	}
	else if (command->command_flags & MS_BUILTIN)
	{
		if (command->input)
			redirect_in(command);
		if (command->output)
			redirect_out(command);
		return (execute_builtin(command, STDIN_FILENO, STDOUT_FILENO));
	}
	if (!(command->command_flags & MS_FORKED))
	{
		//todo: errors
		if (command->command_flags & MS_LAST)
		{
			close(out_pipe[1]);
			out_pipe[1] = -1;
		}
		pid = fork();
		if (pid < 0)
			return (ms_perror("fork", NULL, NULL, errno));
		if (pid == 0)
		{
			if (pipe_io(command, in_pipe, out_pipe) < 0)
				exit(1);
			launch_execve(command);
		}
		if (out_pipe[1] != -1)
			close(out_pipe[1]);
		//close(in_pipe[0]);
		// in_pipe[0] = -1;
		out_pipe[1] = -1;
		status = get_exit_status(pid);
		return (status);
	}
	launch_execve(command);
	return (1);
}

pid_t	execute_process(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (ms_perror("fork", NULL, NULL, errno) - 2);
	command->command_flags |= MS_FORKED;
	if (pid == 0)
	{
		if (pipe_io(command, in_pipe, out_pipe) < 0)
			exit(1);
		status = execute_simple_command(parent, command, in_pipe, out_pipe);
		if (command->command_flags & MS_BUILTIN)
			exit(status);
		exit(126);
	}
	close(out_pipe[1]);
	close(in_pipe[0]);
	in_pipe[0] = -1;
	out_pipe[1] = -1;
	// close(in_pipe[0]);
	// close(in_pipe[1]);
	return (pid);
}

int	execute_and_wait(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;

	if (parent && parent->command_flags & MS_PIPE)
	{
		if (command->command_flags & MS_LAST)
		{
			if (out_pipe[1] != -1)
				close(out_pipe[1]);
			out_pipe[1] = -1;
		}
		pid = execute_process(parent, command, in_pipe, out_pipe);
		if (pid < 0)
			return (1);
		//todo: this is the wrong status!
		return (get_exit_status(pid));
	}
	return (execute_simple_command(parent,command, in_pipe, out_pipe));
}


//todo: make it so that we only
int	execute_command_core(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;

	if (parent && parent->command_flags & MS_PIPE)
	{
		if (command->command_flags & MS_LAST)
		{
			if (out_pipe[1] != -1)
				close(out_pipe[1]);
			out_pipe[1] = -1;
		}
		pid = execute_process(parent, command, in_pipe, out_pipe);
		if (pid < 0)
			return (1);
		if (command->command_flags & MS_LAST)
			return (get_exit_status(pid));
		//todo: this is the wrong status!
		return (0);
	}
	return (execute_simple_command(parent,command, in_pipe, out_pipe));
}
