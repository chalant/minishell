/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_darray_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 10:32:54 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/23 11:09:09 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_darray_reverse(t_darray *darray)
{
	int		i;
	int		j;
	void	*tmp;

	i = 0;
	j = darray->size - 1;
	tmp = malloc(darray->type_size);
	if (!tmp)
		return (-1);
	while (i < j)
	{
		ft_memcpy(tmp, ft_darray_get(darray, i), darray->type_size);
		ft_darray_set(darray, ft_darray_get(darray, j), i);
		ft_darray_set(darray, tmp, j);
		i++;
		j--;
	}
	free(tmp);
	return (1);
}

int	ft_darray_join(t_darray *darray, t_darray *with)
{
	int				i;

	i = -1;
	while (++i < with->size)
	{
		if (ft_darray_append(darray, ft_darray_get(with, i)) < 0)
			return (-1);
	}
	return (1);
}

int	ft_darray_slice(t_darray *darray, t_darray *result, int start, int end)
{
	int		i;

	i = -1;
	while (++i < end - start)
	{
		if (ft_darray_append(result, ft_darray_get(darray, i + start)) < 0)
			return (-1);
	}
	return (1);
}

int	ft_darray_insert(t_darray *darray, t_darray *with, t_darray *tmp, int at)
{
	if (ft_darray_slice(darray, tmp, at, darray->size) < 0)
		return (-1);
	if (at >= darray->size)
	{
		if (darray->size > 0)
			at = darray->size - 1;
		else
			at = 0;
	}
	if (ft_darray_join(darray, with) < 0)
		return (-1);
	if (ft_darray_join(darray, tmp) < 0)
		return (-1);
	return (1);
}

int	ft_darray_onsert(t_darray *darray, t_darray *with, t_darray *tmp, int at)
{
	if (ft_darray_slice(darray, tmp, at + 1, darray->size) < 0)
		return (-1);
	if (at >= darray->size)
	{
		if (darray->size > 0)
			at = darray->size - 1;
		else
			at = 0;
	}
	darray->size = at;
	if (ft_darray_join(darray, with) < 0)
		return (-1);
	if (ft_darray_join(darray, tmp) < 0)
		return (-1);
	return (1);
}
