/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:30:19 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/20 18:34:36 by bvercaem         ###   ########.fr       */
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
// swap out this printf (it's leaking 'still reachable' mem anyways?)
		i++;
	}
	free(envp);
	return (0);
}

static void	ms_add_var_env(char *var)
{
	extern char	**environ;
	int			i;
// can we keep track of char **environ size?
// if we unset a var we don't need to resize it
// if there's left-over space we don't need to malloc anything here :)
// if we do malloc something we could malloc like 3 lines at once

	environ = ms_realloc(environ, 1);
	if (!environ)
		exit(1);
// malloc failed
	i = 0;
	while (environ[i])
		i++;
	environ[i] = var;
	environ[i + 1] = NULL;
}

// NOT TESTED YET WITH ARGS
// no arguments: print env in alphabetical order (all uppercase before any lowercase)
// always returns 0 (even on bad arguments it seems like) (ok not true, e.g. "export =ab=c")
int	ms_export(char **arg)
{
	char	**temp;

// check input? if it starts with '=' that's bad
	if (!arg || !*arg)
		return (ms_env_alpha());
	while (*arg)
	{
		temp = ms_get_var_env(*arg);
		if (temp)
		{
			free(*temp);
			*temp = *arg;
		}
		else
			ms_add_var_env(*arg);
		arg++;
	}
	return (0);
}

/* TEST MAIN */

int	main(void)
{
	extern char	**environ;

	ms_export(NULL);
}
