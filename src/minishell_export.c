/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:30:19 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/22 15:29:07 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// error: ERR_MALLOC
static int	ms_add_var_env(t_ms_context *data, char *var)
{
	char	**envp;
	int		i;

	if (!data->env_excess)
	{
		data->env = ms_realloc(data->env, 5);
		if (!data->env)
			return (ERR_MALLOC);
		data->env_excess = 5;
	}
	envp = data->env;
	i = 0;
	while (envp[i])
		i++;
	envp[i] = var;
	envp[i + 1] = NULL;
	data->env_excess--;
	return (0);
}

// 'var' is an allocated string of valid format [name]=[value]
// error: ERR_MALLOC, free(var)
int	ms_export_var(t_ms_context *data, char *var)
{
	extern char	**environ;
	char		**temp_envp;

	if (!ft_strchr(var, '='))
	{
		free(var);
		return (0);
	}
	temp_envp = ms_get_var_envp(data, var);
	if (temp_envp)
	{
		free(*temp_envp);
		*temp_envp = var;
	}
	else if (ms_add_var_env(data, var))
	{
		free(var);
		return (ERR_MALLOC);
	}
	environ = data->env;
	return (0);
}

// no args: prints env in ascii order
// error: 1 or nomem, prints msg
int	ms_export(t_ms_context *data, char **arg, int fd)
{
	char	*var;
	int		i;
	int		ret;

	ret = 0;
	i = 1;
	if (!arg || !arg[1])
		return (ms_env_alpha(data->env, fd));
	while (arg[i])
	{
		if (!ms_check_varname(arg[i]))
		{
			var = ft_strdup(arg[i]);
			if (!var)
				return (ms_perror(arg[0], arg[i], NULL, errno));
			if (ms_export_var(data, var))
				return (ms_perror(arg[0], NULL, NULL, errno));
		}
		else
			ret = ms_perror(arg[0], arg[i], "not a valid identifier", 0);
		i++;
	}
	return (ret);
}
