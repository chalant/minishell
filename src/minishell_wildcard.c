/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_wildcard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:34:25 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/01 17:03:13 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// returns 1 on malloc error
static int	ms_wildcard_add(struct dirent *entryp, t_darray *buf)
{
	char	*new;

	new = malloc(sizeof(char) * (entryp->d_namlen + 1));
	if (!new)
		return (1);
// malloc error
	ft_strlcpy(new, entryp->d_name, entryp->d_namlen + 1);
	if (ft_darray_append(buf, &new))
	{
		free(new);
		return (1);
// malloc error
	}
	return (0);
}

// returns -1 if no match, and amount of chars compared if a match
// includes terminating 0
static int	ms_cmp_until_wc(char *name, char *token)
{
	int	i;

	i = 0;
	while ((name[i] || token[i]) && token[i] != '*')
	{
		if (name[i] != token[i])
			return (-1);
		i++;
	}
	if (!token[i] || token[i] == '*')
		return (i);
	return (-1);
}

// returns 1 if it's a match
static int	ms_wildcard_cmp(struct dirent *entryp, char *token)
{
	char	*name;
	int		i;

	name = entryp->d_name;
	i = ms_cmp_until_wc(name, token);
	if (i == -1)
		return (0);
	name += i;
	if (!*name && (!token[i] || !token[i + 1]))
		return (1);
	token += i + 1;
// printf("breakpoint 1: name = %s, token = %s\n", name, token);
	while (*token)
	{
		while (*name)
		{
			i = ms_cmp_until_wc(name, token);
			if (i > -1)
				break ;
			name++;
		}
		if (!*name)
			break ;
		name += i;
		if (!*name && (!token[i] || !token[i + 1]))
			return (1);
		token += i + 1;
// printf("breakpoint 2: name = %s, token = %s\n", name, token);
	}
	if (*token)
		return (0);
	return (1);
}

// returns 0 if succesful
// returns malloced strs in buf
// buf is an initialised empty darray"
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
// take quotes into consideration
		if (ms_wildcard_cmp(entryp, token))
			if (ms_wildcard_add(entryp, buf))
			{
				closedir(dirp);
				return (1);
			}
// malloc error
		entryp = readdir(dirp);
	}
	closedir(dirp);
	if (!buf->size)
	{
		str = ft_strdup(token);
		if (!str || ft_darray_append(buf, &str))
			return (1);
// malloc error (don't have that header file in this branch yet)
	}
	return (0);
}
