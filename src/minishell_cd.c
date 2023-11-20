/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:20:20 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/17 17:00:35 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// doesn't free 'path' for now
int	ms_cd(char **arg)
{
// if more than one arg, bad
	if (arg[0] && arg[1])
		return (ms_perror("cd", NULL, "too many arguments", 0));
	if (chdir(*arg))
		return (ms_perror("cd", *arg, NULL, errno));
	return (0);
}
