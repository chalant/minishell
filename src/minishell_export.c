/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:30:19 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/21 23:37:54 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_sort_env(char **envp, int size)
{
	int		i;
	char	*temp;

	while (size > 1)
	{
		i = 0;
		while (i + 1 < size)
		{
			if (ft_strncmp(envp[i], envp[i + 1], ft_strlen(envp[i])) > 0)
			{
				temp = envp[i];
				envp[i] = envp[i + 1];
				envp[i + 1] = temp;
			}
			i++;
		}
		size--;
	}
}

// prints env but in order of ascending ascii value
// error: nomem, prints msg
static int	ms_env_alpha(char **env, int i, int fd)
{
	char	**temp_envp;
	char	*equal_sign;

	i = 0;
	while (env[i])
		i++;
	temp_envp = malloc(sizeof(char *) * (i + 1));
	if (!temp_envp)
		return (ms_perror("export", NULL, NULL, errno));
	i = -1;
	while (env[++i])
		temp_envp[i] = env[i];
	temp_envp[i] = NULL;
	ms_sort_env(temp_envp, i);
	i = -1;
	while (temp_envp[++i])
	{
(void)fd;
		equal_sign = ft_strchr(temp_envp[i], '=');
		*equal_sign = 0;
		printf("declare -x %s=", temp_envp[i]);
		*equal_sign = '=';
		printf("\"%s\"\n", equal_sign + 1);
	}
	free(temp_envp);
	return (0);
}

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
		return (ms_env_alpha(data->env, i, fd));
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
