/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_pipe.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:01:53 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 13:43:11 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	transfer_fds(t_command *command)
{
	if (command->left && !command->left->input)
		command->left->input = command->input;
	if (command->right && !command->right->output)
		command->right->output = command->output;
	return (0);
}

static int	is_last_command(t_command *parent, t_command *command)
{
	if (command->command_flags & MS_LAST)
	{
		command->command_flags &= ~(MS_LAST);
		command->right->command_flags |= MS_LAST;
	}
	if ((parent && !(parent->command_flags & MS_PIPE))
		|| command->command_flags & MS_LAST)
	{
		command->right->output = command->output;
		command->command_flags &= ~(MS_LAST);
		parent->command_flags &= ~(MS_LAST);
		command->right->command_flags |= MS_LAST;
	}
	return (0);
}

static int	execute_maybe_wait(t_command *parent, t_command *command,
	int in_pipe[2], int out_pipe[2])
{
	int	status;

	if (!parent)
		return (0);
	status = execute_command(parent, command, in_pipe, out_pipe);
	if (parent->error == EAGAIN && wait(NULL) != -1)
		status = execute_command(parent, command, in_pipe, out_pipe);
	return (status);
}

int	execute_pipe(t_command *parent, t_command *command,
	int in_pipe[2], int out_pipe[2])
{
	int	status;

	if (init_pipe(out_pipe) < 0)
		return (-1);
	if (handle_redirections(command, open_all) < 0)
		return (close_fd(&out_pipe[1]));
	transfer_fds(command);
	if (execute_maybe_wait(command, command->left, in_pipe, out_pipe) < 0)
		return (-1);
	if (swap_pipe(out_pipe, in_pipe) < 0)
		return (-1);
	if (!parent || !(parent->command_flags & MS_PIPE))
		close_fd(&out_pipe[1]);
	is_last_command(parent, command);
	status = execute_maybe_wait(command, command->right, in_pipe, out_pipe);
	if (status == 1 || status < 0)
		return (status);
	if (!parent || (parent && !(parent->command_flags & MS_PIPE)))
		return (get_exit_status(command->pid));
	return (status);
}
