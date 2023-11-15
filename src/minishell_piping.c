/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_piping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 14:20:57 by ychalant          #+#    #+#             */
/*   Updated: 2023/07/09 14:20:57 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_out(int pipe_[2])
{
	close_read(pipe_);
	dup2(pipe_[1], STDOUT_FILENO);
	close_write(pipe_);
	return (1);
}

int	pipe_in(int pipe_[2])
{
	close_write(pipe_);
	dup2(pipe_[0], STDIN_FILENO);
	close_read(pipe_);
	return (1);
}

int	inject_fd(int fd, int in_pipe[2])
{
	if (fd < 0)
		return (0);
	dup2(fd, STDIN_FILENO);
	close(fd);
	pipe_out(in_pipe);
	return (1);
}

int	collect_fd(int fd, int out_pipe[2])
{
	if (fd < 0)
		return (0);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	pipe_in(out_pipe);
	return (1);
}

int	connect_pipes(int in_pipe[2], int out_pipe[2])
{
	pipe_in(in_pipe);
	pipe_out(out_pipe);
	return (1);
}
