/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 15:32:40 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/14 15:12:58 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// clear history, clear env, write exit, exit(exit_value)
void	ms_flush_exit(t_ms_context *data, int exit_value)
{
	delete_commands(&(data->commands));
	free_parse_data(&(data->parse_data), &(data->tree));
	rl_clear_history();
	ft_clear_ds(data->env);
	write(STDERR_FILENO, "exit\n", 5);
	exit(exit_value);
}

static void	ms_exit_exit(t_ms_context *data, char **arg, int e)
{
	if (arg)
		free(arg);
	free(data->line);
	ms_flush_exit(data, e);
}

// exits innit
// error: 1, doesn't free arg, msg
int	ms_exit(t_ms_context *data, char **arg)
{
	int	i;

// immediately print exit???
	if (!arg || !arg[1])
		ms_exit_exit(data, arg, data->status);
	i = 0;
	if (arg[1][i] == '-' || arg[1][i] == '+')
		i++;
	while (arg[1][i])
	{
		if ('0' > arg[1][i] || arg[1][i] > '9')
		{
			ms_perror(arg[0], arg[1], "numeric argument required", 0);
			ms_exit_exit(data, arg, 2);
		}
		i++;
	}
	if (arg[2])
		return (ms_perror(arg[0], NULL, "too many arguments", 0));
	i = (char) ft_atoi(arg[1]);
	ms_exit_exit(data, arg, i);
	return (1);
}
