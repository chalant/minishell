/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 14:00:30 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/25 18:24:08 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	pid;

void	ms_exit(int exit_value)
{
	write(STDOUT_FILENO, "exit\n", 5);
	rl_clear_history();
	exit(exit_value);
}

// should free line
int	ms_process_line(char *line)
{
	if (!ft_strncmp(line, "exit", 5))
	{
		free(line);
		ms_exit(0);
	}
	if (!ft_strncmp(line, "sleep", 6))
	{
		pid = fork ();
		if (!pid)
		{
			free (line);
			rl_clear_history();
			execve ("sleep", NULL, NULL);
			exit (1);
		}
		if (pid)
			waitpid(pid, NULL, 0);
		pid = 0;
	}
// do more stuff :)
	if (*line)
		add_history(line);
	free(line);
	return (0);
}

void	ms_new_prompt(int sig)
{
	(void) sig;
	write(STDOUT_FILENO, "\n", 1);
	if (pid)
		return ;
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	ms_kill_pid(int sig)
{
	(void) sig;
	if (pid)
	{
		kill (pid, SIGQUIT);
		write (STDOUT_FILENO, "Quit: 3\n", 8);
	}
}

int	main(void)
{
	char				*line;
	extern int			rl_catch_signals;
	struct sigaction	act_sigint;
	struct sigaction	act_sigquit;

	pid = 0;
	rl_catch_signals = 0;
	ft_bzero(&act_sigint, sizeof(struct sigaction));
	act_sigint.__sigaction_u.__sa_handler = ms_new_prompt;
	sigaction(SIGINT, &act_sigint, NULL);
	ft_bzero(&act_sigquit, sizeof(struct sigaction));
	act_sigquit.__sigaction_u.__sa_handler = ms_kill_pid;
	sigaction(SIGQUIT, &act_sigquit, NULL);
	line = readline(MS_PROMPT);
	while (line)
	{
		ms_process_line(line);
		line = readline(MS_PROMPT);
	}
	ms_exit(0);
}
