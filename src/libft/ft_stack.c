/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:09:47 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 12:09:54 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_stack_init(t_stack *stack, t_darray *elements)
{
	stack->size = elements->size;
	stack->elements = elements;
	stack->mode = FT_LIFO;
	return (1);
}

void	*ft_stack_pop(t_stack *stack)
{
	stack->size -= 1;
	if (stack->mode == FT_FIFO)
		return (ft_darray_get(stack->elements,
				stack->elements->size - 1 - stack->size));
	return (ft_darray_get(stack->elements, stack->size));
}

void	*ft_stack_peek(t_stack *stack)
{
	if (stack->size - 1 < 0)
		return (NULL);
	if (stack->size - 1 >= stack->elements->size)
		return (NULL);
	return (ft_darray_get(stack->elements, stack->size - 1));
}

int	ft_stack_push(t_stack *stack, void *element)
{
	stack->size += 1;
	ft_darray_append(stack->elements, element);
	return (1);
}
