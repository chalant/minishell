/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:30:19 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/18 17:37:30 by bvercaem         ###   ########.fr       */
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
static void	ms_env_alpha()
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
}

// no arguments: print env in alphabetical order (all uppercase before any lowercase)
// always returns 0 (even on bad arguments it seems like)
int	ms_export(char *arg)
{
	if (!arg)
		ms_env_alpha();
	return (0);
}

/* TEST MAIN */

int	main(void)
{
	extern char	**environ;

	ms_export(NULL);
}
