/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:32:40 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/05 18:35:45 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// for use in ft_darray_delete
void	ms_clear_token(void *token)
{
	if (((t_token *)token)->string)
		free(((t_token *)token)->string);
}

// sets everything to 0 and returns 'token' ptr
static t_token	*ms_init_token(t_token *token)
{
	token->flags = 0;
	token->string = NULL;
	return (token);
}

static void	ms_add_flags(t_token *token, char c)
{
	if (c == '$')
		token->flags |= IS_VAR;
	if (c == '*')
		token->flags |= IS_WILDCARD;
}

// returns 1 if *symbol is a reserved sequence
static int	ms_is_reserved(const char *symbol, t_tokeniser_info *info)
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
	while (end[0][1])
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

// adds new wildcard tokens and clears old token
static int	ms_add_wildcard(t_darray *tokens, t_token *token)
{
	t_darray	buf;
	t_token		new;
	int			i;

	if (ft_darray_init(&buf, sizeof(char *), 20))
	{
		ms_clear_token(token);
		return (ERR_MALLOC);
	}
	i = ms_wildcard(&buf, token->string);
	ms_clear_token(token);
	if (i)
		return (i);
	while (i < buf.size)
	{
		ms_init_token(&new);
		new.string = ((char *) buf.contents)[i];
// add quote flag if needed
		if (ft_darray_append(tokens, &new))
		{
			while (i < buf.size)
			{
				free(((char *) buf.contents)[i]);
				i++;
			}
			ft_darray_delete(&buf, NULL);
			return (ERR_MALLOC);
		}
		i++;
	}
	ft_darray_delete(&buf, NULL);
	return (0);
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
	if (token->flags & IS_WILDCARD)
		return (ms_add_wildcard(tokens, token));
	if (ft_darray_append(tokens, token) == -1)
	{
		ms_clear_token(token);
		return (ERR_MALLOC);
	}
	return (0);
}

static const char	*ms_handle_symbol(const char *end, t_darray *tokens, t_token *token, t_tokeniser_info *info)
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
	//todo: handle errors
	ms_add_token(start, end, tokens, token);
	return (end);
}

// for now just 'exit()' on early exits (malloc and unclosed quotes)
// it's possible to put an add_flag ft in the while loop if needed
int	ms_tokeniser(const char *input, t_darray *tokens, t_tokeniser_info *info)
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
				//todo: handle errors
				ms_skip_quoted(&token, &end);
			}
			// use this ft to add flags
			ms_add_flags(&token, *end);
			end++;
		}
		//todo: handle errors
		ms_add_token(start, end, tokens, &token);
		end = ms_handle_symbol(end, tokens, &token, info);
	}
	//todo: handle errors
	return (ft_darray_append(tokens, ms_init_token(&token)));
}

t_tokeniser_info	*ms_token_info(t_tokeniser_info *ti, const char *res_single, const char *res_double, const char *res_skip)
{
	ti->reserved_single = res_single;
	ti->reserved_double = res_double;
	ti->reserved_skip = res_skip;
	return (ti);
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
			printf(" , ");
	}
	printf("}\n");
}

// int	main(int ac, char **av)
// {
// 	int					i;
// 	t_darray			array;
// 	t_tokeniser_info	info;

// 	(void)ac;
// 	ms_token_info(&info, RESERVED_SINGLE, RESERVED_DOUBLE, RESERVED_SKIP);
// 	i = 1;
// 	while (i < ac)
// 	{
// 		printf("input: %s\n", av[i]);
// 		if (ft_darray_init(&array, sizeof(t_token), 20))
// 			return (13);
// 		ms_tokeniser(av[i], &array, &info);
// 		ms_print_tokens(&array);
// 		ft_darray_delete(&array, ms_clear_token);
// 		i++;
// 	}
// }
