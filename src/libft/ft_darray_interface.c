#include "libft.h"
#include <stdio.h>

void	*ft_darray_get(t_darray *darray, int index)
{
	return (darray->contents + index * darray->type_size);
}

int	ft_darray_set(t_darray *darray, void *element, int index)
{
	unsigned char *target;

	target = (unsigned char *)darray->contents + index * darray->type_size;
	if (!*target)
		darray->size += 1;
	ft_memcpy(target, element, darray->type_size);
	return (0);
}
