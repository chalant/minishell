/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 13:35:42 by ychalant          #+#    #+#             */
/*   Updated: 2023/04/11 13:35:42 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

static inline size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static inline size_t	ft_slicecmp(const char *s, const char *l, size_t st,
		size_t nd)
{
	size_t	i;

	i = 0;
	while (s[i + st] && i < nd)
	{
		if (s[i + st] != l[i])
			return (0);
		i++;
	}
	return (i == nd);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	ll;
	char	*haystack;

	i = 0;
	j = 0;
	haystack = (char *)big;
	if (!big && len == 0)
		return (NULL);
	if (!little[0])
		return (haystack);
	if (!big[0])
		return (0);
	ll = ft_strlen(little);
	i = 0;
	while (haystack[i] && i + ll <= len)
	{
		if (ft_slicecmp(big, little, j, ll))
			return (&haystack[j]);
		i++;
		j++;
	}
	return (NULL);
}
