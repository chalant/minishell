/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 13:35:42 by ychalant          #+#    #+#             */
/*   Updated: 2023/04/11 13:35:42 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

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

static inline void	ft_itoa(char *buffer, int n, int len, int neg)
{
	long	v;

	v = (long)n;
	v = -(neg) * v + !(neg) * v;
	buffer[0] = '-';
	buffer[len] = '\0';
	len--;
	while (len > -(!neg))
	{
		buffer[len] = (v % 10) + '0';
		v /= 10;
		len--;
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	int		len;
	int		neg;
	char	buffer[12];

	neg = (n < 0);
	len = intlen(n) + neg;
	ft_itoa(buffer, n, len, neg);
	write(fd, buffer, len);
}
