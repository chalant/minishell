/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_operators.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:14:21 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/21 14:31:09 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	distribute_fds(t_command *parent, t_command *command)
{
	if (command && !command->input)
	{
		command->input = parent->input;
		command->command_flags |= MS_INJECTI;
	}
	if (command && !command->output)
	{
		command->output = parent->output;
		command->command_flags |= MS_INJECTO;
	}
	return (0);
}

int	execute_and(t_command *parent, t_command *command,
	int in_pipe[2], int out_pipe[2])
{
	int	status;

	status = handle_redirections(command, open_all) < 0;
	if (status)
		return (close_fd(&out_pipe[1]) * status);
	distribute_fds(command, command->left);
	if (!parent || !(command->command_flags & MS_AND))
		close_fd(&out_pipe[1]);
	status = execute_command(command, command->left, in_pipe, out_pipe);
	if (status == 0)
	{
		distribute_fds(command, command->right);
		return (execute_command(command, command->right, in_pipe, out_pipe));
	}
	return (status);
}

// todo: would be better to have the left command as not NULL and set a flag
// so that we know there is no command, but we should execute rediretions etc.
// that way, we passing redirections to the other command during semantics
int	execute_or(t_command *parent, t_command *command,
	int in_pipe[2], int out_pipe[2])
{
	int	status;

	status = 0;
	if (command->left)
	{
		status = handle_redirections(command, open_all) < 0;
		if (status)
			return (close_fd(&out_pipe[1]) * status);
		distribute_fds(command, command->left);
		if (!parent || !(command->command_flags & MS_OR))
			close_fd(&out_pipe[1]);
		status = execute_command(command, command->left, in_pipe, out_pipe);
	}
	if ((status > 0 && status != 130) || !command->left)
	{
		distribute_fds(command, command->right);
		if (command->command_flags & MS_OPERATOR && !command->left)
			if (handle_redirections(command->right, open_prepend) == 1)
				return (0);
		return (execute_command(command, command->right, in_pipe, out_pipe));
	}
	return (status);
}
