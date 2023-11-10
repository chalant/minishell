/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_token_expansion.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:56:22 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/10 21:25:02 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// shifts as long as a[i++]
static void	ms_shift_strings(char *a, char *b, int i)
{
	while (a[i])
	{
		if (a)
			a[i] = a[i + 1];
		if (b)
			b[i] = b[i + 1];
		i++;
	}
}

void	ms_remove_quotes(char *str, char *mask_exp)
{
	char	qte;
	int		i;

	qte = 0;
	i = 0;
	while (str[i])
	{
		if (mask_exp)
			while (mask_exp[i] > '0')
				i++;
		if (!qte && (str[i] == '"' || str[i] == '\''))
		{
			qte = str[i];
			ms_shift_strings(str, mask_exp, i);
		}
		else if (qte && str[i] == qte)
		{
			qte = 0;
			ms_shift_strings(str, mask_exp, i);
		}
		else if (str[i])
			i++;
	}
}

// expand var (not implemented), remove quotes
void	ms_token_expansion(t_darray *tokens)
{
	int	i;

	i = 0;
	while (i < tokens->size)
	{
// wildcard and var are done during tokenising
		if (((t_token *) (tokens->contents + (i * tokens->type_size)))->flags & IS_QUOTED)
			ms_remove_quotes(((t_token *) (tokens->contents + (i * tokens->type_size)))->string);
		i++;
	}
}
