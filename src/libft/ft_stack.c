#include "libft.h"

int	ft_stack_init(t_stack *stack, t_darray *elements)
{
	stack->size = elements->size;
	stack->elements = elements;
	return (1);
}

void	*ft_stack_pop(t_stack *stack)
{
	stack->size -= 1;
	return (ft_darray_get(stack->elements, stack->size));
}

void	*ft_stack_peek(t_stack *stack)
{
	return (ft_darray_get(stack->elements, stack->size - 1));
}
