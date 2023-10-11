/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 13:35:42 by ychalant          #+#    #+#             */
/*   Updated: 2023/04/11 13:35:42 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static inline int	intlen(int n)
{
	int	i;

	i = (n == 0);
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		len;
	int		neg;
	long	v;
	char	*str;

	neg = (n < 0);
	v = (long)n;
	v = -(neg) * v + !(neg) * v;
	len = intlen(n) + neg;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[0] = '-';
	str[len] = '\0';
	len--;
	while (len > -(!neg))
	{
		str[len] = (v % 10) + '0';
		v /= 10;
		len--;
	}
	return (str);
}
