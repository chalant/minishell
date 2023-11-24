/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_earley_set.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 16:59:20 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/24 17:46:13 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int add_earley_set(t_darray *sets, int size)
{
    t_earley_set    set;
	t_earley_set	*mset;

    set.items = malloc(sizeof(t_darray));
    if (ft_darray_init(set.items, sizeof(t_earley_item), size) < 0)
        return (-1);
    return (ft_darray_append(sets, &set));
}

// int	reverse_earley(t_darray *sets, t_earley_set **reversed, int size)
// {
// 	int				i;
// 	int				j;
// 	int				k;
// 	t_earley_item	*item;
//     t_earley_set    *set;

// 	i = -1;
//     item = malloc(sizeof(t_earley_item));
// 	while (++i < size)
// 	{
// 		j = -1;
//         set = ft_darray_get(sets, i);
// 		while (++j < set->items->size)
// 		{
//             ft_memcpy(item, ft_darray_get(set, j), sizeof(t_earley_item));
// 			if (item->completed)
// 			{
// 				k = item->start;
// 				item->start = i;
// 				ft_darray_append(reversed[k]->items, item);
// 			}
// 		}
// 		reversed[i]->size = j;
// 	}
// 	return (0);
// }
