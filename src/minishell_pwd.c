/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:55:14 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/28 15:42:04 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_pwd(int fd)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
		return (ms_perror("pwd", NULL, NULL, errno));
	write(fd, buf, ft_strlen(buf));
	write(fd, "\n", 1);
	free(buf);
	return (0);
}
