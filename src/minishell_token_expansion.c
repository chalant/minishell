/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_token_expansion.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:56:22 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/09 13:19:40 by bvercaem         ###   ########.fr       */
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

void	ms_remove_quotes(char *str, char *mask_exp)
{
	char	qte;
	int		i;

	qte = 0;
	i = 0;
	while (str[i])
	{
		while (mask_exp && mask_exp[i] > '0')
			i++;
		if (!qte && (str[i] == '"' || str[i] == '\''))
		{
			qte = str[i];
			ms_shift_string(str);
		}
		else if (qte && str[i] == qte)
		{
			qte = 0;
			ms_shift_string(str);
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
