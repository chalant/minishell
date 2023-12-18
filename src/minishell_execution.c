/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:02:24 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 18:17:05 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	launch_heredocs(t_command *command, int *id)
{
	if (!access(MS_HEREDOC_PATH, F_OK))
		if (unlink(MS_HEREDOC_PATH))
			return (ms_perror("unlink", MS_HEREDOC_PATH, NULL, errno) * (-1));
	if (command->redirections && (command->command_flags & MS_OPERAND
			|| command->command_flags & MS_REDIR))
	{
		*id += 1;
		return (ms_heredoc(command, *id));
	}
	if (command->left)
		launch_heredocs(command->left, id);
	if (command->right)
		launch_heredocs(command->right, id);
	if (command->redirections && command->command_flags & MS_OPERATOR)
	{
		*id += 1;
		ms_heredoc(command, *id);
	}
	return (1);
}

int	check_redirection(t_command *command)
{
	if (!command->left || command->left->command_flags & MS_REDIR)
		command->right->command_flags |= MS_NOREDIR;
	return (0);
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

int	execute_command(t_command *parent, t_command *command,
	int in_pipe[2], int out_pipe[2])
{
	if (!command)
		return (close_fd(&out_pipe[1]) * 0);
	if (command->command_flags & MS_OPERATOR)
		check_redirection(command);
	if (command->command_flags & MS_OPERAND)
		command->context->status = execute_operand(parent,
				command, in_pipe, out_pipe);
	else if (command->command_flags & MS_AND)
		command->context->status = execute_and(parent,
				command, in_pipe, out_pipe);
	else if (command->command_flags & MS_OR)
		command->context->status = execute_or(parent,
				command, in_pipe, out_pipe);
	else if (command->command_flags & MS_PIPE)
		command->context->status = execute_pipe(parent,
				command, in_pipe, out_pipe);
	if (g_global_status)
	{
		command->context->status = g_global_status;
		g_global_status = 0;
	}
	return (command->context->status);
}

int	start_execution(t_command *command)
{
	int	in_pipe[2];
	int	out_pipe[2];
	int	status;
	int	hd_id;

	hd_id = 0;
	out_pipe[0] = -1;
	out_pipe[1] = -1;
	in_pipe[0] = -1;
	in_pipe[1] = -1;
	launch_heredocs(command, &hd_id);
	if (handle_redirections(command) < 0)
		return (1);
	status = execute_command(NULL, command, in_pipe, out_pipe);
	close_fd(&in_pipe[0]);
	close_fd(&in_pipe[1]);
	close_fd(&out_pipe[0]);
	close_fd(&out_pipe[1]);
	while (wait(NULL) != -1)
		continue ;
	return (status);
}
