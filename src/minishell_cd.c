/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:20:20 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/27 16:18:43 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// doesn't free 'path' for now
void	ms_cd(t_shellshock *data, char *path)
{
	if (!chdir(path))
		return ;
// something went wrong!
write(1, "oopsie\n", 7);
(void)data;
	exit(1);
}
