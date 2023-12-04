/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:02:24 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/04 17:13:05 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// WIFSIGNALED isn't true even for 'ctrl-c' and 'ctrl-\'
static int	get_exit_status(pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 2);
	while (wait(NULL) != -1)
		continue ;
	if (g_global_status)
	{
		status = g_global_status;
		g_global_status = 0;
		return (status);
	}
	if (WIFSIGNALED(status))
		return (WTERMSIG(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

//copies file descriptors
void	copy_pipe(int src_pipe[2], int dest_pipe[2])
{
	dest_pipe[0] = src_pipe[0];
	dest_pipe[1] = src_pipe[1];
}

int	redirect_in(t_command *command)
{
	// // todo: dup file errors
	if (dup2(command->input, STDIN_FILENO) < 0)
		return (-1);
	return (0);
}

int	redirect_out(t_command *command)
{
	if (dup2(command->output, STDOUT_FILENO) < 0)
		return (-1);
	return (0);
}

int	pipe_out(t_command *command, int _pipe[2])
{
	//todo: print errors here
	if (command->output)
		return (redirect_out(command));
	if (_pipe[1] == -1)
		return (0);
	if (dup2(_pipe[1], STDOUT_FILENO) < 0)
		return (-1);
	return (0);
}

int	pipe_in(t_command *command, int _pipe[2])
{
	if (command->input)
		return (redirect_in(command));
	if (dup2(_pipe[0], STDIN_FILENO) < 0)
		return (-1);
	return (0);
}

/* runs the command as a subprocess, if the command is builtin, we exit
with the returned status, if is not, we exit with an error code. */
static pid_t	execute_process(t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	//todo: maybe print an error ?
	if (pid < 0)
		return (1);
	command->command_flags |= MS_FORKED;
	if (pid == 0)
	{
		if (pipe_in(command, in_pipe) < 0)
			exit(1);
		if (pipe_out(command, out_pipe) < 0)
			exit(1);
		status = execute_command(command, in_pipe, out_pipe);
		if (command->command_flags & MS_BUILTIN)
			exit(status);
		exit(126);
	}
	close(out_pipe[1]);
	return (pid);
}

int	execute_and(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	status = execute_command(command->left, in_pipe, out_pipe);
	if (status == 0)
		return (execute_command(command->right, in_pipe, out_pipe));
	return (status);
}

int	execute_or(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	status = execute_command(command->left, in_pipe, out_pipe);
	if (status > 0)
		return (execute_command(command->right, in_pipe, out_pipe));
	return (status);
}

int	execute_pipe(t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;

	if (pipe(out_pipe) < 0)
		return (-1);
	pid = execute_process(command->left, in_pipe, out_pipe);
	close(in_pipe[0]);
	close(in_pipe[1]);
	copy_pipe(out_pipe, in_pipe);
	out_pipe[1] = -1;
	pid = execute_process(command->right, in_pipe, out_pipe);
	return (get_exit_status(pid));
}

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
	while (++i < nargs - 1 && nargs != 2)
		arguments[i] = *(char **)ft_darray_get(command->arguments, i - 1);
	arguments[i] = NULL;
	return (arguments);
}

//creates arguments in child process to avoid unnecessary copies.
int	launch_execve(t_command *command)
{
	extern char	**environ;
	char		**arguments;

	arguments = make_arguments(command);
	if (!arguments)
		exit(1);
	execve(command->command_name, arguments, environ);
	ms_perror("execve", NULL, NULL, errno);
	ft_clear_ds(arguments);
	return (1);
}

int	execute_builtin(t_command *command, int in_pipe[2], int out_pipe[2])
{
	//todo: call builtins.
	(void)in_pipe;
	(void)out_pipe;
	char	**arguments;

	arguments = make_arguments(command);
	if (strcmp(command->command_name, "echo") == 0)
		return (ms_echo(arguments));
	else if (strcmp(command->command_name, "cd") == 0)
		ms_cd(command->context, arguments);
	else if (strcmp(command->command_name, "pwd") == 0)
		ms_pwd();
	else if (strcmp(command->command_name, "export") == 0)
		ms_export(command->context, arguments);
	else if (strcmp(command->command_name, "env") == 0)
		ms_env();
	else if (strcmp(command->command_name, "unset") == 0)
		ms_unset(command->context, arguments);
	else if (strcmp(command->command_name, "exit") == 0)
		ms_exit(command->context, arguments);
	free(arguments);
	return (0);
}

//this is the core execution function.
int	execute_simple_command(t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;

	if (command->command_flags & MS_BUILTIN)
		return (execute_builtin(command, in_pipe, out_pipe));
	if (!(command->command_flags & MS_FORKED))
	{
		pid = fork();
		if (pid == 0)
		{
			//todo: handle errors
			if (command->output)
				redirect_out(command);
			if (command->input)
				redirect_in(command);
			launch_execve(command);
		}
		return (get_exit_status(pid));
	}
	launch_execve(command);
	return (1);
}

int	execute_command(t_command *command, int in_pipe[2], int out_pipe[2])
{
	if (!command)
		return (0);
	if (command->command_flags & MS_OPERAND)
		return (execute_simple_command(command, in_pipe, out_pipe));
	else if (command->command_flags & MS_AND)
		return (execute_and(command, in_pipe, out_pipe));
	else if (command->command_flags & MS_OR)
		return (execute_or(command, in_pipe, out_pipe));
	else if (command->command_flags & MS_PIPE)
		return (execute_pipe(command, in_pipe, out_pipe));
	return (1);
}

void	print_commands(t_command *command, int depth)
{
	int	i;
	if (!command)
		return ;
	if (!command->command_name)
		return ;
	for (int i = 0; i < depth; i++)
		printf("   |");
	printf("%s ", command->command_name);
	i = -1;
	if (command->arguments)
	{
		while(++i < command->arguments->size)
			printf("%s ", *(char **)ft_darray_get(command->arguments, i));
	}
	i = -1;
	if (command->redirections)
	{
		while(++i < command->redirections->size)
			printf("> %s ", ((t_redirection *)ft_darray_get(command->redirections, i))->file_path);
	}
	printf("input %d output %d\n", command->input, command->output);
	printf("\n");
	if (command->command_flags & MS_OPERAND)
		return ;
	print_commands(command->left, depth + 1);
	print_commands(command->right, depth + 1);
}

int	minishell_execute(t_command *command)
{
	int	in_pipe[2];
	int	out_pipe[2];
	int	status;

	// printf("Commands: \n");
	// print_commands(command, 0);
	//todo: we either create a pipe or open a redirection as input here.
	if (pipe(in_pipe) < 0)
		return (-1);
	status = execute_command(command, in_pipe, out_pipe);
	close(in_pipe[0]);
	close(in_pipe[1]);
	if (!access(MS_HEREDOC_PATH, F_OK))
		if (unlink(MS_HEREDOC_PATH))
			ms_perror("unlink", MS_HEREDOC_PATH, NULL, errno);
	return (status);
}
