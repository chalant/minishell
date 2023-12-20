/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 14:00:30 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/20 18:48:20 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global_states	g_global_state;

// free's 'line'
static int	ms_add_herstory(char *line)
{
	if (*line)
		add_history(line);
	free(line);
	return (0);
}

static int	build_execute(t_ms_context *data,
	t_parse_tree *tree, t_darray *command_array)
{
	int			i;
	t_command	*command;

	reset_commands(command_array);
	command = build_command(command_array, tree);
	if (!command)
		return (-1);
	i = -1;
	while (++i < command_array->actual_size)
		((t_command *)ft_darray_get(command_array, i))->context = data;
	return (start_execution(command));
}

static int	ms_recognize_execute(t_ms_context *data)
{
	int	reco_check;

	reco_check = recognize_input(&(data->parse_data), data);
	if (reco_check < 0)
		return (-1);
	if (reco_check != 2)
	{
		if (parse_input(&(data->parse_data), &(data->tree)) < 0)
			return (-1);
		data->status = build_execute(data, &(data->tree), &(data->commands));
		if (data->status == ERR_NOMEM)
			return (ms_perror(NULL, NULL, "FATAL MEMORY ERROR", 0));
		return (ms_add_herstory(data->line));
	}
	data->status = reco_check;
	return (ms_add_herstory(data->line));
}

// free's 'line'
static int	ms_process_line(t_ms_context *data, t_tkn_info *info)
{
	int	token_check;

	if (g_global_state.status)
	{
		data->status = g_global_state.status;
		g_global_state.status = 0;
	}
	token_check = ms_tokeniser(&data->line, data->parse_data.tokens, info);
	if (token_check == ERR_SIGINT)
		return (ms_add_herstory(data->line));
	if (token_check)
		return (token_check);
	if (data->parse_data.tokens->size == 1)
	{
		data->status = 0;
		return (0);
	}
	return (ms_recognize_execute(data));
}

int	main(void)
{
	t_ms_context	data;
	t_tkn_info		info;

	if (ms_startup(&data))
		return (1);
	ms_token_info(&info, RESERVED_SINGLE, RESERVED_DOUBLE, RESERVED_SKIP);
	data.line = readline(MS_PROMPT_MSG);
	while (data.line)
	{
		if (ms_process_line(&data, &info))
			ms_flush_exit(&data, 1);
		reset_parse_data(&data.parse_data, &data.tree);
		data.line = readline(MS_PROMPT_MSG);
	}
	if (g_global_state.status)
		data.status = g_global_state.status;
	ms_flush_exit(&data, data.status);
}
