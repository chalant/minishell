/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 14:00:30 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/04 14:42:29 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 'tokens' should be terminated with a newly initialised token
// static char **ms_convert_tokens_arg(t_darray *tokens)
// {
// 	char	**arg;
// 	int		i;

// 	arg = malloc(sizeof(char *) * (tokens->size));
// 	if (!arg)
// 		return (NULL);
// 	i = 0;
// 	while (i + 1 < tokens->size)
// 	{
// 		arg[i] = ft_strdup((((t_token *) tokens->contents) + i)->string);
// 		if (!arg[i])
// 		{
// 			ft_clear_ds(arg);
// 			return (NULL);
// 		}
// 		i++;
// 	}
// 	arg[i] = NULL;
// 	return (arg);
// }

// returns 1 if something was executed
// static int	ms_parse_builtins(t_shellshock *data, char **arg, char *line)
// {
// 	if (!ft_strncmp(*arg, "exit", 3))
// 	{
// 		ms_exit(data, arg, line);
// 	}
// 	if (!ft_strncmp(*arg, "cd", 3))
// 		ms_cd(data, arg);
// 	else if (!ft_strncmp(*arg, "pwd", 4))
// 		ms_pwd();
// 	else if (!ft_strncmp(*arg, "export", 7))
// 		ms_export(data, arg);
// 	else if (!ft_strncmp(*arg, "unset", 6))
// 		ms_unset(data, arg);
// 	else if (!ft_strncmp(*arg, "env", 4))
// 		ms_env();
// 	else if (!ft_strncmp(*arg, "echo", 5))
// 		ms_echo(arg);
// 	else
// 		return (0);
// 	return (1);
// }

// static void	ms_sleep(char *line)
// {
// // TEST FUNCTION
// 	extern char	**environ;
// 	char		*argv[2];

// 	pid = fork();
// 	if (!pid)
// 	{
// 		free(line);
// 		rl_clear_history();
// 		argv[0] = "./sleep";
// 		argv[1] = NULL;
// 		execve("sleep", argv, environ);
// 		exit(1);
// 	}
// 	waitpid(pid, NULL, 0);
// 	pid = 0;
// }

// free's 'line'
static int	ms_add_herstory(char *line)
{
	if (*line)
		add_history(line);
	free(line);
	return (0);
}

// should free 'line'
static int	ms_process_line(t_ms_context *data, char *line)
{
	t_token_info	info;
	// t_darray		tokens;
	// char			**arg;

// if (!ft_strncmp(line, "exit", 5))
// ms_flush_exit(data, data->status);
	// ft_darray_init(data->parse_data.tokens, sizeof(t_token), 20);
	if (ms_tokeniser(&line, data->parse_data.tokens, ms_token_info(&info,
				RESERVED_SINGLE, RESERVED_DOUBLE, RESERVED_SKIP)))
	{
		ft_darray_delete(data->parse_data.tokens, ms_clear_token);
		free(line);
		return (1);
	}
	recognize_input(&(data->parse_data));
	parse_input(&(data->parse_data), &(data->tree));
	data->status = execute(data, &(data->tree), &(data->commands));
	reset_parse_data(&(data->parse_data), &(data->tree));
// arg = ms_convert_tokens_arg(&tokens);
// ft_darray_delete(&tokens, ms_clear_token);
// if (!arg)
// 	return (ms_perror("create arg", NULL, NULL, errno));
// malloc error
// ms_parse_builtins(data, arg, line);
// ft_clear_ds(arg);
	return (ms_add_herstory(line));
}

void	ms_new_prompt(int sig)
{
	pid_t	check;

	check = wait3(NULL, WNOHANG, NULL);
	if (sig != SIGINT)
		return ;
	if (check == -1)
		write(STDOUT_FILENO, "^C", 2);
	write(STDOUT_FILENO, "\n", 1);
	if (check > -1)
	{
		g_global_status = 130;
		return ;
// set $? to 130 or get it from waitpid that would probs be better..
	}
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	ms_kill_pid(int sig)
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
// set $? to 131 or get it from waitpid that would probs be better..
	}
}

static void	ms_set_signals(t_ms_context *data)
{
	extern int			rl_catch_signals;

	rl_catch_signals = 0;
	ft_bzero(&(data->act_sigint), sizeof(struct sigaction));
	ft_bzero(&(data->act_sigquit), sizeof(struct sigaction));
	data->act_sigint.sa_handler = ms_new_prompt;
	data->act_sigquit.sa_handler = ms_kill_pid;
	// data->act_sigint.__sigaction_u.__sa_handler = ms_new_prompt;
	// data->act_sigquit.__sigaction_u.__sa_handler = ms_kill_pid;
	sigaction(SIGINT, &(data->act_sigint), NULL);
	sigaction(SIGQUIT, &(data->act_sigquit), NULL);
}

static int	ms_init_parse(t_ms_context *data)
{
	init_parse_data(&(data->parse_data));
	if (alloc_parse_data(&(data->parse_data), 20) < 0)
	{
		free_parse_data(&(data->parse_data), NULL);
		return (1);
	}
	if (ft_darray_init(&(data->commands), sizeof(t_command), 10) < 0)
		return (1);
	data->tree.children = NULL;
	return (0);
}

int	main(void)
{
	t_ms_context	data;

	g_global_status = 0;
	data.env_excess = 0;
	if (ms_envcpy(&data))
		return (1);
// malloc fail
	ms_set_signals(&data);
	data.status = 0;
	ms_init_parse(&data);
	data.line = readline(MS_PROMPT_MSG);
	while (data.line)
	{
		if (ms_process_line(&data, data.line))
			ms_flush_exit(&data, 1);
		data.line = readline(MS_PROMPT_MSG);
	}
	ms_flush_exit(&data, 0);
}
