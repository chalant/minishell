/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:32:40 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/23 15:05:43 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// returns 1 if *symbol is a reserved sequence
static int	ms_is_reserved(const char *symbol, t_token_info *info)
{
	if (ft_strchr(info->reserved_single, *symbol))
		return (1);
	if (ft_strchr(info->reserved_double, *symbol) && symbol[0] == symbol[1])
		return (1);
	return (0);
}

// returns the next instance of *end
// adds the IS_VAR flag if applicable
static int	ms_skip_quoted(t_token *token, const char **end)
{
	char	quote;

	token->flags |= IS_QUOTED;
	quote = **end;
	while ((*end)[1])
	{
		(*end)++;
		if (**end == quote)
			return (0);
		if (quote == '"')
			if (**end == '$')
				token->flags |= IS_VAR;
	}
	return (ERR_QUOTE_UNCLOSED);
}

// mallocs and adds token including start but not end
static int	ms_add_token(const char *start, const char *end, t_darray *tokens, t_token *token)
{
	if (start == end)
		return (0);
	token->string = malloc(sizeof(char) * (end - start + 1));
	if (!token->string)
		return (ERR_MALLOC);
	ft_strlcpy(token->string, start, end - start + 1);
	if (token->flags & IS_VAR)
		return (ms_expand_var(tokens, token));
	if (token->flags & IS_WILDCARD)
		return (ms_expand_wildcard(tokens, token));
	if (ft_darray_append(tokens, token) == -1)
	{
		ms_clear_token(token);
		return (ERR_MALLOC);
	}
	return (0);
}

static const char	*ms_handle_symbol(const char *end, t_darray *tokens, t_token *token, t_token_info *info)
{
	const char	*start;

	if (!*end)
		return (end);
	if (ft_strchr(info->reserved_skip, *end))
		return (end + 1);
	ms_init_token(token);
	token->flags |= IS_RESERVED;
	start = end;
	end++;
	if (ft_strchr(info->reserved_double, *start) && *start == *end)
		end++;
	//todo: handle errors (clear tokens and stuff)
	ms_add_token(start, end, tokens, token);
	return (end);
}

// NULL terminates tokens with a fresh initialised token
// returns > 0 on failure, doesn't delete 'tokens'
int	ms_tokeniser(const char *input, t_darray *tokens, t_token_info *info)
{
	const char	*start;
	const char	*end;
	t_token		token;

	end = input;
	while (end && *end)
	{
		ms_init_token(&token);
		start = end;
		while (*end && !ms_is_reserved(end, info))
		{
			if (*end == '"' || *end == '\'')
			{
				//todo: handle error: quote unclosed
				if (ms_skip_quoted(&token, &end))
					return (ERR_QUOTE_UNCLOSED);
			}
			ms_add_flags_char(&token, *end);
			end++;
		}
		//todo: handle errors (clear stuff?, malloc or opendir or ..?)
		if (ms_add_token(start, end, tokens, &token))
			return (ERR_MALLOC);
		end = ms_handle_symbol(end, tokens, &token, info);
	}
	ms_token_expansion(tokens);
	//todo: handle errors
	ft_darray_append(tokens, ms_init_token(&token));
	return (0);
}
