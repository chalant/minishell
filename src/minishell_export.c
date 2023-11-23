/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:30:19 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/22 19:24:15 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_cmp_and_swap(char **pa, char **pb)
{
	char	*temp;

	if (ft_strncmp(*pa, *pb, ft_strlen(*pb)) <= 0)
		return ;
	temp = *pa;
	*pa = *pb;
	*pb = temp;
}

// prints env but in order of ascending ascii value
// don't print $_
static int	ms_env_alpha()
{
	extern char	**environ;
	char		**envp;
	int			i;
	int			last;

	last = 0;
	while (environ[last])
		last++;
	envp = malloc(sizeof(char *) * (last + 1));
	last = 0;
	while (environ[last])
	{
		envp[last] = environ[last];
		last++;
	}
	envp[last] = NULL;
	while (last > 1)
	{
		i = 0;
		while (i + 1 < last)
		{
			ms_cmp_and_swap(&envp[i], &envp[i + 1]);
			i++;
		}
		last--;
	}
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "_=", 2))
			printf("%s\n", envp[i]);
// this printf is leaking 'still reachable' mem?
		i++;
	}
	free(envp);
	return (0);
}

// returns 1 if malloc failed
static int	ms_add_var_env(t_shellshock *data, char *var)
{
	char		**envp;
	int			i;

	if (!data->env_excess)
	{
		data->env = ms_realloc(data->env, 3);
		if (!data->env)
			return (ERR_MALLOC);
		data->env_excess += 3;
	}
	envp = data->env;
	i = 0;
	while (envp[i])
		i++;
	envp[i] = ft_strdup(var);
// check malloc fail
	envp[i + 1] = NULL;
	data->env_excess--;
	return (0);
}

// free's arg (unless *arg == NULL)
// no arguments: prints env in alphabetical order (all uppercase before any lowercase)
// always returns 0 (even on bad arguments it seems like) (ok not true, e.g. "export =ab=c")
int	ms_export(t_shellshock *data, char **arg)
{
	extern char	**environ;
	char		*ptr;
	char		**temp_envp;
	int			i;
	int			ret;

	if (arg)
		arg++;
	if (!arg || !*arg)
		return (ms_env_alpha());
	ret = 0;
	i = 0;
	while (arg[i])
	{
		if (!ms_check_varname(arg[i]))
		{
			temp_envp = ms_get_var_envp(data, arg[i]);
			if (temp_envp)
			{
				free(*temp_envp);
				*temp_envp = ft_strdup(arg[i]);
// check malloc fail, (do something with NULL hole in env)
			}
			else
				if (ms_add_var_env(data, arg[i]))
					return (ERR_MALLOC);
// malloc failed
		}
		else
			ret = ms_perror("export", arg[i], "not a valid identifier", 0);
		i++;
	}
	environ = data->env;
	return (ret);
}
