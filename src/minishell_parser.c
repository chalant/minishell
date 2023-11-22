/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:26 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/21 18:07:36 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>

int	ms_search_core(t_parse_tree *tree, t_parsing_data *data, t_parser_state state)
{
	int				i;
	t_parser_state	nstate;
	t_ms_symbol		*symbol;
	t_ms_edge		*item;
	t_parse_tree	subtree;

	if (state.depth >= state.rule->length - 1)
		return (1);
	symbol = state.rule->symbols[state.depth];
	subtree.terminal = 0;
	subtree.rule_name = NULL;
	subtree.children = NULL;
	if (symbol->symbol_type == MS_TERMINAL_SYMBOL)
	{
		subtree.terminal = 1;
		if (state.node >= tree->end)
			return (0);
		else if (symbol->match(symbol, (t_token *)ft_darray_get(data->tokens, state.node)))
		{
			subtree.rule_name = ((t_token *)ft_darray_get(data->tokens, state.node))->string;
			subtree.start = state.node - 1;
			subtree.end = state.node;
			ft_darray_set(tree->children, &subtree, state.depth);
			nstate.depth = state.depth + 1;
			nstate.rule = state.rule;
			nstate.node = state.node + 1;
			if (state.node < data->input_length)
				ms_search_core(tree, data, nstate);
			return (1);
		}
		return (0);
	}
	else if (symbol->symbol_type == MS_NON_TERMINAL_SYMBOL)
	{
		item = (t_ms_edge *)data->chart->adjacency_list[state.node]->contents;
		i = -1;
		while (++i < data->chart->adjacency_list[state.node]->size)
		{
			if (strcmp(data->grammar->rules[(item + i)->rule]->name, symbol->name) == 0)
			{
				nstate.depth = state.depth + 1;
				nstate.node = ((t_ms_edge *)data->chart->adjacency_list[state.node]->contents + i)->finish;
				nstate.rule = state.rule;
				int	res = ms_search_core(tree, data, nstate);
				if (res > 0)
				{
					subtree.rule_name = (char *)symbol->name;
					subtree.start = state.node;
					subtree.end = nstate.node;
					ft_darray_set(tree->children, &subtree, state.depth);
				}
				if (nstate.node == tree->end)
					return (1);
			}
		}
		return (0);
	}
	// todo: this should be an error.
	printf("Parsing Failed!");
	return (-1);
}

int	ms_search(t_parse_tree *parse_tree, t_parsing_data *data, int rule)
{
	t_parser_state	state;

	state.depth = 0;
	state.node = parse_tree->start;
	state.rule = data->grammar->rules[rule];
	parse_tree->rule = rule;
	ms_search_core(parse_tree, data, state);
	return (1);
}

int	ms_build_parse_tree(t_parse_tree *parse_tree, t_parsing_data *data)
{
	int				end;
	int				i;
	t_ms_edge		*edge;
	t_parse_tree	*child;

	if (parse_tree->terminal || !parse_tree->end || parse_tree->start == parse_tree->end)
		return (0);
	end = -1;
	parse_tree->children = malloc(sizeof(t_darray));
	if (ft_darray_init(parse_tree->children, sizeof(t_parse_tree), data->tokens->size + 1) < 0)
		return (-1);
	while (++end < data->chart->adjacency_list[parse_tree->start]->size)
	{
		edge = ((t_ms_edge *)data->chart->adjacency_list[parse_tree->start]->contents + end);
		if (edge->finish == parse_tree->end && strcmp(parse_tree->rule_name, data->grammar->rules[edge->rule]->name) == 0)
			ms_search(parse_tree, data, edge->rule);
	}
	i = -1;
	while (++i < parse_tree->children->size)
	{
		child = (t_parse_tree *)parse_tree->children->contents + i;
		ms_build_parse_tree(child, data);
	}
	return (1);
}
