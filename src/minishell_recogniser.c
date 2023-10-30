/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_recogniser.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:34 by ychalant          #+#    #+#             */
/*   Updated: 2023/10/23 13:50:03 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ms_symbol	*ms_next_symbol(t_ms_grammar *grammar, t_earley_item *item)
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
		if (current_item->next == item->next
			&& current_item->rule == item->rule
			&& current_item->start == item->start)
				return (0);
	}
	if (ft_darray_append(set->items, item) < 0)
		return (-1);
	return (1);
}

int	earley_complete(t_earley_set **sets, t_ms_grammar *grammar, t_earley_item *item, int state_id)
{
	t_earley_set	*set;
	t_earley_item	new_item;
	t_earley_item	*old_item;
	int				i;

	i = -1;
	while (++i < sets[item->start]->items->size)
	{
		set = sets[item->start];
		item->completed = 1;
		old_item = (t_earley_item *)set->items->contents + i;
		if (strcmp(ms_next_symbol(grammar, old_item)->name, grammar->rules[item->rule]->name) == 0)
		{
			new_item.next = old_item->next + 1;
			new_item.rule = old_item->rule;
			new_item.start = old_item->start;
			new_item.completed = 0;
			if (earley_safe_append(sets[state_id], &new_item) < 0)
				return (-1);
		}
	}
	return (0);
}

//adds an item to the set
int	earley_predict(t_earley_set **sets, t_ms_grammar *grammar, t_ms_symbol *symbol, int state_id)
{
	int				i;
	t_earley_item	item;

	i = -1;
	while (++i < grammar->length)
	{
		if (strcmp(symbol->name, grammar->rules[i]->name) == 0)
		{
			item.rule = i;
			item.next = 0;
			item.start = state_id;
			item.completed = 0;
			if (earley_safe_append(sets[state_id], &item) < 0)
				return (-1);
		}
	}
	return (0);
}

//add items to the next state if they match
int	earley_scan(t_earley_set **sets, t_ms_symbol *symbol, int state_id, int item_idx, t_darray *tokens)
{
	t_earley_item	new_item;
	t_earley_item	*item;
	t_earley_item	*items;

	items = (t_earley_item *)sets[state_id]->items->contents;
	item = items + item_idx;
	if (symbol->match(symbol, (t_token *)ft_darray_get(tokens, state_id)))
	{
		new_item.next = item->next + 1;
		new_item.start = item->start;
		new_item.rule = item->rule;
		new_item.completed = 0;
		if (ft_darray_append(sets[state_id + 1]->items, &new_item) < 0)
			return (-1);
	}
	return (0);
}

int	build_earley_items(t_earley_set **sets, t_ms_grammar *grammar, int n_sets, t_darray *tokens)
{
	(void)n_sets;
	int				i;
	int				j;
	t_earley_item	earley_item;
	//t_earley_item	*items;
	t_ms_symbol		*symbol;

	i = -1;
	while (++i < grammar->length)
	{
		if (strcmp(grammar->start_rule, grammar->rules[i]->name) == 0)
		{
			earley_item.rule = i;
			earley_item.start = 0;
			earley_item.next = 0;
			earley_item.completed = 0;
			if (ft_darray_append(sets[0]->items, &earley_item) < 0)
				return (-1);
		}
	}
	i = -1;
	while (++i < tokens->size)
	{
		j = -1;
		while (++j < sets[i]->items->size)
		{
			//items = (t_earley_item *)sets[i]->items->contents;
			symbol = ms_next_symbol(grammar, (t_earley_item *)ft_darray_get(sets[i]->items, j));
			if (symbol->symbol_type == MS_NON_TERMINAL_SYMBOL)
				earley_predict(sets, grammar, symbol, i);
			else if (symbol->symbol_type == MS_TERMINAL_SYMBOL)
				earley_scan(sets, symbol, i, j, tokens);
			else if (symbol->symbol_type == MS_NULL_SYMBOL)
				earley_complete(sets, grammar, (t_earley_item *)ft_darray_get(sets[i]->items, j), i);
			else
				printf("Illegal rule");
		}
	}
	return (0);
}
