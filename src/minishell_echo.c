/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:10:33 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/21 16:50:33 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// free's arg
int	ms_echo(char **arg)
{
	int	flag_n;
	int	i;

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
		write(STDOUT_FILENO, arg[i], ft_strlen(arg[i]));
		i++;
		if (arg[i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (!flag_n)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
