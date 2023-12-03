/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_earley_set.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 16:59:20 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/29 14:09:55 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_earley_set(void *set)
{
	t_earley_set	*tmp;

	tmp = (t_earley_set *)set;
	ft_darray_delete(tmp->items, NULL);
	free(tmp->items);
	tmp->items = NULL;
	tmp->size = 0;
	return ;
}

void	reset_earley_set(void *set)
{
	t_earley_set	*tmp;

	tmp = (t_earley_set *)set;
	ft_darray_reset(tmp->items, NULL);
	tmp->size = 0;
	return ;
}

int	clear_earley_sets(t_darray *sets, int (*del_method)(t_darray *, void(*)(void *)))
{
	int				i;
	t_earley_set	*set;

	i = -1;
	while (++i < sets->size)
	{
		set = ft_darray_get(sets, i);
		del_method(set->items, NULL);
		//free(set->items);
	}
	del_method(sets, NULL);
	return (0);
}

int add_earley_set(t_darray *sets, int size)
{
	t_earley_set	new;

	if (sets->size < sets->actual_size)
	{
		sets->size += 1;
		return (1);
	}
	new.items = malloc(sizeof(t_darray));
	new.size = 0;
	if (ft_darray_init(new.items, sizeof(t_earley_item), size) < 0)
		return (-1);
	return (ft_darray_append(sets, &new));
}
