/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 13:35:42 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/23 14:39:01 by bvercaem         ###   ########.fr       */
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

char	*ft_strdup(const char *s)
{
	size_t	n;
	char	*ptr;

	if (!s)
		return (NULL);
	n = ft_strlen(s);
	ptr = malloc(sizeof(char) * (n + 1));
	if (!ptr)
		return (NULL);
	ptr[n] = '\0';
	while (n--)
		ptr[n] = s[n];
	return (ptr);
}
