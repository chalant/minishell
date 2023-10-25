/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:26 by ychalant          #+#    #+#             */
/*   Updated: 2023/10/23 13:50:07 by ychalant         ###   ########.fr       */
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
	if (symbol->symbol_type == MS_TERMINAL_SYMBOL)
	{
		subtree.terminal = 1;
		if (state.node >= tree->end)
			return (0);
		else if (symbol->match(symbol, data->input[state.node]))
		{
			//printf("Success! 1 %s %s %d\n", data->input[state.node], symbol->name, state.node);
			subtree.rule_name = (char *)data->input[state.node];
			subtree.terminal = 1;
			subtree.start = state.node - 1;
			subtree.end = state.node;
			ft_darray_set(tree->children, &subtree, state.depth);
			//ft_darray_append(tree->children, &subtree);
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
					//ft_darray_append(tree->children, &subtree);
					//printf("Success! 1 %s %d %d\n", symbol->name, state.node, nstate.node);
					//return (1);
				}
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
	// int	i;

	state.depth = 0;
	state.node = parse_tree->start;
	state.rule = data->grammar->rules[rule];
	parse_tree->rule = rule;
	// i = -1;
	// printf("Decompose ");
	// while (++i < state.rule->length)
	// 	printf("%s ", state.rule->symbols[i]->name);
	// printf("%d %d\n", parse_tree->start, parse_tree->end);
	//printf("Hello hello! %d %d\n", state.node, state.end);
	//todo: go to sub_children
	ms_search_core(parse_tree, data, state);
	return (1);
}

int	ms_build_parse_tree(t_parse_tree *parse_tree, t_parsing_data *data)
{
	int				end;
	int				i;
	t_ms_edge		*edge;
	t_parse_tree	*child;

	if (parse_tree->terminal || parse_tree->end == 0)
		return (0);
	end = 0;
	edge = ((t_ms_edge *)data->chart->adjacency_list[parse_tree->start]->contents);
	while (++end < data->chart->adjacency_list[parse_tree->start]->size)
	{
		if (edge->finish == parse_tree->end && strcmp(parse_tree->rule_name, data->grammar->rules[edge->rule]->name) == 0)
			break;
		edge = ((t_ms_edge *)data->chart->adjacency_list[parse_tree->start]->contents + end);
	}
	parse_tree->children = malloc(sizeof(t_darray));
	ft_darray_init(parse_tree->children, sizeof(t_parse_tree), 10);
	ms_search(parse_tree, data, edge->rule);
	i = -1;
	//printf("YOYOYOYOYOYOYOYOYOYOYOYOYOY! %d %s %d %d\n", parse_tree->terminal, parse_tree->rule_name, parse_tree->start, parse_tree->end);
	while (++i < parse_tree->children->size)
	{
		child = (t_parse_tree *)parse_tree->children->contents + i;
		ms_build_parse_tree(child, data);
	}
	return (1);
}
