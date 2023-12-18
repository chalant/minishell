/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_recogniser_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:24:31 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 15:36:43 by ychalant         ###   ########.fr       */
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

	i = -1;
	while (++i < set->items->size)
	{
		current_item = ft_darray_get(set->items, i);
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

int	set_parsing_info(t_parsing_data *data, void *context)
{
	int			i;
	int			j;
	t_ms_symbol	*symbol;

	i = -1;
	while (++i < data->grammar->length)
	{
		j = -1;
		while (++j < data->grammar->rules[i]->length)
		{
			symbol = data->grammar->rules[i]->symbols[j];
			symbol->context = context;
		}
	}
	return (1);
}

int	next_earley_items(t_parsing_data *data, t_earley_set *set,
	int state_id, int item_idx)
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

int	init_earley_items(t_parsing_data *data, void *context)
{
	int				i;
	t_earley_item	earley_item;
	t_earley_set	*set;

	i = -1;
	set = ft_darray_get(data->earley_sets, 0);
	set_parsing_info(data, context);
	while (++i < data->grammar->length)
	{
		if (strcmp(data->grammar->start_rule,
				data->grammar->rules[i]->name) == 0)
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
