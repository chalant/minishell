/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:32:40 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/20 18:44:48 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_tokeniser_error(char *input, char perror)
{
	free(input);
	if (perror)
		ms_perror("tokeniser", NULL, NULL, errno);
	return (ERR_MALLOC);
}

// mallocs and adds token including start but not end
// error: prints msg
static int	ms_add_token(char *start, char *end, t_darray *tkns, t_token *token)
{
	if (start == end)
		return (0);
	token->string = malloc(sizeof(char) * (end - start + 1));
	if (!token->string)
	{
		ms_perror("malloc", NULL, NULL, errno);
		return (ERR_MALLOC);
	}
	ft_strlcpy(token->string, start, end - start + 1);
	if (tkns->size
		&& (((t_token *)tkns->contents) + tkns->size - 1)->flags & IS_HEREDOC)
		token->flags |= IS_DELIMITER;
	else if (token->flags & IS_VAR)
		return (ms_expand_var(tkns, token));
	else if (token->flags & IS_WILDCARD)
		return (ms_expand_wildcard(tkns, token));
	if (ft_darray_append(tkns, token) == -1)
	{
		ms_clear_token(token);
		ms_perror("malloc", NULL, NULL, errno);
		return (ERR_MALLOC);
	}
	return (0);
}

static char	*ms_handle_symbol(char *end,
	t_darray *tokens, t_token *token, t_tkn_info *info)
{
	char	*start;

	if (!*end)
		return (end);
	if (ft_strchr(info->reserved_skip, *end))
		return (end + 1);
	ms_init_token(token);
	token->flags |= IS_RESERVED;
	start = end;
	end++;
	if (ft_strchr(info->reserved_double, *start) && *start == *end)
	{
		end++;
		if (!ft_strncmp(start, "<<", 2))
			token->flags |= IS_HEREDOC;
	}
	if (ms_add_token(start, end, tokens, token))
		return (NULL);
	return (end);
}

// NULL terminates tokens with a fresh initialised token.
// error: free '*input', print error msg
int	ms_tokeniser(char **input, t_darray *tokens, t_tkn_info *info)
{
	char	*start_end[2];
	t_token	token;
	int		start_size;
	int		check;

	if (tokens->size)
		tokens->size--;
	start_size = tokens->size;
	start_end[1] = *input;
	while (start_end[1] && *(start_end[1]))
	{
		ms_init_token(&token);
		check = ms_until_reserved(input, start_end, &token, info);
		if (check)
			return (check);
		if (ms_add_token(start_end[0], start_end[1], tokens, &token))
			return (ms_tokeniser_error(*input, 0));
		start_end[1] = ms_handle_symbol(start_end[1], tokens, &token, info);
		if (!start_end[1])
			return (ms_tokeniser_error(*input, 0));
	}
	ms_token_expansion(tokens, start_size);
	if (ft_darray_append(tokens, ms_init_token(&token)))
		return (ms_tokeniser_error(*input, 1));
	return (0);
}
