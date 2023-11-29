/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:02:24 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/29 17:36:58 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_status(int pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 2);
	while (wait(NULL) != -1)
		continue ;
	if (((status) & 0x7f) == 0)
		return ((((status) & 0xff00) >> 8));
	return (0);
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
		return (1);
	return (0);
}

int	redirect_out(t_command *command)
{
	if (dup2(command->output, STDOUT_FILENO) < 0)
		return (1);
	return (0);
}

int	pipe_out(t_command *command, int _pipe[2])
{
	if (command->output)
		return (redirect_out(command));
	//todo: report errors
	dup2(_pipe[1], STDOUT_FILENO);
	return (0);
}

int	pipe_in(t_command *command, int _pipe[2])
{
	if (command->input)
		return (redirect_in(command));
	//todo: report errors
	dup2(_pipe[0], STDIN_FILENO);
	return (0);
}

/* runs the command as a subprocess, if the command is builtin, we exit
with the returned status, if is not, we exit with an error code. */
int	execute_process(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	pid;
	int	status;

	pid = fork();
	//todo: maybe print an error ?
	if (pid < 0)
		return (-1);
	//todo: make cleaner redirection handling.
	command->command_flags |= MS_FORKED;
	if (pid == 0)
	{
		//todo handle pipe errors.
		pipe_in(command, in_pipe);
		pipe_out(command, out_pipe);
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

	//todo: if there are redirections, the out_pipe has
	//a write file descriptor in append mode.
	status = execute_command(command->left, in_pipe, out_pipe);
	if (status == 0)
		return (execute_command(command->right, in_pipe, out_pipe));
	return (status);
}

int	execute_or(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	//todo: if there are redirections, the out_pipe has
	//a write file descriptor in append mode.
	status = execute_command(command->left, in_pipe, out_pipe);
	if (status > 0)
		return (execute_command(command->right, in_pipe, out_pipe));
	return (status);
}

//todo: should also be able to launch heredocs.
int	execute_pipe(t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;

	if (pipe(out_pipe) < 0)
		return (-1);
	//todo: out_pipe could also be a redirection so we need to either create a pipe or open a file...
	//read from in pipe of the parent
	pid = execute_process(command->left, in_pipe, out_pipe);
	copy_pipe(out_pipe, in_pipe);
	pid = execute_process(command->right, in_pipe, out_pipe);
	return (get_exit_status(pid));
}

int	launch_execve(t_command *command)
{
	extern char	**environ;
	char		**arguments;
	int			nargs;
	int			i;

	//todo: lookup into the environment.
	//doing this in the child process to avoid unnecessary copies.
	nargs = 2;
	if (command->arguments)
		nargs = command->arguments->size + 2;
	arguments = malloc(sizeof(char *) * nargs);
	if (!arguments)
		exit(1);
	arguments[0] = command->command_name;
	i = 0;
	while (++i < nargs - 1 && nargs != 2)
		arguments[i] = *(char **)ft_darray_get(command->arguments, i - 1);
	arguments[i] = NULL;
	//todo handle errors
	execve(command->command_name, arguments, environ);
	perror("error HERE");
	//todo: free arguments here in cause of errors..
	return (1);
}

int	execute_builtin(t_command *command, int in_pipe[2], int out_pipe[2])
{
	//todo: call builtins.
	(void)command;
	(void)in_pipe;
	(void)out_pipe;
	// if (strcmp(command->command_name, "echo") == 0)
	// 	ms_echo(command->arguments);
	// else if (strcmp(command->command_name, "echo") == 0)
	// 	ms_cd(command->data,)
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
	int			in_pipe[2];
	int			out_pipe[2];

	//printf("Commands: \n");
	print_commands(command, 0);
	//todo: we either create a pipe or open a redirection as input here.
	if (pipe(in_pipe) < 0)
		return (-1);
	execute_command(command, in_pipe, out_pipe);
	close(in_pipe[0]);
	close(in_pipe[1]);
	//todo: close all open file descriptors
	if (!access(MS_HEREDOC_PATH, F_OK))
		if (unlink(MS_HEREDOC_PATH))
			ms_perror("unlink", MS_HEREDOC_PATH, NULL, errno);
	return (0);
}
