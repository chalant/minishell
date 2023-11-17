/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_variable_util.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:44:01 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/17 14:38:03 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// returns 1 if no good
// takes both [name]=[value] and just [name]
int	ms_check_varname(char *var)
{
	while (*var && *var != '=')
	{
		if ( !('a' <= *var && *var <= 'z') && !('A' <= *var && *var <= 'Z')
			&& *var != '_' && !('0' <= *var && *var <= '9'))
			return (1);
		var++;
	}
	return (0);
}

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
