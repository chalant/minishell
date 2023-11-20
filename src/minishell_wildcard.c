/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_wildcard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:34:25 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/20 17:23:51 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	ms_prep_new(struct dirent *entryp, t_token *new)
{
	ms_init_token(new);
	new->mask_exp = ft_calloc(ft_strlen(entryp->d_name) + 1, sizeof(char));
	if (!new->mask_exp)
		return (ERR_MALLOC);
	return (0);
}

// returns 1
static int	ms_finish_mask(char *name, char *mask, char fill)
{
	int	i;

	if (!mask)
		return (1);
	i = 0;
	while (name[i])
	{
		mask[i] = fill;
		i++;
	}
	return (1);
}

// only looks for quotes
static void	ms_add_flags(t_token *token)
{
	int	i;

	token->flags = 0;
	if (!token->string)
		return ;
	i = 0;
	while (token->string[i] && !(token->flags & IS_QUOTED))
	{
		if (token->mask_exp[i] == '0'
			&& (token->string[i] == '"' || token->string[i] == '\''))
			token->flags |= IS_QUOTED;
		i++;
	}
}

// returns 1 on malloc error, doesn't clear 'new'
static int	ms_wildcard_add(t_darray *tokens, struct dirent *entryp, t_token *new)
{
	int	namlen;

	namlen = ft_strlen(entryp->d_name);
	new->string = malloc(sizeof(char) * (namlen + 1));
	if (!new->string)
		return (ERR_MALLOC);
	ft_strlcpy(new->string, entryp->d_name, namlen + 1);
	ms_add_flags(new);
	if (ft_darray_append(tokens, new))
		return (ERR_MALLOC);
	return (0);
}

// returns n, increments mask by n
// if !token->mask_exp, '0' char is used instead
static int	ms_cpy_mask(int n, char **mask, char *card, t_token *token)
{
	char	*old_mask;
	int		i;

	if (!n)
		return (0);
	old_mask = token->mask_exp + (card - token->string);
	i = 0;
	while (i < n)
	{
		if (token->mask_exp)
			(*mask)[i] = old_mask[i];
		else
			(*mask)[i] = '0';
		i++;
	}
	(*mask) += n;
	return (n);
}

// returns -1 if no match, and amount of chars compared if a match
// includes terminating 0
static int	ms_cmp_until_wc(char *name, char *card, int *qts, t_token *token)
{
	int		i;
	char	cqt;
	char	*mask;

	i = 0;
	mask = token->mask_exp + (card - token->string);
	*qts = 0;
	cqt = 0;
	while ((name[i] || card[i + *qts]) && (*qts % 2 || card[i + *qts] != '*'))
	{
		while ((!token->mask_exp || mask[i + *qts] == '0') && ((!(*qts % 2) && (card[i + *qts] == '"' || card[i + *qts] == '\'')) || ((*qts % 2) && cqt == card[i + *qts])))
		{
			if (!(*qts % 2))
				cqt = card[i + *qts];
			(*qts)++;
		}
		if ((!name[i] && !card[i + *qts]) || (!(*qts % 2) && card[i + *qts] == '*'))
			break ;
		if (name[i] != card[i + *qts])
			return (-1);
		i++;
	}
	if (!card[i + *qts] || card[i + *qts] == '*')
		return (i);
	return (-1);
}

// returns 1 if it's a match
static int	ms_wildcard_cmp(struct dirent *entryp, t_token *token, t_token *new)
{
	char	*card;
	char	*name;
	char	*mask;
	int		i;
	int		qts;

	card = token->string;
	name = entryp->d_name;
	i = ms_cmp_until_wc(name, card, &qts, token);
	if (i == -1)
	{
		ms_clear_token(new);
		return (0);
	}
	mask = new->mask_exp;
	name += ms_cpy_mask(i, &mask, card, token);
	if (!*name && (!card[i + qts] || !card[i + qts + 1]))
		return (1);
	card += i + qts + 1;
	while (*card)
	{
		while (*name)
		{
			i = ms_cmp_until_wc(name, card, &qts, token);
			if (i > -1)
				break ;
			name++;
			*mask = '1';
			mask++;
		}
		if (!*name)
			break ;
		name += ms_cpy_mask(i, &mask, card, token);
		if (!*name && (!card[i + qts] || !card[i + qts + 1]))
			return (1);
		card += i + qts + 1;
	}
	while (*card == '*' || ((*card == '"' || *card == '\'') && card[0] == card[1]))
	{
		if (*card != '*')
			card++;
		card++;
	}
	if (*card)
	{
		ms_clear_token(new);
		return (0);
	}
	return (ms_finish_mask(name, mask, '1'));
}

static int	ms_wildcard_empty(t_darray *tokens, t_token *token)
{
	if (ft_darray_append(tokens, token))
		return (ms_wildcard_error(NULL, token, NULL, ERR_MALLOC));
	return (0);
}

// returns 1 if entry is a suitable candidate
static int	ms_pre_check(t_token *token, struct dirent *entryp)
{
	if (!ft_strncmp(entryp->d_name, ".", 2) || !ft_strncmp(entryp->d_name, "..", 3))
		return (0);
	if (entryp->d_name[0] == '.' && token->string[0] != '.')
		return (0);
	if (entryp->d_name[0] != '.' && token->string[0] == '.')
		return (0);
	return (1);
}

// returns 0 if succes
// on error, doesn't clear 'tokens', prints message
int	ms_expand_wildcard(t_darray *tokens, t_token *token)
{
	DIR				*dirp;
	struct dirent	*entryp;
	t_token			new;
	int				start_size;

	dirp = opendir(".");
	if (!dirp)
	{
		ms_clear_token(token);
		return (ms_perror("filename expansion", ".", NULL, errno));
	}
	entryp = readdir(dirp);
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

// errors are all good! (i'm pretty sure)