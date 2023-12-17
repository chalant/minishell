/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_core.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 15:17:05 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/17 17:04:41 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	execute_builtin(t_command *command, int in_fd, int out_fd)
{
	int		status;
	char	**arguments;

	(void)in_fd;
	status = 0;
	arguments = make_arguments(command, command->command_name);
	if (!arguments)
		return (-1);
	if (strcmp(command->command_name, "echo") == 0)
		status = ms_echo(arguments, out_fd);
	else if (strcmp(command->command_name, "cd") == 0)
		status = ms_cd(command->context, arguments, out_fd);
	else if (strcmp(command->command_name, "pwd") == 0)
		status = ms_pwd(arguments, out_fd);
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
	char		**arguments;
	char		*binary;

	binary = get_binary(command->command_name);
	if (!binary)
		exit(1);
	arguments = make_arguments(command, binary);
	if (!arguments)
	{
		free(binary);
		ms_perror(command->command_name, NULL, NULL, ENOMEM);
		exit(1);
	}
	execve(binary, arguments, command->context->env);
	free(binary);
	free(arguments);
	ms_perror(command->command_name, NULL, NULL, errno);
	exit(127);
}

int	ms_execve(t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;

	if (!(command->command_flags & MS_FORKED))
	{
		pid = fork();
		if (pid < 0)
			return (ms_perror("fork", NULL, NULL, errno) - 2);
		if (pid == 0)
		{
			if (pipe_io(command, in_pipe, out_pipe) < 0)
				exit(1);
			launch_execve(command);
		}
		close_fd(&out_pipe[1]);
		//close(in_pipe[0]);
		// in_pipe[0] = -1;
		close_fd(&command->output);
		return (get_exit_status(pid));
	}
	launch_execve(command);
	return (1);
}

int	execute_simple_command(t_command *command, int in_pipe[2], int out_pipe[2])
{
	if (command->command_flags & MS_BUILTIN && !(command->command_flags & MS_FORKED))
	{
		if (command->output > 0)
			return (execute_builtin(command, command->input, command->output));
		else
			return (execute_builtin(command, command->input, STDOUT_FILENO));
	}
	else if (command->command_flags & MS_BUILTIN && command->command_flags & MS_FORKED)
	{
		if (redirect_io(command) < 0)
			exit(1);
		return (execute_builtin(command, STDIN_FILENO, STDOUT_FILENO));
	}
	return (ms_execve(command, in_pipe, out_pipe));
}

pid_t	execute_process(t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (ms_perror("fork", NULL, NULL, errno) - 2);
	command->command_flags |= MS_FORKED;
	if (pid == 0)
	{
		if (handle_redirections(command) < 0)
			exit(1);
		if (pipe_io(command, in_pipe, out_pipe) < 0)
			exit(1);
		status = execute_simple_command(command, in_pipe, out_pipe);
		if (command->command_flags & MS_BUILTIN)
			exit(status);
		exit(126);
	}
	close_fd(&out_pipe[1]);
	close_fd(&in_pipe[0]);
	return (pid);
}

int	execute_command_core(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
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
