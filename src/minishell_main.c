/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 14:00:30 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/30 23:04:43 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	pid;

// 'tokens' should be terminated with a newly initialised token
static char **ms_convert_tokens_arg(t_darray *tokens)
{
	char	**arg;
	int		i;

	arg = malloc(sizeof(char *) * (tokens->size));
	if (!arg)
		return (NULL);
	i = 0;
	while (i + 1 < tokens->size)
	{
		arg[i] = ft_strdup((((t_token *) tokens->contents) + i)->string);
		if (!arg[i])
		{
			ft_clear_ds(arg);
			return (NULL);
		}
		i++;
	}
	arg[i] = NULL;
	return (arg);
}

// returns 1 if something was executed
int	ms_parse_builtins(t_shellshock *data, char **arg, char *line)
{
	if (!ft_strncmp(*arg, "exit", 3))
	{
		ms_exit(data, arg, line);
	}
	if (!ft_strncmp(*arg, "cd", 3))
		ms_cd(data, arg);
	else if (!ft_strncmp(*arg, "pwd", 4))
		ms_pwd();
	else if (!ft_strncmp(*arg, "export", 7))
		ms_export(data, arg);
	else if (!ft_strncmp(*arg, "unset", 6))
		ms_unset(data, arg);
	else if (!ft_strncmp(*arg, "env", 4))
		ms_env();
	else if (!ft_strncmp(*arg, "echo", 5))
		ms_echo(arg);
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
	waitpid(pid, NULL, 0);
	pid = 0;
}

// free's 'line'
static int	ms_add_herstory(char *line)
{
	if (*line)
		add_history(line);
	free(line);
	return (0);
}

// should free 'line'
int	ms_process_line(t_shellshock *data, char *line)
{
	t_token_info	info;
	t_darray		tokens;
	char			**arg;

if (!ft_strncmp(line, "sleep", 6))
{
	ms_sleep(line);
	return (ms_add_herstory(line));
}
	ft_darray_init(&tokens, sizeof(t_token), 20);
	if (ms_tokeniser(&line, &tokens, ms_token_info(&info,
				RESERVED_SINGLE, RESERVED_DOUBLE, RESERVED_SKIP)))
	{
		ft_darray_delete(&tokens, ms_clear_token);
		free(line);
		return (1);
	}
// int i = 0;
// while (i < tokens.size)
// {
// 	printf("|%s|\n", (((t_token *) tokens.contents) + i)->string);
// 	i++;
// }
	arg = ms_convert_tokens_arg(&tokens);
	ft_darray_delete(&tokens, ms_clear_token);
	if (!arg)
		return (ms_perror("create arg", NULL, NULL, errno));
// malloc error
	ms_parse_builtins(data, arg, line);
	ft_clear_ds(arg);
	return (ms_add_herstory(line));
	(void) data;
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
// set $? to 130 or get it from waitpid that would probs be better..
}

void	ms_kill_pid(int sig)
{
	if (sig != SIGQUIT)
		return ;
	if (pid)
	{
		kill(pid, SIGQUIT);
		printf("Quit: %i\n", SIGQUIT);
// set $? to 131 or get it from waitpid that would probs be better..
	}
}

static void	ms_set_signals(t_shellshock *data)
{
	extern int			rl_catch_signals;

	rl_catch_signals = 0;
	ft_bzero(&(data->act_sigint), sizeof(struct sigaction));
	ft_bzero(&(data->act_sigquit), sizeof(struct sigaction));
	data->act_sigint.__sigaction_u.__sa_handler = ms_new_prompt;
	data->act_sigquit.__sigaction_u.__sa_handler = ms_kill_pid;
	sigaction(SIGINT, &(data->act_sigint), NULL);
	sigaction(SIGQUIT, &(data->act_sigquit), NULL);
}

int	main(void)
{
	t_shellshock		data;
	char				*line;

	data.env_excess = 0;
	if (ms_envcpy(&data))
		return (1);
// malloc fail
	ms_set_signals(&data);
	line = readline(MS_PROMPT_MSG);
	while (line)
	{
		if (ms_process_line(&data, line))
			ms_flush_exit(&data, 1);
		line = readline(MS_PROMPT_MSG);
	}
	ms_flush_exit(&data, 0);
}
