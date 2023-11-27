/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_earley_set.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 16:59:20 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/27 11:33:37 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int add_earley_set(t_darray *sets, int size)
{
    t_earley_set    set;

    set.items = malloc(sizeof(t_darray));
    if (ft_darray_init(set.items, sizeof(t_earley_item), size) < 0)
        return (-1);
    return (ft_darray_append(sets, &set));
}
