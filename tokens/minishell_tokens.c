/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:32:40 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/15 16:52:16 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_tokens.h"

// returns the next instance of *end
static const char	*ms_skip_quoted(const char *end)
{
	char	quote;

	quote = *end;
	end++;
	end = ft_strchr(end, quote);
	if (!end)
		exit(1);
// error: unclosed quote
	return (end);
}

// mallocs and adds token including start but not end
static void	ms_add_token(const char *start, const char *end, t_darray *tokens)
{
	t_token	new;

	if (start == end)
		return ;
	new.string = malloc(sizeof(char) * (end - start + 1));
	if (!new.string)
		exit(1);
// error: malloc failed
	ft_strlcpy(new.string, start, end - start + 1);
	if (ft_darray_append(tokens, &new) == -1)
		exit(1);
// error: malloc failed
}

static const char	*ms_handle_symbol(char *end, t_darray *tokens)
{
	const char	*start;

	if (!*end)
		return (end);
	if (ft_strchr(SKIP_SYMBOLS, *end))
		return (end + 1);
	start = end;
	end++;
	if (ft_strchr(DOUBLE_SYMBOLS, *start) && *start == *end)
		end++;
	ms_add_token(start, end, tokens);
	return (end);
}

// frees all malloced token strings and calls ft_darray_delete
void	ms_clear_tokens(t_darray *tokens)
{
	int	i;

	i = 0;
	while (i < tokens->size)
	{
		free(((t_token)((unsigned char *) tokens->contents)[i * tokens->type_size]).string);
		i++;
	}
	ft_darray_delete(tokens);
}

void	ms_tokeniser(const char *input, t_darray *tokens)
{
	const char	*start;
	const char	*end;

	end = input;
	while (end && *end)
	{
		start = end;
		while (*end && !ft_strchr(RESERVED_SYMBOLS, *end))
		{
			end++;
			if (*end == '"' || *end == '\'')
				end = ms_skip_quoted(end);
		}
		ms_add_token(start, end, tokens);
		end = ms_handle_symbol(end, tokens);
	}
}
