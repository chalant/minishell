/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_recogniser.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:34 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/05 15:09:36 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ms_symbol	*next_symbol(t_ms_grammar *grammar, t_earley_item *item)
{
	return (grammar->rules[item->rule]->symbols[item->next]);
}

//checks if the item is already in the array before appending
int	earley_safe_append(t_earley_set *set, t_earley_item *item)
{
	int				i;
	t_earley_item	*current_item;
	t_earley_item	*items;

	items = (t_earley_item *)set->items->contents;
	i = -1;
	while (++i < set->items->size)
	{
		current_item = items + i;
		if (!current_item)
			return (-1);
		if (current_item->next == item->next
			&& current_item->rule == item->rule
			&& current_item->start == item->start)
			return (0);
	}
	if (ft_darray_append(set->items, item) < 0)
		return (-1);
	return (1);
}

int	contains_token(t_ms_rule *rule, t_token *token)
{
	int	i;

	i = -1;
	while (++i < rule->length)
	{
		if (strcmp(rule->symbols[i]->name, token->string) == 0)
		{
			return (1);
		}
	}
	return (0);
}

int	earley_complete(t_parsing_data *data, int j, int state_id)
{
	int				i;
	t_earley_item	new_item;
	t_earley_item	*old_item;
	t_earley_item	*item;
	t_earley_set	*set;

	i = -1;
	set = ft_darray_get(data->earley_sets, state_id);
	item = ft_darray_get(set->items, j);
	while (++i < ((t_earley_set *)(ft_darray_get(data->earley_sets,
				item->start)))->items->size)
	{
		item->completed = 1;
		old_item = ft_darray_get(((t_earley_set *)(ft_darray_get(data->earley_sets, item->start)))->items, i);
		if (strcmp(next_symbol(data->grammar, old_item)->name, data->grammar->rules[item->rule]->name) == 0)
		{
			new_item.next = old_item->next + 1;
			new_item.rule = old_item->rule;
			new_item.start = old_item->start;
			new_item.completed = 0;
			if (earley_safe_append(ft_darray_get(data->earley_sets, state_id), &new_item) < 0)
				return (-1);
		}
		item = ft_darray_get(set->items, j);
	}
	return (0);
}

int	earley_predict(t_parsing_data *data, t_ms_symbol *symbol, int state_id)
{
	int				i;
	t_earley_item	item;
	t_earley_set	*set;

	set = ft_darray_get(data->earley_sets, state_id);
	i = -1;
	while (++i < data->grammar->length)
	{
		if (strcmp(symbol->name, data->grammar->rules[i]->name) == 0)
		{
			item.rule = i;
			item.next = 0;
			item.start = state_id;
			item.completed = 0;
			if (earley_safe_append(set, &item) < 0)
				return (-1);
		}
	}
	return (0);
}

int	earley_scan(t_parsing_data *data, t_ms_symbol *symbol, int state_id, int item_idx)
{
	t_earley_item	new_item;
	t_earley_item	*item;
	t_earley_item	*items;

	items = ((t_earley_set *)(ft_darray_get(data->earley_sets, state_id)))->items->contents;
	item = items + item_idx;
	if (symbol->match(symbol, ft_darray_get(data->tokens, state_id)))
	{
		new_item.next = item->next + 1;
		new_item.start = item->start;
		new_item.rule = item->rule;
		new_item.completed = 0;
		if (ft_darray_append(((t_earley_set *)(ft_darray_get(data->earley_sets, state_id + 1)))->items, &new_item) < 0)
			return (-1);
	}
	return (0);
}

int	set_parsing_info(t_darray *sets, t_ms_grammar *grammar, t_darray *tokens, t_graph *chart)
{
	int			i;
	int			j;
	t_ms_symbol *symbol;

	i = -1;
	while (++i < grammar->length)
	{
		j = -1;
		while (++j < grammar->rules[i]->length)
		{
			symbol = grammar->rules[i]->symbols[j];
			symbol->earley_sets = sets;
			symbol->tokens = tokens;
			symbol->chart = chart;
		}
	}
	return (1);
}

int	next_earley_items(t_parsing_data *data, t_earley_set *set, int state_id, int item_idx)
{
	t_ms_symbol		*symbol;

	symbol = next_symbol(data->grammar, ft_darray_get(set->items, item_idx));
	if (symbol->symbol_type == MS_NON_TERMINAL_SYMBOL)
		return (earley_predict(data, symbol, state_id));
	else if (symbol->symbol_type == MS_TERMINAL_SYMBOL)
		return (earley_scan(data, symbol, state_id, item_idx));
	else if (symbol->symbol_type == MS_NULL_SYMBOL)
		return (earley_complete(data, item_idx, state_id));
	return (0);
}

int	init_earley_items(t_parsing_data *data)
{
	int				i;
	t_earley_item	earley_item;
	t_earley_set	*set;

	i = -1;
	set = ft_darray_get(data->earley_sets, 0);
	set_parsing_info(data->earley_sets, data->grammar, data->tokens, data->chart);
	while (++i < data->grammar->length)
	{
		if (strcmp(data->grammar->start_rule, data->grammar->rules[i]->name) == 0)
		{
			earley_item.rule = i;
			earley_item.start = 0;
			earley_item.next = 0;
			earley_item.completed = 0;
			if (ft_darray_append(set->items, &earley_item) < 0)
				return (-1);
		}
	}
	return (1);
}

int	build_earley_items(t_parsing_data *data)
{
	int				i;
	int				j;
	t_earley_set	*set;

	if (init_earley_items(data) < 0)
		return (-1);
	i = -1;
	while (++i < data->tokens->size)
	{
		j = -1;
		set = ft_darray_get(data->earley_sets, i);
		while (++j < set->items->size)
		{
			if (next_earley_items(data, set, i, j) < 0)
				return (-1);
			set = ft_darray_get(data->earley_sets, i);
		}
	}
	return (0);
}
