/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 18:31:09 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/14 15:28:19 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_unset_var(t_ms_context *data, char *name)
{
	char	**envp;
	int		i;

	envp = ms_get_var_envp(data, name);
	if (!envp)
		return ;
	free(*envp);
	i = 0;
	while (envp[i])
	{
		envp[i] = envp[i + 1];
		i++;
	}
	data->env_excess++;
	return ;
}

// returns 0 even if 'name' doesn't exist
// error: nomem, prints msg
int	ms_unset(t_ms_context *data, char **arg)
{
	extern char	**environ;
	int			i;

	if (!arg || !arg[1])
		return (0);
	i = 1;
	while (arg[i])
	{
		ms_unset_var(data, arg[i]);
		i++;
	}
	if (data->env_excess > 20)
	{
		data->env = ms_realloc(data->env, 5);
		if (!data->env)
			return (ms_perror(arg[0], NULL, NULL, errno) * ERR_NOMEM);
		data->env_excess = 5;
	}
	environ = data->env;
	return (0);
}
