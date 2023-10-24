/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 14:00:30 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/24 19:12:28 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_exit(void)
{
	write(STDOUT_FILENO, "exit\n", 5);
	rl_clear_history();
	exit(0);
}

// should free input
int	ms_process_input(char *input)
{
	if (!ft_strncmp(input, "exit", 5))
	{
		free(input);
		ms_exit();
	}
// do more stuff :)
if (!ft_strncmp(input, "re", 3))
{
}
write(1, input, ft_strlen(input));
write(1, "\n", 1);
	free(input);
	return (0);
}

void	ms_new_prompt(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

// C-d (EOF) with no text prints "exit\n" and exits
int	main(void)
{
	char	*input;
	struct sigaction	act;

	act.__sigaction_u.__sa_handler = ms_new_prompt;
	sigaction(SIGINT, &act, NULL);
	input = readline(MS_PROMPT);
	while (input)
	{
		ms_process_input(input);
		input = readline(MS_PROMPT);
	}
// try to get rid of the newline?
	ms_exit();
}
