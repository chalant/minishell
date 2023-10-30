/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:32:40 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/30 14:20:38 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// returns the next instance of *end
static int	ms_skip_quoted(const char **end)
{
	char	quote;

	quote = **end;
	*end = ft_strchr(*end + 1, quote);
	if (!*end)
		return (2);
	return (0);
}

// mallocs and adds token including start but not end
static int	ms_add_token(const char *start, const char *end, t_darray *tokens, t_token *token)
{
	if (start == end)
		return (0);
	token->string = malloc(sizeof(char) * (end - start + 1));
	if (ft_strchr(RESERVED_SYMBOLS, *start))
		token->flags |= IS_RESERVED;
	if (!token->string)
		return (-1);
	ft_strlcpy(token->string, start, end - start + 1);
	return (ft_darray_append(tokens, token));
}

static const char	*ms_handle_symbol(const char *end, t_darray *tokens)
{
	const char	*start;
	t_token		new;

	new.flags = 0;
	if (!*end)
		return (end);
	if (ft_strchr(SKIP_SYMBOLS, *end))
		return (end + 1);
	start = end;
	end++;
	if (ft_strchr(DOUBLE_SYMBOLS, *start) && *start == *end)
	{
		new.flags |= IS_RESERVED;
		end++;
	}
	ms_add_token(start, end, tokens, &new);
	return (end);
}

// for use in ft_darray_delete
void	ms_clear_token(void *token)
{
	free(((t_token *)token)->string);
}

// for now just 'exit()' on early exits (malloc and unclosed quotes)
// it's possible to put an add_flag ft in the while loop if needed
int	ms_tokeniser(const char *input, t_darray *tokens)
{
	const char	*start;
	const char	*end;
	t_token		tok;

	tok.flags = 0;
	end = input;
	while (end && *end)
	{
		start = end;
		while (*end && !ft_strchr(RESERVED_SYMBOLS, *end))
		{
			if (*end == '"' || *end == '\'')
			{
				//todo: handle errors
				ms_skip_quoted(&end);
				tok.flags |= IS_QUOTED;
			}
			end++;
		}
		//todo:handle errors
		ms_add_token(start, end, tokens, &tok);
		end = ms_handle_symbol(end, tokens);
	}
	tok.string = NULL;
	return (ft_darray_append(tokens, &tok));
}

/* TEST MAIN AND PRINT FOR TOKENS RIGHT HERE */

// prints all tokens in {t, t} format
void	ms_print_tokens(t_darray *tokens)
{
	int	i;

	i = 0;
	printf("TOKENS: {");
	while (i < tokens->size)
	{
		printf("%s", ((t_token *)(tokens->contents + i * tokens->type_size))->string);
		i++;
		if (i < tokens->size)
			printf(",");
	}
	printf("}\n");
}
