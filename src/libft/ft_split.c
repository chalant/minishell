/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 13:35:42 by ychalant          #+#    #+#             */
/*   Updated: 2023/04/11 13:35:42 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static inline char	**clear_lst(char **s, int from)
{
	while (from > -1)
	{
		free(s[from]);
		from--;
	}
	free(s);
	return (NULL);
}

static inline int	ft_count(char const *s, char c)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static inline size_t	len_word(char const *s, size_t start, char c)
{
	size_t	len;

	len = 0;
	while (s[start + len] && s[start + len] != c)
		len++;
	return (len);
}

static inline char	**fill(char **str, char const *s, int count, char c)
{
	int			i;
	size_t		j;
	size_t		len;

	i = 0;
	j = 0;
	while (i < count)
	{
		while (s[j] && s[j] == c)
			j++;
		len = len_word(s, j, c);
		str[i] = ft_substr(s, j, len);
		if (!str[i])
			return (clear_lst(str, i - 1));
		j += len;
		i++;
	}
	str[count] = 0;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	int			count;
	char		**str;

	if (!s)
		return (NULL);
	count = ft_count(s, c);
	str = malloc(sizeof(char *) * (count + 1));
	if (!str)
		return (NULL);
	return (fill(str, s, count, c));
}
