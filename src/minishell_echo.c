/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:10:33 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/20 17:25:04 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_echo(char **args)
{
	int	flag_n;
	int	i;

	i = 0;
	flag_n = 0;
	if (args && args[i] && !ft_strncmp(args[i], "-n", 3))
	{
		i++;
		flag_n = 1;
	}
	while (args && args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		i++;
		if (args[i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (!flag_n)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

/* TEST MAIN */

int	main(int ac, char **av)
{
	char	**args;

	args = malloc(sizeof(char *) * ac);
	ac--;
	args[ac] = NULL;
	while (ac--)
		args[ac] = av[ac + 1];
	ms_echo(args);
	free(args);
}
