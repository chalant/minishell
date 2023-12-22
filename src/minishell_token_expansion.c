/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_token_expansion.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:56:22 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/22 16:18:12 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// shifts as long as a[i++]
void	ms_shift_strings(char *a, char *b, int i)
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

// removes quotes innit
void	ms_quote_expansion(t_darray *tokens, int start_index)
{
	int		i;
	t_token	*token;

	i = start_index;
	while (i < tokens->size)
	{
		token = ((t_token *) tokens->contents) + i;
		if (token->flags & IS_QUOTED)
			ms_remove_quotes(token->string, token->mask_exp);
		i++;
	}
}
