/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_wildcard_cmp.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 20:15:02 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/15 20:27:59 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_add_wc_chars(t_wildcard_data *wc_d, int *qts, t_token *token)
{
	int	i;

	i = -1;
	while (*wc_d->name)
	{
		i = ms_cmp_until_wc(wc_d, qts, token);
		if (i > -1)
			break ;
		wc_d->name++;
		*wc_d->mask = '1';
		wc_d->mask++;
	}
	return (i);
}

static int	ms_wc_cmp_loop(t_wildcard_data *wc_d, int qts, t_token *token)
{
	int	i;

	while (*wc_d->card)
	{
		i = ms_add_wc_chars(wc_d, &qts, token);
		if (!*wc_d->name)
			break ;
		wc_d->name += ms_cpy_mask(i, &wc_d->mask, wc_d->card, token);
		if (!*wc_d->name && (!wc_d->card[i + qts] || !wc_d->card[i + qts + 1]))
			return (1);
		wc_d->card += i + qts + 1;
	}
	return (0);
}

static int	ms_check_and_fill(t_wildcard_data *d, t_token *new)
{
	while (*d->card == '*'
		|| ((*d->card == '"' || *d->card == '\'') && d->card[0] == d->card[1]))
	{
		if (*d->card != '*')
			d->card++;
		d->card++;
	}
	if (*d->card)
	{
		ms_clear_token(new);
		return (0);
	}
	if (!d->mask)
		return (1);
	while (*d->name)
	{
		*d->mask = '1';
		d->name++;
		d->mask++;
	}
	return (1);
}

// returns 1 if it's a match, clears 'new' if not
int	ms_wildcard_cmp(t_dirent *entryp, t_token *token, t_token *new)
{
	t_wildcard_data	wc_d;
	int				i;
	int				qts;

	wc_d.card = token->string;
	wc_d.name = entryp->d_name;
	i = ms_cmp_until_wc(&wc_d, &qts, token);
	if (i == -1)
	{
		ms_clear_token(new);
		return (0);
	}
	wc_d.mask = new->mask_exp;
	wc_d.name += ms_cpy_mask(i, &wc_d.mask, wc_d.card, token);
	if (!*wc_d.name && (!wc_d.card[i + qts] || !wc_d.card[i + qts + 1]))
		return (1);
	wc_d.card += i + qts + 1;
	if (ms_wc_cmp_loop(&wc_d, qts, token))
		return (1);
	return (ms_check_and_fill(&wc_d, new));
}

// returns 1 if entry is a suitable candidate
int	ms_pre_check(t_token *token, t_dirent *entryp)
{
	if (!ft_strncmp(entryp->d_name, ".", 2)
		|| !ft_strncmp(entryp->d_name, "..", 3))
		return (0);
	if (entryp->d_name[0] == '.' && token->string[0] != '.')
		return (0);
	if (entryp->d_name[0] != '.' && token->string[0] == '.')
		return (0);
	return (1);
}
