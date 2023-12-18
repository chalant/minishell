/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_io.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 15:16:57 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/17 16:51:36 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	close_fd(int *fd)
{
	if (*fd > 0)
		close(*fd);
	*fd = -1;
	return (1);
}

int	pipe_out(t_command *command, int pipe_[2])
{
	if (command->output > 0)
		return (redirect_out(command));
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
