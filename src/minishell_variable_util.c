/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_variable_util.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:44:01 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/04 14:13:56 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// returns 1 if no good (includes digit-start)
// takes both [name]=[value] and just [name]
int	ms_check_varname(char *var)
{
	if ('0' <= *var && *var <= '9')
		return (1);
	while (*var && *var != '=')
	{
		if ( !('a' <= *var && *var <= 'z') && !('A' <= *var && *var <= 'Z')
			&& *var != '_' && !('0' <= *var && *var <= '9'))
			return (1);
		var++;
	}
	return (0);
}

// 'var' is of type [name]=[value] or just [name]
// returns an incremented copy of data->env, double-pointing to a name
// returns NULL if no variable is found
char	**ms_get_var_envp(t_ms_context *data, char *var)
{
	char	**envp;
	int		i;
	int		name_len;

	envp = data->env;
	if (!var || !*var)
		return (NULL);
	name_len = 0;
	while (var[name_len] && var[name_len] != '=')
		name_len++;
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var, name_len))
			if (envp[i][name_len] == '=')
				return (envp + i);
		i++;
	}
	return (NULL);
}

// 'var' is of type [name]=[value] or just [name]
// returns the ptr of a variable in data->env
// (points to the name, unlike getenv() which points to the value)
// returns NULL if no variable is found
char	*ms_get_var_env(t_ms_context *data, char *var)
{
	char	**envp;

	envp = ms_get_var_envp(data, var);
	if (!envp)
		return (NULL);
	return (*envp);
}
