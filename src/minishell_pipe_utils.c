/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pipe_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:01:41 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 12:01:42 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_pipe(int	pipe_[2])
{
	if (pipe_[1] == -1)
	{
		close_fd(&pipe_[0]);
		close_fd(&pipe_[1]);
		if (pipe(pipe_) < 0)
			return ((ms_perror("pipe", NULL, NULL, errno) * -1));
	}
	return (0);
}

int	swap_pipe(int *src_pipe, int *dest_pipe)
{
	dest_pipe[0] = src_pipe[0];
	dest_pipe[1] = src_pipe[1];
	if (pipe(src_pipe) < 0)
		return ((ms_perror("pipe", NULL, NULL, errno) * -1));
	return (0);
}
