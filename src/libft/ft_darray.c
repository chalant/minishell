/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_darray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 16:35:27 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/05 14:28:11 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "unistd.h"

int	ft_darray_init(t_darray *darray, int type_size, int size)
{
	darray->contents = malloc(type_size * size);
	if (!darray->contents)
		return (-1);
	ft_bzero(darray->contents, type_size * size);
	darray->size = 0;
	darray->max_size = size;
	darray->type_size = type_size;
	darray->block_size = size;
	darray->actual_size = 0;
	return (0);
}

// if del_content == NULL that step is skipped.
int	ft_darray_reset(t_darray *darray, void del_content(void *))
{
	int	i;

	if (!darray || !darray->contents)
		return (0);
	if (del_content)
	{
		i = 0;
		while (i < darray->actual_size)
		{
			del_content(darray->contents + i * darray->type_size);
			i++;
		}
	}
	darray->size = 0;
	return (0);
}

// if del_content == NULL that step is skipped.
int	ft_darray_delete(t_darray *darray, void del_content(void *))
{
	int	i;

	if (!darray || !darray->contents)
		return (0);
	if (del_content)
	{
		i = 0;
		while (i < darray->actual_size)
		{
			del_content(darray->contents + i * darray->type_size);
			i++;
		}
	}
	free(darray->contents);
	darray->contents = NULL;
	darray->size = 0;
	darray->max_size = 0;
	darray->actual_size = 0;
	return (0);
}

int	ft_darray_append(t_darray *darray, void *element)
{
	void	*new_elements;
	void	*address;

	if (darray->size + 1 > darray->max_size)
	{
		new_elements = malloc((darray->max_size + darray->block_size) * darray->type_size);
		if (!new_elements)
			return (-1);
		ft_bzero(new_elements, (darray->max_size + darray->block_size) * darray->type_size);
		ft_memcpy(new_elements, darray->contents, darray->max_size * darray->type_size);
		free(darray->contents);
		darray->contents = new_elements;
		darray->max_size += darray->block_size;
	}
	darray->size += 1;
	if (darray->size > darray->actual_size)
		darray->actual_size += 1;
	address = (unsigned char*)darray->contents + (darray->size - 1) * darray->type_size;
	ft_memcpy(address, element, darray->type_size);
	return (0);
}
