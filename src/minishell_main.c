/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 14:00:30 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/18 19:48:08 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_global_status;

// free's 'line'
static int	ms_add_herstory(char *line)
{
	if (*line)
		add_history(line);
	free(line);
	return (0);
}

int	build_and_execute(t_ms_context *data, t_parse_tree *tree, t_darray *command_array)
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

// should free 'line'
static int	ms_process_line(t_ms_context *data, t_token_info *info)
{
	int	check;

	if (g_global_status)
	{
		data->status = g_global_status;
		g_global_status = 0;
	}
	if (ms_tokeniser(&data->line, data->parse_data.tokens, info))
		return (-1);
	check = recognize_input(&(data->parse_data), data);
	if (check < 0)
		return (-1);
	if (check != 2)
	{
		if (parse_input(&(data->parse_data), &(data->tree)) < 0)
			return (-1);
		data->status = build_and_execute(data, &(data->tree), &(data->commands));
		if (data->status == ERR_NOMEM)
			return (ms_perror(NULL, NULL, "FATAL MEMORY ERROR", 0));
		return (ms_add_herstory(data->line));
	}
	return (ms_add_herstory(data->line));
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
	data.status = 0;
	if (ms_envcpy(&data))
		return (ms_perror("malloc", NULL, NULL, errno));
	if (ms_set_signals(&data))
		return (ms_perror("sigaction", NULL, NULL, errno));
// todo: error management
	ms_init_parse(&data);
	ms_token_info(&info, RESERVED_SINGLE, RESERVED_DOUBLE, RESERVED_SKIP);
	data.line = readline(MS_PROMPT_MSG);
	while (data.line)
	{
		if (ms_process_line(&data, &info))
			ms_flush_exit(&data, 1);
		reset_parse_data(&data.parse_data, &data.tree);
		data.line = readline(MS_PROMPT_MSG);
	}
	if (g_global_status)
		data.status = g_global_status;
	ms_flush_exit(&data, data.status);
}
