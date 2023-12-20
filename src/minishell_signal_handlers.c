/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal_handlers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:02:50 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/20 18:03:31 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "sys/ioctl.h"

static void	ms_new_prompt(int sig)
{
	pid_t	check;

	if (sig != SIGINT)
		return ;
	g_global_state.status = 130;
	check = wait3(NULL, WNOHANG, NULL);
	if (check == -1)
		write(STDOUT_FILENO, "^C", 2);
	if (check > -1)
		return ;
	if (g_global_state.prompt)
	{
		rl_replace_line("", 1);
		ioctl(STDIN_FILENO, TIOCSTI, "\x04");
		g_global_state.prompt = 0;
		return ;
	}
	write(STDOUT_FILENO, "\n", 1);
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
