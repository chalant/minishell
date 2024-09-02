/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:53 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/21 14:53:35 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	build_chart(t_darray *sets, t_graph *graph, int size)
{
	int				i;
	int				j;
	t_ms_edge		edge;
	t_earley_item	*item;
	t_earley_set	*set;

	i = -1;
	while (++i < size)
	{
		j = -1;
		set = ft_darray_get(sets, i);
		while (++j < set->items->size)
		{
			item = ft_darray_get(set->items, j);
			if (item->completed)
			{
				edge.finish = i;
				edge.start = item->start;
				edge.rule = item->rule;
				add_edge(graph, edge.start, &edge);
			}
		}
	}
	return (0);
}

int	update_parsing_data(t_parsing_data *data, int size)
{
	int	i;

	i = -1;
	while (++i < size)
	{
		add_earley_set(data->earley_sets, 10);
		add_adjacency_list(data->chart, sizeof(t_ms_edge), 10);
	}
	return (0);
}

int	is_completed(t_darray *items)
{
	t_earley_item	*item;
	int				i;

	i = -1;
	while (++i < items->size)
	{
		item = ft_darray_get(items, i);
		if (item->completed && item->start == 0)
			return (1);
	}
	return (0);
}

int	parse_input(t_parsing_data *data, t_parse_tree *tree)
{
	tree->start = 0;
	tree->rule_name = data->grammar->start_rule;
	tree->end = data->input_length - 1;
	tree->terminal = 0;
	ms_start_rule(tree, data);
	if (build_parse_tree(tree, data) < 0)
		return (-1);
	return (0);
}

/*
attempts to match the input with the grammar.
returns 2 if there is a syntax error
returns -1 if there is an internal error
returns 0 otherwise
*/
int	recognize_input(t_parsing_data *data, void *context)
{
	t_earley_set	*last_set;

	update_parsing_data(data, data->tokens->size);
	if (build_earley_items(data, context) < 0)
		return (-1);
	last_set = ft_darray_get(data->earley_sets, data->earley_sets->size - 1);
	if (!last_set->items->size || !is_completed(last_set->items))
	{
		if (g_global_state.status != 130)
			ms_message_header(data, ms_syntax_error, 2);
		return (2);
	}
	if (build_chart(data->earley_sets, data->chart, data->tokens->size) < 0)
		return (-1);
	data->input_length = data->tokens->size;
	data->chart_size = data->tokens->size;
	return (0);
}
