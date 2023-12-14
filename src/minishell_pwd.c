/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:55:14 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/14 14:35:03 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// error: 1 or nomem, prints msg
int	ms_pwd(char **arg, int fd)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
	{
		if (errno == ENOMEM)
			return (ms_perror(arg[0], NULL, NULL, errno) * ERR_NOMEM);
		return (ms_perror(arg[0], NULL, NULL, errno));
	}
	write(fd, buf, ft_strlen(buf));
	write(fd, "\n", 1);
	free(buf);
	return (0);
}
