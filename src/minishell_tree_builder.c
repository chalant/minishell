/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tree_builder.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:26 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 17:17:08 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_terminal(t_parse_tree *tree, t_parsing_data *data,
	t_parser_state state, t_ms_symbol *symbol)
{
	t_parse_tree	*subtree;
	t_token			*token;

	token = (t_token *)ft_darray_get(data->tokens, state.node);
	if (state.node >= tree->end)
		return (0);
	else if (!symbol->match(symbol, token))
		return (0);
	subtree = get_subtree(tree, state.depth);
	subtree->token = token;
	set_subtree(subtree, token->string, state.node, state.node);
	if (ft_darray_set(tree->children, subtree, state.depth) < 0)
		return (-1);
	if (state.node >= data->input_length)
		return (0);
	if (ms_search_core(tree, data,
			next_state(state.depth + 1, state.node + 1, state.rule)) < 0)
		return (-1);
	return (1);
}

static int	add_subtree(t_parse_tree *tree, t_ms_symbol *symbol,
	t_parser_state state, t_parser_state nstate)
{
	t_parse_tree	*subtree;

	subtree = get_subtree(tree, state.depth);
	set_subtree(subtree, symbol->name, state.node, nstate.node);
	subtree->terminal = 0;
	return (0);
}

int	process_non_terminal(t_parse_tree *tree, t_parsing_data *data,
	t_parser_state state, t_ms_symbol *symbol)
{
	int				i;
	int				res;
	t_parser_state	nstate;
	t_ms_edge		*item;
	t_darray		*edges;

	edges = get_edges(data->chart, state.node);
	item = (t_ms_edge *)edges->contents;
	i = -1;
	while (++i < edges->size)
	{
		if (ft_strcmp(data->grammar->rules[(item + i)->rule]->name,
				symbol->name) != 0)
			continue ;
		nstate = next_state(state.depth + 1,
				(item + i)->finish, state.rule);
		res = ms_search_core(tree, data, nstate);
		if (res > 0)
			add_subtree(tree, symbol, state, nstate);
		else if (res < 0)
			return (-1);
		if (nstate.node == tree->end)
			return (1);
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

int	build_parse_tree(t_parse_tree *parse_tree, t_parsing_data *data)
{
	int				i;
	t_parse_tree	*child;

	if (parse_tree->terminal
		|| parse_tree->start == parse_tree->end)
		return (0);
	i = -1;
	if (!parse_tree->children && init_children(parse_tree) < 0)
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
