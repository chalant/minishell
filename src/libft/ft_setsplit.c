/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 10:06:41 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/27 20:20:31 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_sep(char c, char *charset)
{
	int	i;

	i = 0;
	while (*(charset + i))
	{
		if (c == *(charset + i))
			return (i);
		i++;
	}
	return (-1);
}

int	ft_stringlen(char *str, char *charset)
{
	int	l;

	l = 0;
	while (*(str + l) && ft_is_sep(*(str + l), charset) < 0)
		l++;
	return (l);
}

int	ft_fill_string(char **split, int strnbr, char *str, char *charset)
{
	int	l;
	int	i;

	l = ft_stringlen(str, charset);
	if (!l)
		return (0);
	i = 0;
	*(split + strnbr) = (char *) malloc((l + 1) * sizeof (char));
	if (!*(split + strnbr))
	{
		ft_clear_ds(split);
		return (0);
	}
	*(*(split + strnbr) + l) = 0;
	while (i < l)
	{
		*(*(split + strnbr) + i) = *(str + i);
		i++;
	}
	return (1);
}

static int	ft_splitlen(char *str, char *charset)
{
	int	l;

	if (!str || !charset)
		return (0);
	l = 0;
	while (*str)
	{
		while (*str && ft_is_sep(*str, charset) >= 0)
			str++;
		if (*str)
			l++;
		while (*str && ft_is_sep(*str, charset) < 0)
			str++;
	}
	return (l);
}

char	**ft_setsplit(char *str, char *charset)
{
	int		l;
	char	**split;

	l = ft_splitlen(str, charset);
	split = (char **) malloc((l + 1) * sizeof (char *));
	if (!split)
		return ((char **)0);
	*(split + l) = 0;
	if (l == 0)
		return (split);
	l = 0;
	while (*str && l >= 0)
	{
		if (ft_is_sep(*str, charset) < 0)
		{
			if (!ft_fill_string(split, l, str, charset))
				return ((char **)0);
			l++;
			while (*str && ft_is_sep(*str, charset) < 0)
				str++;
		}
		else
			str++;
	}
	return (split);
}
