/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_operands.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 15:17:05 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/21 14:49:03 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "sys/stat.h"

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

int	get_executable(t_command *command, char **binary, struct stat *statbuf)
{
	if (ft_strchr(command->command_name, '/'))
	{
		if (stat(command->command_name, statbuf) == -1)
			exit(ms_perror(command->command_name, NULL, NULL, errno) * 127);
		if (S_ISDIR(statbuf->st_mode))
			exit(ms_perror(command->command_name, NULL,
					"Is a directory", 0) * 126);
		else if (access(command->command_name, X_OK) != 0)
			exit(ms_perror(command->command_name, NULL, NULL, errno) * 126);
		*binary = ft_strdup(command->command_name);
		if (!*binary)
			exit(ms_perror(command->command_name, NULL, NULL, errno));
	}
	return (get_binary(command->command_name, binary));
}

//creates arguments in child process to avoid unnecessary copies.
int	launch_execve(t_command *command)
{
	char		**arguments;
	char		*binary;
	struct stat	statbuf;

	binary = NULL;
	if (get_executable(command, &binary, &statbuf) < 0)
		exit(1);
	else if (!binary)
		exit(ms_perror(command->command_name, NULL,
				"command not found", 0) * 127);
	arguments = make_arguments(command, binary);
	if (!arguments)
	{
		free(binary);
		exit(ms_perror(command->command_name, NULL, NULL, ENOMEM));
	}
	execve(binary, arguments, command->context->env);
	free(binary);
	free(arguments);
	ms_perror(command->command_name, NULL, NULL, errno);
	exit(127);
}

int	execute_simple_command(t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;

	if (command->command_flags & MS_BUILTIN)
		return (execute_builtin(command));
	else if (!(command->command_flags & MS_FORKED))
	{
		pid = fork();
		if (pid < 0)
			return (ms_perror("fork", NULL, NULL, errno) * -1);
		if (pid == 0)
		{
			if (pipe_io(command, in_pipe, out_pipe) < 0)
				exit(1);
			launch_execve(command);
		}
		close_fd(&out_pipe[1]);
		return (get_exit_status(pid));
	}
	launch_execve(command);
	return (1);
}

int	execute_operand(t_command *parent, t_command *command,
	int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;

	if (parent && parent->command_flags & MS_PIPE)
	{
		if (command->command_flags & MS_LAST)
			close_fd(&out_pipe[1]);
		pid = execute_process(command, in_pipe, out_pipe);
		if (pid < 0)
		{
			parent->error = errno;
			return (-1);
		}
		parent->pid = pid;
		return (0);
	}
	if (handle_redirections(command) < 0)
		return (1);
	return (execute_simple_command(command, in_pipe, out_pipe));
}
