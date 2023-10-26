/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:55:14 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/26 17:25:11 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_pwd(t_shellshock *data)
{
	char	*buf;

	buf = getcwd(NULL, 0);
if (!buf)
	exit(1);
(void) data;
// use data to free stuff and exit?
	write (STDOUT_FILENO, buf, ft_strlen(buf));
	write (STDOUT_FILENO, "\n", 1);
	free (buf);
}

/* TEST MAIN */

// int	main(void)
// {
// 	ms_pwd(NULL);
// }
