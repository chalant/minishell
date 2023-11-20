/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:32:40 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/20 13:48:13 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// sets everything to 0 and returns 'token' ptr
t_token	*ms_init_token(t_token *token)
{
	token->flags = 0;
	token->string = NULL;
	token->mask_exp = NULL;
	return (token);
}

static void	ms_add_flags_char(t_token *token, char c)
{
	if (c == '$')
		token->flags |= IS_VAR;
	if (c == '*')
		token->flags |= IS_WILDCARD;
}

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

// for now just 'exit()' on early exits (malloc and unclosed quotes)
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
				ms_skip_quoted(&token, &end);
			}
			// use this ft to add flags
			ms_add_flags_char(&token, *end);
			end++;
		}
		//todo: handle errors (clear tokens and stuff)
		ms_add_token(start, end, tokens, &token);
		end = ms_handle_symbol(end, tokens, &token, info);
	}
	ms_token_expansion(tokens);
	//todo: handle errors
	return (ft_darray_append(tokens, ms_init_token(&token)));
}

t_token_info	*ms_token_info(t_token_info *ti, const char *res_single, const char *res_double, const char *res_skip)
{
	ti->reserved_single = res_single;
	ti->reserved_double = res_double;
	ti->reserved_skip = res_skip;
	return (ti);
}

/* TEST MAIN AND PRINT FOR TOKENS RIGHT HERE */
/*
// prints all tokens in {t, t} format
void	ms_print_tokens(t_darray *tokens)
{
	int		i;
	t_token	*token;

	i = 0;
	printf("TOKENS: {");
	while (i < tokens->size)
	{
		token = (t_token *)(tokens->contents + i * tokens->type_size);
		// printf("[");
		// if (token->flags & IS_QUOTED)
		// 	printf("q");
		// else
		// 	printf(" ");
		// if (token->flags & IS_WILDCARD)
		// 	printf("w");
		// else
		// 	printf(" ");
		// if (token->flags & IS_VAR)
		// 	printf("v");
		// else
		// 	printf(" ");
		// printf("]  ");
		printf("%6s", token->string);
		i++;
		if (i < tokens->size)
			printf(" , ");
	}
	printf("}\n");
}

// prints all masks in {t, t} format
void	ms_print_masks(t_darray *tokens)
{
	int		i;
	t_token	*token;

	i = 0;
	printf(" MASKS: {");
	while (i < tokens->size)
	{
		token = (t_token *)(tokens->contents + i * tokens->type_size);
		// printf("%7s", "");
		printf("%6s", token->mask_exp);
		i++;
		if (i < tokens->size)
			printf(" , ");
	}
	printf("}\n");
}

int	main(int ac, char **av)
{
	int				i;
	t_darray		array;
	t_token_info	info;

	(void)ac;
	ms_token_info(&info, RESERVED_SINGLE, RESERVED_DOUBLE, RESERVED_SKIP);
	i = 1;
	while (i < ac)
	{
		printf("input: %s\n", av[i]);
		if (ft_darray_init(&array, sizeof(t_token), 20))
			return (13);
		ms_tokeniser(av[i], &array, &info);
		ms_print_tokens(&array);
		ms_print_masks(&array);
		ft_darray_delete(&array, ms_clear_token);
		i++;
	}
}
*/
