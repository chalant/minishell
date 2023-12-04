/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:30:19 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/04 14:13:56 by ychalant         ###   ########.fr       */
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

// returns 1 if malloc in ms_realloc failed
static int	ms_add_var_env(t_ms_context *data, char *var)
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
	envp[i] = var;
	envp[i + 1] = NULL;
	data->env_excess--;
	return (0);
}

// 'var' is an allocated string of valid format [name]=[value]
// on error, 'var' is free'd
int	ms_export_var(t_ms_context *data, char *var)
{
	extern char	**environ;
	char		**temp_envp;

	temp_envp = ms_get_var_envp(data, var);
	if (temp_envp)
	{
		free(*temp_envp);
		*temp_envp = var;
	}
	else
		if (ms_add_var_env(data, var))
		{
			free(var);
			return (ERR_MALLOC);
		}
// malloc failed
	environ = data->env;
	return (0);
}

// no arguments: prints env in alphabetical order (all uppercase before any lowercase)
int	ms_export(t_ms_context *data, char **arg)
{
	char		*var;
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
			var = ft_strdup(arg[i]);
			if (!var)
				return (ERR_MALLOC);
// malloc failed
			if (ms_export_var(data, var))
				return (ERR_MALLOC);
// malloc failed
		}
		else
			ret = ms_perror("export", arg[i], "not a valid identifier", 0);
		i++;
	}
	return (ret);
}
