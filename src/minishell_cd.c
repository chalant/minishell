/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:20:20 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/26 17:32:10 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// should this free 'path'?
void	ms_cd(t_shellshock *data, char *path)
{
	if (!chdir(path))
		return ;
// something went wrong!
write (1, "oopsie\n", 7);
(void)data;
	exit (1);
}

/* TEST MAIN */

// int	main(int ac, char **av)
// {
// 	char	*buf;

// 	if (ac != 2)
// 		return (1);
// 	ms_cd(NULL, av[1]);
// 	buf = getcwd(NULL, 0);
// 	printf ("now in %s\n", buf);
// 	free (buf);
// }
