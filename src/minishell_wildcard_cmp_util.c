/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_wildcard_cmp_util.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 20:21:28 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/15 20:24:00 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// returns n, increments mask by n
// if !token->mask_exp, '0' char is used instead
int	ms_cpy_mask(int n, char **mask, char *card, t_token *token)
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

static void	ms_cmp_count_quote(t_wildcard_data *d, int *qts, char *cqt, int *i)
{
	if (!(*qts % 2))
		*cqt = d->card[*i + *qts];
	(*qts)++;
}

// returns -1 if no match, and amount of chars compared if a match
// includes terminating 0
int	ms_cmp_until_wc(t_wildcard_data *d, int *qts, t_token *token)
{
	int		i;
	char	cqt;
	char	*mask;

	i = 0;
	mask = token->mask_exp + (d->card - token->string);
	*qts = 0;
	cqt = 0;
	while ((d->name[i] || d->card[i + *qts])
		&& (*qts % 2 || d->card[i + *qts] != '*'))
	{
		while ((!token->mask_exp || mask[i + *qts] == '0') && ((!(*qts % 2)
					&& (d->card[i + *qts] == '"' || d->card[i + *qts] == '\''))
				|| ((*qts % 2) && cqt == d->card[i + *qts])))
			ms_cmp_count_quote(d, qts, &cqt, &i);
		if ((!d->name[i] && !d->card[i + *qts])
			|| (!(*qts % 2) && d->card[i + *qts] == '*'))
			break ;
		if (d->name[i] != d->card[i + *qts])
			return (-1);
		i++;
	}
	if (!d->card[i + *qts] || d->card[i + *qts] == '*')
		return (i);
	return (-1);
}
