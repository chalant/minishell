/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_wildcard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:34:25 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/31 17:56:30 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_wildcard_cmp(struct dirent *entryp, char *token)
{
}

// returns 0 if succesful
// returns malloced strs in buf
// buf is an initialised empty darray
int	ms_wildcard(t_darray *buf, char *token)
{
	DIR				*dirp;
	struct dirent	*entryp;
	char			*str;

	dirp = opendir(".");
	if (!dirp)
		return (ms_print_error("opendir", ".", 1));
	entryp = readdir(dirp);
	while (entryp)
	{
		entryp = readdir(dirp);
	}
	closedir(dirp);
	if (!buf->size)
	{
		str = ft_strdup(token);
		if (!str || ft_darray_append(buf, str))
			return (1);
// malloc error (don't have that header file in this branch yet)
	}
	return (0);
}
