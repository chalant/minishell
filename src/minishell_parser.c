/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:26 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/24 16:43:30 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_tree(t_parse_tree *tree)
{
	tree->rule_name = NULL;
	tree->children = NULL;
	tree->rule_name = NULL;
	tree->children = NULL;
	tree->terminal = 0;
	tree->start = 0;
	tree->end = 0;
	return (1);
}

t_parser_state	next_state(int depth, int node, t_ms_rule *rule)
{
	t_parser_state	state;

	state.depth = depth;
	state.node = node;
	state.rule = rule;
	return (state);
}

void	set_subtree(t_parse_tree *subtree, char *rule_name, int start, int end)
{
	init_tree(subtree);
	subtree->rule_name = rule_name;
	subtree->start = start;
	subtree->end = end;
}

int	process_terminal(t_parse_tree *tree, t_parsing_data *data,
	t_parser_state state, t_ms_symbol *symbol)
{
	t_parse_tree	subtree;
	t_token			*token;

	token = (t_token *)ft_darray_get(data->tokens, state.node);
	if (state.node >= tree->end)
		return (0);
	else if (symbol->match(symbol,
			(t_token *)ft_darray_get(data->tokens, state.node)))
	{
		set_subtree(&subtree, token->string, state.node, state.node);
		subtree.terminal = 1;
		ft_darray_set(tree->children, &subtree, state.depth);
		if (state.node < data->input_length)
			ms_search_core(tree, data,
				next_state(state.depth + 1, state.node + 1, state.rule));
		return (1);
	}
	return (0);
}

int	process_non_terminal(t_parse_tree *tree, t_parsing_data *data,
	t_parser_state state, t_ms_symbol *symbol)
{
	int				i;
	t_parser_state	nstate;
	t_parse_tree	subtree;
	t_ms_edge		*item;
	t_darray		*edges;

	edges = get_edges(data->chart, state.node);
	item = (t_ms_edge *)edges->contents;
	i = -1;
	while (++i < edges->size)
	{
		if (strcmp(data->grammar->rules[(item + i)->rule]->name, symbol->name) == 0)
		{
			nstate = next_state(state.depth + 1, (item + i)->finish, state.rule);
			if (ms_search_core(tree, data, nstate) > 0)
			{
				set_subtree(&subtree, symbol->name, state.node, nstate.node);
				ft_darray_set(tree->children, &subtree, state.depth);
			}
			if (nstate.node == tree->end)
				return (1);
		}
	}
	return (0);
}

int	ms_search_core(t_parse_tree *tree, t_parsing_data *data,
	t_parser_state state)
{
	t_ms_symbol		*symbol;

	if (state.depth >= state.rule->length - 1)
		return (1);
	symbol = state.rule->symbols[state.depth];
	if (symbol->symbol_type == MS_TERMINAL_SYMBOL)
		return (process_terminal(tree, data, state, symbol));
	else if (symbol->symbol_type == MS_NON_TERMINAL_SYMBOL)
		return (process_non_terminal(tree, data, state, symbol));
	return (-1);
}

int	ms_search(t_parse_tree *parse_tree, t_parsing_data *data, int rule)
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
		if (edge->finish == parse_tree->end && strcmp(parse_tree->rule_name,
				data->grammar->rules[edge->rule]->name) == 0)
			if (ms_search(parse_tree, data, edge->rule) < 0)
				return (-1);
	}
	return (1);
}

int	build_parse_tree(t_parse_tree *parse_tree, t_parsing_data *data)
{
	int				i;
	t_parse_tree	*child;

	if (parse_tree->terminal || !parse_tree->end
		|| parse_tree->start == parse_tree->end)
		return (0);
	i = -1;
	parse_tree->children = malloc(sizeof(t_darray));
	if (!parse_tree->children)
		return (-1);
	if (ft_darray_init(parse_tree->children,
			sizeof(t_parse_tree), data->tokens->size) < 0)
		return (-1);
	if (fill_parse_tree(parse_tree, data) < 0)
		return (-1);
	i = -1;
	while (++i < parse_tree->children->size)
	{
		child = (t_parse_tree *)parse_tree->children->contents + i;
		build_parse_tree(child, data);
	}
	return (1);
}
