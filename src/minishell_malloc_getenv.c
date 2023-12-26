/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_malloc_getenv.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 19:14:49 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/26 18:56:04 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 'str' and 'qt' is not needed for a ? expansion
static char	**ms_special_getenv(char *str, int *special)
{
	char	**ret;

	(void) str;
	ret = malloc(sizeof(char *) * 2);
	if (!ret)
		return (NULL);
	ret[0] = ft_itoa((unsigned char) *special);
	if (!ret[0])
	{
		free(ret);
		return (NULL);
	}
	ret[1] = NULL;
	return (ret);
}

static char	**ms_normal_getenv(char *str, int *qt)
{
	char	**ret;
	char	*value;

	value = getenv(str);
	if (!value || !*value)
	{
		ret = malloc(sizeof(char *));
		if (ret)
			*ret = NULL;
		return (ret);
	}
	if (!*qt)
		return (ft_setsplit(value, RESERVED_SKIP));
	ret = malloc(sizeof(char *) * 2);
	if (!ret)
		return (NULL);
	ret[0] = ft_strdup(value);
	if (!ret[0])
	{
		free(ret);
		return (NULL);
	}
	ret[1] = NULL;
	return (ret);
}

// returns NULL on malloc error, no print
char	**ms_malloc_getenv(char *str, int *qt_or_special)
{
	if (ft_strchr(SPECIAL_VAR, *str))
		return (ms_special_getenv(str, qt_or_special));
	return (ms_normal_getenv(str, qt_or_special));
}
