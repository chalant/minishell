/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_treebuilder_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:26:53 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 16:39:03 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parser_state	next_state(int depth, int node, t_ms_rule *rule)
{
	t_parser_state	state;

	state.depth = depth;
	state.node = node;
	state.rule = rule;
	return (state);
}

t_parse_tree	*get_subtree(t_parse_tree *tree, int index)
{
	t_parse_tree	subtree;
	int				i;

	if (index >= tree->children->actual_size)
	{
		i = -1;
		while (++i < index + 1 - tree->children->max_size)
		{
			init_tree(&subtree);
			if (ft_darray_append(tree->children, &subtree) < 0)
				return (NULL);
		}
	}
	return (ft_darray_get(tree->children, index));
}

void	set_subtree(t_parse_tree *subtree, char *rule_name, int start, int end)
{
	subtree->rule_name = rule_name;
	subtree->start = start;
	subtree->end = end;
}

int	ms_start_search(t_parse_tree *parse_tree, t_parsing_data *data, int rule)
{
	t_parser_state	state;

	state.depth = 0;
	state.node = parse_tree->start;
	state.rule = data->grammar->rules[rule];
	parse_tree->rule = rule;
	return (ms_search_core(parse_tree, data, state));
}

int	fill_parse_tree(t_parse_tree *parse_tree, t_parsing_data *data)
{
	int				i;
	t_ms_edge		*edge;
	t_darray		*edges;

	i = -1;
	edges = get_edges(data->chart, parse_tree->start);
	while (++i < edges->size)
	{
		edge = (t_ms_edge *)edges->contents + i;
		if (edge->finish == parse_tree->end && ft_strcmp(parse_tree->rule_name,
				data->grammar->rules[edge->rule]->name) == 0)
			if (ms_start_search(parse_tree, data, edge->rule) < 0)
				return (-1);
	}
	return (1);
}
