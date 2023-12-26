/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_token_expansion.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:56:22 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/26 17:22:06 by bvercaem         ###   ########.fr       */
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

// unsets the IS_QUOTED flag
void	ms_remove_quotes(t_token *token)
{
	char	qte;
	int		i;

	token->flags &= ~(IS_QUOTED);
	qte = 0;
	i = 0;
	while (token->string[i])
	{
		if (token->mask_exp)
			while (token->mask_exp[i] > '0')
				i++;
		if (!qte && (token->string[i] == '"' || token->string[i] == '\''))
		{
			qte = token->string[i];
			ms_shift_strings(token->string, token->mask_exp, i);
		}
		else if (qte && token->string[i] == qte)
		{
			qte = 0;
			ms_shift_strings(token->string, token->mask_exp, i);
		}
		else if (token->string[i])
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
			ms_remove_quotes(token);
		i++;
	}
}
