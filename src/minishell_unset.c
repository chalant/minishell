/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 18:31:09 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/20 18:49:33 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NOT TESTED YET
// returns 0 even if 'name' doesn't exist
int	ms_unset(char *name)
{
	char	**environ;
	int		i;
	int		name_len;

// check input? if it starts with '=' that's bad
// should this free(name)?
// should this resize **environ?
	i = 0;
	name_len = ft_strlen(name);
	while (environ[i])
	{
		if (!ft_strncmp(environ[i], name, name_len))
			if (environ[i][name_len] == '=')
				break ;
		i++;
	}
	if (!environ[i])
		return (0);
	free(environ[i]);
	while (environ[i + 1])
	{
		environ[i] = environ[i + 1];
		i++;
	}
	environ[i] = environ[i + 1];
	return (0);
}
