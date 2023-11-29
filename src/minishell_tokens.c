/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:32:40 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/29 18:43:15 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// returns 1 if *symbol is a reserved sequence
static int	ms_is_reserved(const char *symbol, t_token_info *info)
{
if (*symbol == '\n')
printf("found a nl!\n");
	if (ft_strchr(info->reserved_single, *symbol))
		return (1);
	if (ft_strchr(info->reserved_double, *symbol) && symbol[0] == symbol[1])
		return (1);
	return (0);
}

// on error, *line = NULL
static int	ms_prompt_quote(char **end, char **start, char **line)
{
	char	*temp;
	char	*add_line;
	size_t	start_index;
	size_t	end_index;

	start_index = *start - *line;
	end_index = *end - *line;
	temp = ft_strjoin(*line, "\n");
	free(*line);
	*line = NULL;
	if (!temp)
		return (ms_perror("tokenising", NULL, NULL, errno));
// strjoin error
	add_line = readline("> ");
	if (!add_line)
	{
		free(temp);
		return (ms_perror("readline", NULL, NULL, errno));
// readline errror
	}
	*line = ft_strjoin(temp, add_line);
	free(temp);
	free(add_line);
	if (!*line)
		return (ms_perror("tokenising", NULL, NULL, errno));
// strjoin error
	*start = (*line) + start_index;
	*end = (*line) + end_index;
	return (0);
}

// returns 1 on error
// returns the next instance of *end
// adds the IS_VAR flag if applicable
static int	ms_skip_quoted(t_token *tkn, char **end, char **start, char **line)
{
	char	quote;

	tkn->flags |= IS_QUOTED;
	quote = **end;
	while (1)
	{
		while (**end)
		{
			(*end)++;
			if (**end == quote)
				return (0);
			if (quote == '"')
				if (**end == '$')
					tkn->flags |= IS_VAR;
		}
		if (ms_prompt_quote(end, start, line))
			return (1);
	}
}

// mallocs and adds token including start but not end
static int	ms_add_token(char *start, char *end, t_darray *tkns, t_token *token)
{
	if (start == end)
		return (0);
	token->string = malloc(sizeof(char) * (end - start + 1));
	if (!token->string)
		return (ERR_MALLOC);
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
		return (ERR_MALLOC);
	}
	return (0);
}

static char	*ms_handle_symbol(char *end, t_darray *tokens, t_token *token, t_token_info *info)
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
	//todo: handle errors (clear tokens and stuff)
	ms_add_token(start, end, tokens, token);
	return (end);
}

// NULL terminates tokens with a fresh initialised token
// returns > 0 on failure, doesn't delete 'tokens'
int	ms_tokeniser(char **input, t_darray *tokens, t_token_info *info)
{
	char	*start;
	char	*end;
	t_token	token;

	end = *input;
	while (end && *end)
	{
		ms_init_token(&token);
		start = end;
		while (*end && !ms_is_reserved(end, info))
		{
			if (*end == '"' || *end == '\'')
			{
				//todo: handle error: quote unclosed
				if (ms_skip_quoted(&token, &end, &start, input))
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
