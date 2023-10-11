/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 13:35:42 by ychalant          #+#    #+#             */
/*   Updated: 2023/04/11 13:35:42 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static inline int	is_space(char c)
{
	return ((c > 8 && c < 14) || c == 32);
}

static inline int	ft_isdigit(char c)
{
	return (c > 47 && c < 58);
}

int	ft_atoi(const char *nptr)
{
	size_t				i;
	unsigned long long	number;
	int					sign;
	int					is_sign;

	i = 0;
	number = 0;
	while (nptr[i] && is_space(nptr[i]))
		i++;
	is_sign = (nptr[i] == '-' || nptr[i] == '+');
	sign = is_sign * ((nptr[i] == '+') - (nptr[i] == '-')) + !is_sign;
	i += is_sign;
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		number = 10 * number + (nptr[i] - '0');
		i++;
	}
	if (number > 9223372036854775807 && sign == 1)
		return (-1);
	if (number > 9223372036854775807 && sign == -1)
		return (0);
	return (sign * number);
}
