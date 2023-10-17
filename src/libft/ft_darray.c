/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_darray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 16:35:27 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/17 13:45:10 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "unistd.h"

int	ft_darray_init(t_darray *darray, int type_size, int size)
{
	darray->contents = malloc(type_size * size);
	if (!darray->contents)
		return (-1);
	darray->size = 0;
	darray->max_size = size;
	darray->type_size = type_size;
	darray->block_size = size;
	return (0);
}

int	ft_darray_reset(t_darray *darray)
{
	if (!darray)
		return (0);
	darray->size = 0;
	return (0);
}

// if del_content == NULL that step is skipped.
int	ft_darray_delete(t_darray *darray, void del_content(void *))
{
	int	i;

	if (!darray)
		return (0);
	if (del_content)
	{
		i = 0;
		while (i < darray->size)
		{
			del_content(darray->contents + i * darray->type_size);
			i++;
		}
	}
	free(darray->contents);
	return (0);
}

int	ft_darray_append(t_darray *darray, void *element)
{
	void	*new_elements;
	void	*address;

	darray->size += 1;
	if (darray->size > darray->max_size)
	{
		new_elements = malloc((darray->max_size + darray->block_size) * darray->type_size);
		if (!new_elements)
			return (-1);
		ft_memcpy(new_elements, darray->contents, darray->max_size * darray->type_size);
		free(darray->contents);
		darray->contents = new_elements;
		darray->max_size += darray->block_size;
	}
	address = (unsigned char*)darray->contents + (darray->size - 1) * darray->type_size;
	ft_memcpy(address, element, darray->type_size);
	return (0);
}
