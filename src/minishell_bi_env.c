/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bi_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:32:58 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/17 18:05:51 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <crt_externs.h>

// void	ms_change()
// {
// 	extern int	findme;
// 	findme = 3;
// }

// void	ms_env()
// {
// 	char	**envp = environ;

// 	if (!envp)
// 		exit (1);
// 	while(*envp)
// 	{
// 		write(1, *envp, ft_strlen(*envp));
// 		write(1, "\n", 1);
// 		envp++;
// 	}
// }

/* TEST MAIN */

int	main(int ac, char *av[], char **env)
{
	(void)ac;
	(void)av;
	env = malloc(10);
	// *environ = ft_strdup("testt=hi");
	// ms_env();
	printf("%s", getenv("testt"));
	free(env);
	// int	findme = 5;
	// printf("%i\n", findme);
	// ms_change();
	// printf("%i\n", findme);
}
