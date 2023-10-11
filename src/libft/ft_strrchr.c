/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 13:35:42 by ychalant          #+#    #+#             */
/*   Updated: 2023/04/11 13:35:42 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	char	cc;
	int		i;
	int		last_occurrence;
	int		match;

	cc = (char)c;
	last_occurrence = -1;
	i = 0;
	while (s[i])
	{
		match = s[i] == cc;
		last_occurrence = match * i + last_occurrence * !match;
		i++;
	}
	if (cc == '\0')
		return ((char *)s + i);
	if (last_occurrence != -1)
		return ((char *)s + last_occurrence);
	return (NULL);
}
