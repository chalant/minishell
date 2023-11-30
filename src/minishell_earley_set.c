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

// int	init_earley_sets(t_parsing_data *data)
// {
// 	data->earley_sets = malloc(sizeof(t_darray));
// 	if (!data->earley_sets)
// 		return (-1);
// 	return (0);
// }

int	clear_earley_sets(t_darray *sets, int (*del_method)(t_darray *, void(*)(void *)))
{
	int				i;
	t_earley_set	*set;

	i = -1;
	while (++i < sets->size)
	{
		set = ft_darray_get(sets, i);
		del_method(set->items, NULL);
		free(set->items);
	}
	del_method(sets, NULL);
	return (0);
}

int add_earley_set(t_darray *sets, int size)
{
    t_earley_set    set;

	set.items = malloc(sizeof(t_darray));
	set.size = 0;
	if (ft_darray_init(set.items, sizeof(t_earley_item), size) < 0)
		return (-1);
    return (ft_darray_append(sets, &set));
}
