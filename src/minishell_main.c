/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 14:00:30 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/31 13:55:54 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	pid;

// if exit_value == -5, flushes all then just returns (not in use i think)
void	ms_exit(t_shellshock *data, int exit_value)
{
	rl_clear_history();
	ft_clear_ds(data->env);
	if (exit_value == -5)
		return ;
	write(STDOUT_FILENO, "exit\n", 5);
	exit(exit_value);
}

// returns 1 if something was executed
int	ms_parse_builtins(t_shellshock *data, char *line)
{
	if (!ft_strncmp(line, "exit", 5))
	{
		free(line);
		ms_exit(data, 0);
	}
	if (!ft_strncmp(line, "cd ", 3))
		ms_cd(data, line + 3);
	else if (!ft_strncmp(line, "pwd", 4))
		ms_pwd(data);
	else if (!ft_strncmp(line, "export", 7))
		ms_export(data, NULL);
	else if (!ft_strncmp(line, "export ", 7))
		ms_export(data, ft_split(line + 7, ' '));
	else if (!ft_strncmp(line, "unset ", 6))
		ms_unset(data, ft_split(line + 6, ' '));
	else if (!ft_strncmp(line, "env", 4))
		ms_env();
	else if (!ft_strncmp(line, "echo ", 5))
		ms_echo(ft_split(line + 5, ' '));
	else if (!ft_strncmp(line, "echo", 5))
		ms_echo(NULL);
	else
		return (0);
	return (1);
}

static void	ms_sleep(char *line)
{
// TEST FUNCTION
	extern char	**environ;
	char		*argv[2];

	pid = fork();
	if (!pid)
	{
		free(line);
		rl_clear_history();
		argv[0] = "./sleep";
		argv[1] = NULL;
		execve("sleep", argv, environ);
		exit(1);
	}
	if (pid)
		waitpid(pid, NULL, 0);
		pid = 0;
}

// should free line
int	ms_process_line(t_shellshock *data, char *line)
{
	if (ms_parse_builtins(data, line))
		(void) line;
	else if (!ft_strncmp(line, "sleep", 6))
		ms_sleep(line);
// do more stuff, just have some fun with it :)
	if (*line)
		add_history(line);
	free(line);
	return (0);
}

void	ms_new_prompt(int sig)
{
	if (sig != SIGINT)
		return ;
	if (!pid)
		write(STDOUT_FILENO, "^C", 2);
	write(STDOUT_FILENO, "\n", 1);
	if (pid)
		return ;
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	ms_kill_pid(int sig)
{
	if (sig != SIGQUIT)
		return ;
	if (pid)
	{
		kill(pid, SIGQUIT);
		printf("Quit: %i\n", SIGQUIT);
	}
}

int	main(void)
{
	t_shellshock		data;
	char				*line;
	extern int			rl_catch_signals;
	struct sigaction	act_sigint;
	struct sigaction	act_sigquit;

	rl_catch_signals = 0;
	data.env_excess = 0;
	if (ms_envcpy(&data))
		return (1);
// malloc fail
	ft_bzero(&act_sigint, sizeof(struct sigaction));
	ft_bzero(&act_sigquit, sizeof(struct sigaction));
	act_sigint.__sigaction_u.__sa_handler = ms_new_prompt;
	act_sigquit.__sigaction_u.__sa_handler = ms_kill_pid;
	sigaction(SIGINT, &act_sigint, NULL);
	sigaction(SIGQUIT, &act_sigquit, NULL);
	line = readline(MS_PROMPT);
	while (line)
	{
		ms_process_line(&data, line);
		line = readline(MS_PROMPT);
	}
	ms_exit(&data, 0);
}
