/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:20:20 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/22 15:48:08 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// seems to always return 1 on error
int	ms_cd(char **arg)
{
	char	*path;

	path = arg[1];
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
			return (ms_perror("cd", NULL, "HOME not set", 0));
	}
	else if (arg[2])
		return (ms_perror("cd", NULL, "too many arguments", 0));
	if (!*path)
		return (0);
	if (chdir(path))
		return (ms_perror("cd", path, NULL, errno));
	return (0);
}
