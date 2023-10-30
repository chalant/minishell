/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_core_atoi.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 11:56:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/10/09 12:13:55 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

static inline int	is_space(char c)
{
	return ((c > 8 && c < 14) || c == 32);
}

static inline int	ft_isdigit(char c)
{
	return (c > 47 && c < 58);
}

static int	handle_overflows(unsigned long long *number, int sign)
{
	if (*number > 2147483647 && sign == 1)
		return (philo_error("integer overflow"));
	if (*number > 2147483647 && sign == -1)
		return (philo_error("integer underflow"));
	return (1);
}

int	philo_atoi(int *result, const char *nptr)
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
	if (sign == -1)
		return (philo_error("expected a non-negative number"));
	i += is_sign;
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		number = 10 * number + (nptr[i] - '0');
		if (!handle_overflows(&number, sign))
			return (0);
		i++;
	}
	if (nptr[i] && !ft_isdigit(nptr[i]))
		return (philo_error("not an integer"));
	*result = sign * number;
	return (1);
}

int	philo_atoui(unsigned long long *result, const char *nptr)
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
	if (sign == -1)
		return (philo_error("exptected a non-negative number"));
	i += is_sign;
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		number = 10 * number + (nptr[i] - '0');
		if (!handle_overflows(&number, sign))
			return (0);
		i++;
	}
	if (nptr[i] && !ft_isdigit(nptr[i]))
		return (philo_error("not an integer"));
	*result = sign * number;
	return (1);
}
