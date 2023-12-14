/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_core.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 15:17:05 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/14 17:07:03 by ychalant         ###   ########.fr       */
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
	ms_perror(command->command_name, NULL, NULL, errno);
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
	else if (command->command_flags & MS_BUILTIN && command->command_flags & MS_FORKED)
	{
		if (redirect_io(command) < 0)
			exit(1);
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
			return (ms_perror("fork", NULL, NULL, errno) - 2);
		if (pid == 0)
		{
			//todo: handle errors
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
		if (command->output > 0)
			close(command->output);
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
	{
		if (parent)
			parent->error = errno;
		return (ms_perror("fork", NULL, NULL, errno) - 2);
	}
	command->command_flags |= MS_FORKED;
	if (pid == 0)
	{
		if (command->redirections && command->redirections->size)
			create_files(command, command->redirections);
		//todo: handle errors
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

int	execute_command_core(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	pid_t	pid;

	//todo: handle errors
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
			return (-1);
		// if (command->command_flags & MS_LAST)
		// 	return (get_exit_status(pid));
		parent->pid = pid;
		return (0);
	}
	if (command->redirections && command->redirections->size)
		create_files(command, command->redirections);
	return (execute_simple_command(parent,command, in_pipe, out_pipe));
}
