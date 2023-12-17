/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 14:00:30 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/17 16:39:34 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_global_status;

// void	test_print_tokens(t_ms_context *data)
// {
// // TOKEN PRINT IN PROGRESS
// 	printf("## tokens: ");
// 	int i = 0;
// 	while (i < data->parse_data.tokens->size)
// 	{
// 		// printf("%2i|", ((t_token *)ft_darray_get(data->parse_data.tokens, i))->flags & IS_SPECIAL);
// 		printf("%6s, ", ((t_token *)ft_darray_get(data->parse_data.tokens, i))->string);
// 		i++;
// 	}
// 	printf("\n## masks : ");
// 	i = 0;
// 	while (i < data->parse_data.tokens->size)
// 	{
// 		printf("%6s, ", ((t_token *)ft_darray_get(data->parse_data.tokens, i))->mask_exp);
// 		i++;
// 	}
// 	printf("\n");
// // TOKEN PRINT IS OVER
// }


// free's 'line'
static int	ms_add_herstory(char *line)
{
	if (*line)
		add_history(line);
	free(line);
	return (0);
}

int	execute(t_ms_context *data, t_parse_tree *tree, t_darray *command_array)
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
	return (minishell_execute(command));
}

// should free 'line'
static int	ms_process_line(t_ms_context *data, t_token_info *info)
{
	if (g_global_status)
	{
		data->status = g_global_status;
		g_global_status = 0;
	}
	if (ms_tokeniser(&data->line, data->parse_data.tokens, info))
		return (-1);
	data->status = recognize_input(&(data->parse_data), data);
	if (data->status < 0)
		return (-1);
	if (data->status != 2)
	{
		if (parse_input(&(data->parse_data), &(data->tree)) < 0)
			return (-1);
		data->status = execute(data, &(data->tree), &(data->commands));
		if (data->status == ERR_NOMEM)
			return (data->status);
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
