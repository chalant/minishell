/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_wildcard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:34:25 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/05 17:57:49 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_wildcard_error(DIR *dirp, t_darray *buf, int ret)
{
	if (dirp)
		closedir(dirp);
	if (buf)
		ft_darray_delete(buf, ms_freestr_darray);
	return (ret);
}

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
static int	ms_cmp_until_wc(char *name, char *token, int *qts)
{
	int		i;
	char	cqt;

	i = 0;
	*qts = 0;
	while ((name[i] || token[i + *qts]) && (*qts % 2 || token[i + *qts] != '*'))
	{
		while ((!(*qts % 2) && (token[i + *qts] == '"' || token[i + *qts] == '\'')) || ((*qts % 2) && cqt == token[i + *qts]))
		{
			if (!(*qts % 2))
				cqt = token[i + *qts];
			(*qts)++;
		}
		if ((!name[i] && !token[i + *qts]) || (!(*qts % 2) && token[i + *qts] == '*'))
			break ;
		if (name[i] != token[i + *qts])
		{
			return (-1);
		}
		i++;
	}
	if (!token[i + *qts] || token[i + *qts] == '*')
		return (i);
	return (-1);
}

// returns 1 if it's a match
static int	ms_wildcard_cmp(struct dirent *entryp, char *token)
{
	char	*name;
	int		i;
	int		qts;

	name = entryp->d_name;
	i = ms_cmp_until_wc(name, token, &qts);
	if (i == -1)
	{
		return (0);
	}
	name += i;
	if (!*name && (!token[i + qts] || !token[i + qts + 1]))
		return (1);
	token += i + qts + 1;
	while (*token)
	{
		while (*name)
		{
			i = ms_cmp_until_wc(name, token, &qts);
			if (i > -1)
				break ;
			name++;
		}
		if (!*name)
			break ;
		name += i;
		if (!*name && (!token[i + qts] || !token[i + qts + 1]))
			return (1);
		token += i + qts + 1;
	}
	while ((*token == '"' || *token == '\'') && token[0] == token[1])
		token += 2;
	if (*token)
	{
		return (0);
	}
	return (1);
}

// returns 0 if no errors
// returns malloced strs in t_darray *buf
// buf should be an initialised empty darray
// clears buf if error encountered
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
		if (ft_strncmp(entryp->d_name, ".", 2) && ft_strncmp(entryp->d_name, "..", 3))
			if (ms_wildcard_cmp(entryp, token))
				if (ms_wildcard_add(entryp, buf))
					return (ms_wildcard_error(dirp, buf, ERR_MALLOC));
// malloc error (print error or no?)
		entryp = readdir(dirp);
	}
	closedir(dirp);
	if (!buf->size)
	{
		str = ft_strdup(token);
		if (!str || ft_darray_append(buf, &str))
			return (ms_wildcard_error(NULL, buf, ERR_MALLOC));
// malloc error
	}
	return (0);
}
