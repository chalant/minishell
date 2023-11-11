/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_wildcard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:34:25 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/11 14:33:03 by bvercaem         ###   ########.fr       */
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
	return (ret);
}

static int	ms_prep_new(struct dirent *entryp, t_token *new)
{
	ms_init_token(new);
	new->mask_exp = ft_calloc(entryp->d_namlen + 1, sizeof(char));
	if (!new->mask_exp)
		return (ERR_MALLOC);
	return (0);
}

// returns n
static int	ms_catnmask(t_token *new, int n, char fill)
{
	char	*str;
	int		i;

	i = n;
	str = new->mask_exp;
	while (*str)
		str++;
	while (i--)
		str[i] = fill;
	return (n);
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

// returns 1 on malloc error
static int	ms_wildcard_add(t_darray *tokens, struct dirent *entryp, t_token *new)
{
	new->string = malloc(sizeof(char) * (entryp->d_namlen + 1));
	if (!new->string)
		return (ERR_MALLOC);
// malloc error
	ft_strlcpy(new->string, entryp->d_name, entryp->d_namlen + 1);
	ms_add_flags(new);
	if (ft_darray_append(tokens, new))
		return (ERR_MALLOC);
// malloc error
	return (0);
}

// returns -1 if no match, and amount of chars compared if a match
// includes terminating 0
static int	ms_cmp_until_wc(char *name, char *card, int *qts)
{
	int		i;
	char	cqt;

	i = 0;
	*qts = 0;
	while ((name[i] || card[i + *qts]) && (*qts % 2 || card[i + *qts] != '*'))
	{
		while ((!(*qts % 2) && (card[i + *qts] == '"' || card[i + *qts] == '\'')) || ((*qts % 2) && cqt == card[i + *qts]))
		{
			if (!(*qts % 2))
				cqt = card[i + *qts];
			(*qts)++;
		}
		if ((!name[i] && !card[i + *qts]) || (!(*qts % 2) && card[i + *qts] == '*'))
			break ;
		if (name[i] != card[i + *qts])
		{
			return (-1);
		}
		i++;
	}
	if (!card[i + *qts] || card[i + *qts] == '*')
		return (i);
	return (-1);
}

// returns 1 if it's a match
static int	ms_wildcard_cmp(struct dirent *entryp, char *card, t_token *new)
{
	char	*name;
	char	*mask;
	int		i;
	int		qts;

	name = entryp->d_name;
	i = ms_cmp_until_wc(name, card, &qts);
	if (i == -1)
	{
		ms_clear_token(new);
		return (0);
	}
	ms_catnmask(new, i, '0');
	name += i;
	if (!*name && (!card[i + qts] || !card[i + qts + 1]))
		return (1);
	mask = new->mask_exp + i;
	card += i + qts + 1;
	while (*card)
	{
		while (*name)
		{
			i = ms_cmp_until_wc(name, card, &qts);
			if (i > -1)
				break ;
			name++;
			*mask = '1';
			mask++;
		}
		if (!*name)
			break ;
		ms_catnmask(new, i, '0');
		name += i;
		if (!*name && (!card[i + qts] || !card[i + qts + 1]))
			return (1);
		card += i + qts + 1;
	}
	while ((*card == '"' || *card == '\'') && card[0] == card[1])
		card += 2;
	if (*card)
	{
		ms_clear_token(new);
		return (0);
	}
	return (1);
}

static int	ms_wildcard_empty(t_darray *tokens, t_token *token)
{
	int	check;

	if (ft_darray_append(tokens, token))
		return (ms_wildcard_error(NULL, token, NULL, ERR_MALLOC));
// malloc error
	return (0);
}

// returns 0 if succes
int	ms_expand_wildcard(t_darray *tokens, t_token *token)
{
	DIR				*dirp;
	struct dirent	*entryp;
	char			*card;
	t_token 		new;
	int				start_size;

	dirp = opendir(".");
	if (!dirp)
		return (ms_print_error("opendir", ".", 1));
	entryp = readdir(dirp);
	card = token->string;
	start_size = tokens->size;
	while (entryp)
	{
		if (ms_prep_new(entryp, &new))
			return (ms_wildcard_error(dirp, token, &new, ERR_MALLOC));
		if (ft_strncmp(entryp->d_name, ".", 2) && ft_strncmp(entryp->d_name, "..", 3))
			if (ms_wildcard_cmp(entryp, card, &new))
				if (ms_wildcard_add(tokens, entryp, &new))
					return (ms_wildcard_error(dirp, token, &new, ERR_MALLOC));
// malloc error (print error or no?)
		entryp = readdir(dirp);
	}
	closedir(dirp);
	if (tokens->size == start_size)
		return (ms_wildcard_empty(tokens, token));
	ms_clear_token(token);
	return (0);
}
