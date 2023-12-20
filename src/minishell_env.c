/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:32:58 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/20 17:58:09 by bvercaem         ###   ########.fr       */
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

	data->env_excess = 0;
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
			return (1);
		i++;
	}
	new[i] = NULL;
	data->env = new;
	environ = new;
	return (0);
}

// for ENVIRON: call with data->env and assign environ after
// mallocs size of 'ptr' + 'add', free's 'ptr'
// error: NULL, malloc fail, clears 'ptr'
char	**ms_realloc(char **ptr, int add)
{
	size_t	i;
	char	**new;

	i = 0;
	while (ptr && ptr[i])
		i++;
	new = malloc(sizeof(char *) * (i + add + 1));
	if (!new)
	{
		ft_clear_ds(ptr);
		return (NULL);
	}
	i = 0;
	while (ptr && ptr[i])
	{
		new[i] = ptr[i];
		i++;
	}
	new[i] = NULL;
	if (ptr)
		free(ptr);
	return (new);
}
