/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:32:58 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/09 15:26:37 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// prints out env on std_out and returns 0
int	ms_env(void)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
	{
		write(1, environ[i], ft_strlen(environ[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

// returns 1 if malloc failed, else 0
int	ms_envcpy(t_ms_context *data)
{
	extern char	**environ;
	char		**new;
	int			i;

	i = 0;
	while (environ[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	if (!new)
		return (1);
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
	data->env = new;
	environ = new;
	return (0);
}

static char	**ms_realloc_error(char **ptr, char **new)
{
	ms_perror("ms_realloc", NULL, NULL, errno);
	if (ptr)
		ft_clear_ds(ptr);
	if (new)
		ft_clear_ds(new);
	return (NULL);
}

// for ENVIRON: call with data->env and assign environ after
// mallocs size of 'ptr' + 'add', copies content of 'ptr' over and clears it
// returns new 'ptr' or NULL if malloc failed (always clears 'ptr', prints msg)
char	**ms_realloc(char **ptr, int add)
{
	size_t	i;
	char	**new;

	if (!ptr)
		return (NULL);
	i = 0;
	while (ptr[i])
		i++;
	new = malloc(sizeof(char *) * (i + add + 1));
	if (!new)
		return (ms_realloc_error(ptr, NULL));
	i = 0;
	while (ptr[i])
	{
		new[i] = ft_strdup(ptr[i]);
		if (!new[i])
			return (ms_realloc_error(ptr, new));
		i++;
	}
	new[i] = NULL;
	ft_clear_ds(ptr);
	return (new);
}
