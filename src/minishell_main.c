/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 14:00:30 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/15 17:59:16 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_global_status;

// TOKEN PRINT IN PROGRESS
// printf("tokens after tokenising:\ntokens: ");
// int i = 0;
// while (i < data->parse_data.tokens->size)
// {
// 	printf("%2i|", ((t_token *)ft_darray_get(data->parse_data.tokens, i))->flags & IS_SPECIAL);
// 	printf("%6s, ", ((t_token *)ft_darray_get(data->parse_data.tokens, i))->string);
// 	i++;
// }
// printf("\nmasks : ");
// i = 0;
// while (i < data->parse_data.tokens->size)
// {
// 	printf("%9s, ", ((t_token *)ft_darray_get(data->parse_data.tokens, i))->mask_exp);
// 	i++;
// }
// printf("\n\n");
// TOKEN PRINT IS OVER


// free's 'line'
static int	ms_add_herstory(char *line)
{
	if (*line)
		add_history(line);
	free(line);
	return (0);
}

// should free 'line'
static int	ms_process_line(t_ms_context *data, t_token_info *info)
{
	int	recogniser_status;

	if (g_global_status)
	{
		data->status = g_global_status;
		g_global_status = 0;
	}
	if (ms_tokeniser(&data->line, data->parse_data.tokens, info))
	{
		ft_darray_delete(data->parse_data.tokens, ms_clear_token);
		return (1);
	}
	recogniser_status = recognize_input(&(data->parse_data));
	if (recogniser_status < 0)
		return (-1);
	if (recogniser_status != 2)
	{
		parse_input(&(data->parse_data), &(data->tree));
		data->status = execute(data, &(data->tree), &(data->commands));
		return (ms_add_herstory(data->line));
	}
	data->status = 2;
	return (ms_add_herstory(data->line));
}

void	ms_new_prompt(int sig)
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
	}
}

static int	ms_set_signals(t_ms_context *data)
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
	t_token_info	info;

	g_global_status = 0;
	data.env_excess = 0;
	if (ms_envcpy(&data))
		return (ms_perror("malloc", NULL, NULL, errno));
	if (ms_set_signals(&data))
		return (ms_perror("sigaction", NULL, NULL, errno));
	data.status = 0;
// todo: error management
	ms_init_parse(&data);
	ms_token_info(&info, RESERVED_SINGLE, RESERVED_DOUBLE, RESERVED_SKIP);
	data.line = readline(MS_PROMPT_MSG);
	while (data.line)
	{
		if (ms_process_line(&data, &info) || data.status == ERR_NOMEM)
			ms_flush_exit(&data, 1);
		reset_parse_data(&data.parse_data, &data.tree);
		data.line = readline(MS_PROMPT_MSG);
	}
	ms_flush_exit(&data, 0);
}
