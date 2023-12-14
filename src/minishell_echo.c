/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:10:33 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/14 15:25:52 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// no errors
int	ms_echo(char **arg, int fd)
{
	int		flag_n;
	int		i;

	flag_n = 0;
	i = 1;
	if (arg && arg[i] && !ft_strncmp(arg[i], "-n", 3))
	{
		i++;
		flag_n = 1;
	}
	while (arg && arg[i])
	{
		write(fd, arg[i], ft_strlen(arg[i]));
		i++;
		if (arg[i])
			write(fd, " ", 1);
	}
	if (!flag_n)
		write(fd, "\n", 1);
	return (0);
}
