/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 13:35:42 by ychalant          #+#    #+#             */
/*   Updated: 2023/04/11 13:35:42 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static inline int	is_sep(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static inline size_t	ft_count_trim(const char *s, char const *set)
{
	size_t	i;
	size_t	count;

	if (!*s)
		return (0);
	i = 0;
	count = 0;
	while (s[i])
	{
		count++;
		i++;
	}
	i--;
	while (is_sep(s[i], set))
	{
		count--;
		i--;
	}
	return (count);
}

static inline void	fill(const char *s, char *trim, size_t count)
{
	size_t	j;

	j = 0;
	while (j < count)
	{
		trim[j] = s[j];
		j++;
	}
	trim[j] = '\0';
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	count;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	while (*s1 && is_sep(*s1, set))
		s1++;
	count = ft_count_trim(s1, set);
	str = malloc(sizeof(char) * (count + 1));
	if (!str)
		return (NULL);
	str[0] = '\0';
	if (!count)
		return (str);
	fill(s1, str, count);
	return (str);
}
