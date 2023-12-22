/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_wildcard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:34:25 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/22 16:13:16 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// close dirp, clear token, clear new, perror if ret == err_malloc, return ret
static int	ms_wildcard_error(DIR *dirp, t_token *token, t_token *new, int ret)
{
	if (dirp)
		closedir(dirp);
	if (token)
		ms_clear_token(token);
	if (new)
		ms_clear_token(new);
	if (ret == ERR_MALLOC)
		ms_perror("filename expansion", NULL, NULL, errno);
	return (ret);
}

static int	ms_prep_new(t_dirent *entryp, t_token *new)
{
	ms_init_token(new);
	new->mask_exp = ft_calloc(ft_strlen(entryp->d_name) + 1, sizeof(char));
	if (!new->mask_exp)
		return (ERR_MALLOC);
	return (0);
}

static int	ms_wildcard_empty(t_darray *tokens, t_token *token)
{
	if (ft_darray_append(tokens, token))
		return (ms_wildcard_error(NULL, token, NULL, ERR_MALLOC));
	return (0);
}

static int	ms_open_dir(DIR **dirp, t_dirent **entryp, t_token *token)
{
	*entryp = NULL;
	*dirp = opendir(".");
	if (!*dirp)
	{
		ms_clear_token(token);
		return (1);
	}
	*entryp = readdir(*dirp);
	return (0);
}

// either clears 'token' or adds it to tokens
// error: ERR_MALLOC: clear 'token', doesn't clear 'tokens', prints message
int	ms_expand_wildcard(t_darray *tokens, t_token *token)
{
	DIR			*dirp;
	t_dirent	*entryp;
	t_token		new;
	int			start_size;

	if (ms_open_dir(&dirp, &entryp, token))
		return (ms_perror("filename expansion", ".", NULL, errno));
	start_size = tokens->size;
	while (entryp)
	{
		if (ms_pre_check(token, entryp))
		{
			if (ms_prep_new(entryp, &new))
				return (ms_wildcard_error(dirp, token, &new, ERR_MALLOC));
			if (ms_wildcard_cmp(entryp, token, &new))
				if (ms_wildcard_add(tokens, entryp, &new))
					return (ms_wildcard_error(dirp, token, &new, ERR_MALLOC));
		}
		entryp = readdir(dirp);
	}
	closedir(dirp);
	if (tokens->size == start_size)
		return (ms_wildcard_empty(tokens, token));
	ms_clear_token(token);
	return (0);
}
