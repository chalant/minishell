/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_darray_interface.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:09:15 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/23 11:04:24 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	*ft_darray_get(t_darray *darray, int index)
{
	if (index >= darray->actual_size || index < 0)
		return (NULL);
	return (darray->contents + index * darray->type_size);
}

int	ft_darray_exists(t_darray *darray, int index)
{
	unsigned char	*target;

	if (index >= darray->actual_size)
		return (0);
	else if (index < 0)
		return (0);
	target = (unsigned char *)darray->contents + index * darray->type_size;
	if (!*target)
		return (0);
	return (1);
}

int	ft_darray_set(t_darray *darray, void *element, int index)
{
	unsigned char	*target;

	if (index >= darray->max_size)
		return (ft_darray_append(darray, element));
	target = (unsigned char *)(darray->contents + index * darray->type_size);
	ft_memcpy(target, element, darray->type_size);
	return (0);
}
