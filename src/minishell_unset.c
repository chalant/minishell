/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 18:31:09 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/04 14:13:56 by ychalant         ###   ########.fr       */
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
	free(envp[0]);
	i = 0;
	while (envp[i])
	{
		envp[i] = envp[i + 1];
		i++;
	}
	data->env_excess++;
	return ;
}

// free's args
// returns 0 even if 'name' doesn't exist
int	ms_unset(t_ms_context *data, char **arg)
{
	extern char	**environ;
	int			i;

	i = 0;
	if (arg)
		arg++;
	while (arg && arg[i])
	{
		ms_unset_var(data, arg[i]);
		i++;
	}
	if (data->env_excess > 20)
	{
		i = -1 * (data->env_excess - 10);
		data->env = ms_realloc(data->env, i);
		if (!data->env)
			return (ERR_MALLOC);
// error: malloc failed
		data->env_excess += i;
	}
	environ = data->env;
	return (0);
}
