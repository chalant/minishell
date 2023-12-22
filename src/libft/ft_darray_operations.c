/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_darray_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 10:32:54 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/22 12:58:23 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_darray_reverse(t_darray *darray)
{
	int		i;
	int		j;
	void	*tmp;

	i = -1;
	j = darray->size - 1;
	while (++i < j)
	{
		tmp = ft_darray_get(darray, j);
		ft_darray_set(darray, ft_darray_get(darray, i), j);
		ft_darray_set(darray, tmp, i);
		j--;
	}
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
	int				n;

	if (ft_darray_slice(darray, tmp, at, darray->actual_size) < 0)
		return (-1);
	darray->size = at + 1;
	if (ft_darray_join(darray, with) < 0)
		return (-1);
	if (ft_darray_join(darray, tmp) < 0)
		return (-1);
}
