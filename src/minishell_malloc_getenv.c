/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_malloc_getenv.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 19:14:49 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/21 19:29:59 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ms_special_getenv(char *str, int *qt, char *mask_f, t_token *new)
{
	char	**ret;

	(void) str;
	(void) qt;
	new->flags |= IS_SPECIAL;
	ret = malloc(sizeof(char *) * 2);
	if (!ret)
		return (NULL);
	ret[0] = ft_strdup("$$?");
	if (!ret[0])
	{
		free(ret);
		return (NULL);
	}
	ret[1] = NULL;
	*mask_f = '3';
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

char	**ms_malloc_getenv(char *str, int *qt, char *mask_f, t_token *new)
{
	if (ft_strchr(SPECIAL_VAR, *str))
		return (ms_special_getenv(str, qt, mask_f, new));
	return (ms_normal_getenv(str, qt));
}
