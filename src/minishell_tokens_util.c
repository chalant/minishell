/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens_util.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:30:35 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/20 15:40:33 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_info	*ms_token_info(t_token_info *ti, const char *res_single,
	const char *res_double, const char *res_skip)
{
	ti->reserved_single = res_single;
	ti->reserved_double = res_double;
	ti->reserved_skip = res_skip;
	return (ti);
}

// sets everything to 0 and returns 'token' ptr
t_token	*ms_init_token(t_token *token)
{
	token->flags = 0;
	token->string = NULL;
	token->mask_exp = NULL;
	return (token);
}

// for use in ft_darray_delete
void	ms_clear_token(void *token)
{
	if (((t_token *)token)->string)
	{
		free(((t_token *)token)->string);
		((t_token *)token)->string = NULL;
	}
	if (((t_token *)token)->mask_exp)
	{
		free(((t_token *)token)->mask_exp);
		((t_token *)token)->mask_exp = NULL;
	}
}

void	ms_add_flags_char(t_token *token, char c)
{
	if (c == '$')
		token->flags |= IS_VAR;
	if (c == '*')
		token->flags |= IS_WILDCARD;
}

void	ms_add_flags_str(t_token *new)
{
	int		i;
	char	qt;

	i = 0;
	qt = 0;
	while (new->string[i])
	{
		if (!qt && new->mask_exp[i] == '0' && (new->string[i] == '"' || new->string[i] == '\''))
		{
			qt = new->string[i];
			new->flags |= IS_QUOTED;
			i++;
		}
		if (!qt && new->string[i] == '*')
			new->flags |= IS_WILDCARD;
		if (qt && new->mask_exp[i] == '0' && new->string[i] == qt)
			qt = 0;
		i++;
	}
}
