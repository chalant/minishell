/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_earley_set.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 16:59:20 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/28 17:11:40 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    clear_earley_set(t_earley_set *set)
{
    ft_darray_delete(set->items, NULL);
}

int add_earley_set(t_darray *sets, int size)
{
    t_earley_set    set;

    set.items = malloc(sizeof(t_darray));
    if (ft_darray_init(set.items, sizeof(t_earley_item), size) < 0)
        return (-1);
    return (ft_darray_append(sets, &set));
}
