/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:10:33 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/06 16:40:44 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_echo(char **arg, int fd)
{
	int		flag_n;
	int		i;

	i = 0;
	flag_n = 0;
	if (arg)
		arg++;
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
