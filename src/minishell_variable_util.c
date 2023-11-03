/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_variable_util.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:44:01 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/20 18:42:48 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NOT TESTED YET
// 'var' is of type [name]=[value]
// returns the ptr of a variable in env
// (points to the name, unlike getenv() which points to the value)
// returns NULL if no variable is found
char	*ms_get_var_env(char *var)
{
	extern char	**environ;
	int			i;
	int			name_len;

	if (!var || !*var)
		return (NULL);
	name_len = 0;
	while (var[name_len] != '=')
		name_len++;
	i = 0;
	while (environ[i])
	{
		if (!ft_strncmp(environ[i], var, name_len + 1))
			return (environ[i]);
		i++;
	}
	return (NULL);
}
