/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 15:32:40 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/01 16:53:00 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// clear history, clear env, write exit, exit(exit_value)
void	ms_flush_exit(t_shellshock *data, int exit_value)
{
	free_parse_data(&(data->parse_data), &(data->tree));
	rl_clear_history();
	ft_clear_ds(data->env);
	write(STDOUT_FILENO, "exit\n", 5);
	exit(exit_value);
}

// doesn't clear arg if error occured
// any tokens should be cleared before this command
int	ms_exit(t_shellshock *data, char **arg)
{
	int	i;

	if (!arg || !arg[1])
	{
		if (arg)
		{
			free(arg[0]);
			free(arg);
		}
		free(data->line);
		ms_flush_exit(data, data->status);
		// return return of last cmd
	}
	i = 0;
	if (arg[1][i] == '-' || arg[1][i] == '+')
		i++;
	while (arg[1][i])
	{
		if ('0' > arg[1][i] || arg[1][i] > '9')
		{
			ms_perror("exit", arg[1], "numeric argument required", 0);
			ft_clear_ds(arg);
			free(data->line);
			ms_flush_exit(data, 2);
		}
		i++;
	}
	if (arg[2])
		return (ms_perror("exit", NULL, "too many arguments", 0));
	i = (char) ft_atoi(arg[1]);
	ft_clear_ds(arg);
	free(data->line);
	ms_flush_exit(data, i);
	return (1);
}
