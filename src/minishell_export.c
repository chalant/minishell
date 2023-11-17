/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:30:19 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/17 17:18:25 by bvercaem         ###   ########.fr       */
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
	extern char	**environ;
	int			i;

	if (!data->env_excess)
	{
		data->env = ms_realloc(data->env, 3);
		if (!data->env)
			return (ERR_MALLOC);
		environ = data->env;
		data->env_excess += 3;
	}
	i = 0;
	while (environ[i])
		i++;
	environ[i] = var;
	environ[i + 1] = NULL;
	data->env_excess--;
}

// free's arg (unless *arg == NULL)
// no arguments: prints env in alphabetical order (all uppercase before any lowercase)
// always returns 0 (even on bad arguments it seems like) (ok not true, e.g. "export =ab=c")
int	ms_export(t_shellshock *data, char **arg)
{
	char	*ptr;
	char	**temp;
	int		i;
	int		ret;

	if (!arg || !*arg)
		return (ms_env_alpha());
	ret = 0;
	i = 0;
	while (arg[i])
	{
		if (!ms_check_varname(arg[i]))
		{
			ptr = ms_get_var_env(arg[i]);
			temp = &ptr;
			if (*temp)
			{
				free(*temp);
				*temp = arg[i];
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
	return (ret);
}
