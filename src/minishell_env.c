/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:32:58 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/20 18:51:14 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <crt_externs.h>

// prints out env on std_out and returns 0
int	ms_env()
{
	extern char	**environ;
	char		**envp;

	envp = environ;
	while(*envp)
	{
		write(1, *envp, ft_strlen(*envp));
		write(1, "\n", 1);
		envp++;
	}
	return (0);
}

// returns 1 if malloc failed, else 0
int	ms_envcpy(void)
{
	extern char	**environ;
	char		**new;
	int			i;

	i = 0;
	while (environ[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (environ[i])
	{
		new[i] = ft_strdup(environ[i]);
		if (!new[i])
		{
			ft_clear_ds(new);
			return (1);
		}
		i++;
	}
	new[i] = NULL;
	environ = new;
	return (0);
}

// mallocs size of 'ptr' + 'addon', copies content of 'ptr' over and clears it
// returns new 'ptr' or NULL if malloc failed (always clears 'ptr')
char	**ms_realloc(char **ptr, int addon)
{
	size_t	i;
	char	**new;

	if (!ptr)
		return (NULL);
	i = 0;
	while (ptr[i])
		i++;
	new = malloc(sizeof(char *) * (i + addon + 1));
	i = 0;
	while (ptr[i])
	{
		new[i] = ft_strdup(ptr[i]);
		if (!new[i])
		{
			ft_clear_ds(new);
			ft_clear_ds(ptr);
			return (NULL);
// malloc failed
		}
		i++;
	}
	new[i] = NULL;
	ft_clear_ds(ptr);
	return (new);
}

/* TEST MAIN */

// int	main(void)
// {
// 	extern char	**environ;

// 	// environ = ms_realloc(environ, 1);
// 	// if (!environ)
// 	// 	return (1);
// 	// i = 0;
// 	// while (environ[i])
// 	// 	i++;
// 	// environ[i] = ft_strdup("test=ait");
// 	// environ[i + 1] = NULL;
// 	if (ms_envcpy())
// 		return (1);
// 	ft_clear_ds(environ);
// }
