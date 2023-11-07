/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_token_expansion.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:56:22 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/07 18:33:58 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_shift_string(char *str)
{
	while (*str)
	{
		str[0] = str[1];
		str++;
	}
}

void	ms_remove_quotes(char *str)
{
	char	qte;
// take mask_exp into account
	qte = 0;
	while (*str)
	{
		if (!qte && (*str == '"' || *str == '\''))
		{
			qte = *str;
			ms_shift_string(str);
		}
		else if (qte && *str == qte)
		{
			qte = 0;
			ms_shift_string(str);
		}
		else
			str++;
	}
}

// expand var (not implemented), remove quotes
void	ms_token_expansion(t_darray *tokens)
{
	int	i;

	i = 0;
	while (i < tokens->size)
	{
		if (((t_token *) (tokens->contents + (i * tokens->type_size)))->flags & IS_VAR)
			(void) (((t_token *) (tokens->contents + (i * tokens->type_size)))->string);
// add var expander
		if (((t_token *) (tokens->contents + (i * tokens->type_size)))->flags & IS_QUOTED)
			ms_remove_quotes(((t_token *) (tokens->contents + (i * tokens->type_size)))->string);
		i++;
	}
}
