/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 18:31:09 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/27 16:20:03 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int ms_unset_core(t_shellshock *data, char *name)
{
	extern char	**environ;
	int	i;
	int	name_len;

	i = 0;
	name_len = ft_strlen(name);
	while (environ[i])
	{
		if (!ft_strncmp(environ[i], name, name_len))
			if (environ[i][name_len] == '=')
				break ;
		i++;
	}
	if (!environ[i])
		return (1);
	free(environ[i]);
	while (environ[i + 1])
	{
		environ[i] = environ[i + 1];
		i++;
	}
	environ[i] = environ[i + 1];
	data->env_excess++;
	return (0);
}

// free's args
// returns 0 even if 'name' doesn't exist
int	ms_unset(t_shellshock *data, char **arg)
{
	extern char	**environ;
	int			i;

// check input? if it starts with '=' that's bad
	i = 0;
	while (arg && arg[i])
	{
		ms_unset_core(data, arg[i]);
		i++;
	}
	ft_clear_ds(arg);
	if (data->env_excess > 20)
	{
		i = -1 * (data->env_excess - 10);
		data->env = ms_realloc(data->env, i);
		if (!data->env)
			exit(1);
		environ = data->env;
// error: malloc failed
		data->env_excess += i;
	}
	return (0);
}
