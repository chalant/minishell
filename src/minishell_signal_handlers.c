#include "minishell.h"

static void	ms_new_prompt(int sig)
{
	pid_t	check;

	check = wait3(NULL, WNOHANG, NULL);
	if (sig != SIGINT)
		return ;
	if (check == -1)
		write(STDOUT_FILENO, "^C", 2);
	g_global_status = 130;
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

	check = wait3(NULL, WNOHANG, NULL);
	if (sig != SIGQUIT)
		return ;
	if (check > -1)
	{
		// kill(check, SIGQUIT);
		printf("Quit: %i\n", SIGQUIT);
		g_global_status = 131;
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
