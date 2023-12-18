/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:45:06 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/18 18:42:22 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_errprintstrcol(const char *str)
{
	if (!str)
		return ;
	write(STDERR_FILENO, str, ft_strlen(str));
	write(STDERR_FILENO, ": ", 2);
}

// prints error like (shellshock: cmd: item: msg: err)
// handles NULL pointers, and err <= 0 (takes 'msg' as last arg)
// returns 1, or ERR_NOMEM if err == ENOMEM
int	ms_perror(const char *cmd, const char *item, const char *msg, int err)
{
	char	*strerr;

	write(STDERR_FILENO, "shellshock: ", 12);
	ms_errprintstrcol(cmd);
	ms_errprintstrcol(item);
	if (msg)
	{
		write(STDERR_FILENO, msg, ft_strlen(msg));
		if (err > 0)
			write(STDERR_FILENO, ": ", 2);
	}
	if (err > 0)
	{
		strerr = strerror(err);
		write(STDERR_FILENO, strerr, ft_strlen(strerr));
	}
	write(STDERR_FILENO, "\n", 1);
	if (err == ENOMEM)
		return (ERR_NOMEM);
	return (1);
}

int	ms_message_header(void *data, int (*printer)(void *), int fd)
{
	write(fd, "shellshock: ", 12);
	return (printer(data));
}
