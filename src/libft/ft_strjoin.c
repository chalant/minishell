/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yves <yves@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 13:35:42 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/16 16:53:28 by yves             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static inline size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static inline int	fill(char *s, char const *with, size_t from, size_t until)
{
	size_t	i;

	i = 0;
	while (i < until)
	{
		s[i + from] = with[i];
		i++;
	}
	return (i + from);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	ls1;
	size_t	ls2;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	ls1 = ft_strlen(s1);
	ls2 = ft_strlen(s2);
	str = malloc(sizeof(char) * (ls1 + ls2 + 1));
	if (!str)
		return (NULL);
	str[fill(str, s2, fill(str, s1, 0, ls1), ls2)] = '\0';
	return (str);
}
