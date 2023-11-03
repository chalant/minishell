/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:55:14 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/27 16:19:44 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_pwd(t_shellshock *data)
{
	char	*buf;

	buf = getcwd(NULL, 0);
if (!buf)
{
(void) data;
// use data to free stuff and exit?
	exit(1);
}
	write(STDOUT_FILENO, buf, ft_strlen(buf));
	write(STDOUT_FILENO, "\n", 1);
	free(buf);
}
