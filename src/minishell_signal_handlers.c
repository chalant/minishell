/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal_handlers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:02:50 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/19 19:18:38 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_new_prompt(int sig)
{
	pid_t	check;

	if (sig != SIGINT)
		return ;
	g_global_state.status = 130;
	// if (g_global_state.prompt)
	// {
	// 	g_global_state.prompt = 0;
	// 	// rl_done = 1;
	// 	// rl_free_line_state();
	// 	// rl_stuff_char(-1);
	// 	// rl_pending_input = -1;
	// 	// rl_read_key();
	// 	// rl_close();
	// 	return ;
	// }
	check = wait3(NULL, WNOHANG, NULL);
	if (check == -1)
		write(STDOUT_FILENO, "^C", 2);
	write(STDOUT_FILENO, "\n", 1);
	if (check > -1)
		return ;
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

static void	ms_kill_pid(int sig)
{
	pid_t	check;

	if (sig != SIGQUIT)
		return ;
	check = wait3(NULL, WNOHANG, NULL);
	if (check > -1)
	{
		// kill(check, SIGQUIT);
		printf("Quit: %i\n", SIGQUIT);
		g_global_state.status = 131;
	}
}

int	ms_set_signals(t_ms_context *data)
{
	extern int	rl_catch_signals;

	rl_catch_signals = 0;
	ft_bzero(&(data->act_sigint), sizeof(struct sigaction));
	ft_bzero(&(data->act_sigquit), sizeof(struct sigaction));
	data->act_sigint.sa_handler = ms_new_prompt;
	data->act_sigquit.sa_handler = ms_kill_pid;
	if (sigaction(SIGINT, &(data->act_sigint), NULL)
		|| sigaction(SIGQUIT, &(data->act_sigquit), NULL))
		return (1);
	return (0);
}
