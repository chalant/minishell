/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_recogniser.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:34 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 16:06:33 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_earley_item(t_parsing_data *data, t_earley_item *current_item,
	t_earley_item *old_item, int state_id)
{
	t_earley_item	new_item;

	if (strcmp(next_symbol(data->grammar, old_item)->name,
			data->grammar->rules[current_item->rule]->name) == 0)
	{
		new_item.start = old_item->start;
		new_item.rule = old_item->rule;
		new_item.next = old_item->next + 1;
		new_item.completed = 0;
		if (earley_safe_append(ft_darray_get(data->earley_sets, state_id),
				&new_item) < 0)
			return (-1);
	}
	return (0);
}

int	earley_complete(t_parsing_data *data, int j, int state_id)
{
	int				i;
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
		old_item = ft_darray_get(((t_earley_set *)
					(ft_darray_get(data->earley_sets, item->start)))->items, i);
		add_earley_item(data, item, old_item, state_id);
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

int	earley_scan(t_parsing_data *data, t_ms_symbol *symbol,
	int state_id, int item_idx)
{
	t_earley_item	new_item;
	t_earley_item	*item;
	t_earley_item	*items;
	int				match;

	items = ((t_earley_set *)(ft_darray_get(data->earley_sets,
					state_id)))->items->contents;
	item = items + item_idx;
	match = symbol->match(symbol, ft_darray_get(data->tokens, state_id));
	if (match > 0)
	{
		new_item.next = item->next + 1;
		new_item.start = item->start;
		new_item.rule = item->rule;
		new_item.completed = 0;
		if (ft_darray_append(((t_earley_set *)(ft_darray_get(data->earley_sets,
						state_id + 1)))->items, &new_item) < 0)
			return (-1);
	}
	else if (match < 0)
		return (-1);
	return (0);
}

int	build_earley_items(t_parsing_data *data, void *context)
{
	int				i;
	int				j;
	t_earley_set	*set;

	if (init_earley_items(data, context) < 0)
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
