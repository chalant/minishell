/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_grammar_matching.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:02:14 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 18:38:44 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_eof_token(t_ms_context *context)
{
	t_token			tok;
	t_parsing_data	data;

	data = context->parse_data;
	context->parse_data.tokens->size -= 1;
	ms_init_token(&tok);
	tok.flags |= IS_RESERVED;
	tok.flags |= IS_EOF;
	if (ft_darray_append(data.tokens, &tok) < 0)
		return (-1);
	if (add_earley_set(data.earley_sets, 5) < 0)
		return (-1);
	return (0);
}

static int	add_parsing_data(t_parsing_data *parse_data, char *line)
{
	int				init_size;
	t_token_info	info;

	ms_token_info(&info, RESERVED_SINGLE, RESERVED_DOUBLE, RESERVED_SKIP);
	init_size = parse_data->tokens->size - 1;
	ms_tokeniser(&line, parse_data->tokens, &info);
	while (++init_size < parse_data->tokens->size)
	{
		if (add_earley_set(parse_data->earley_sets, 5) < 0)
			return (-1);
		if (add_adjacency_list(parse_data->chart, sizeof(t_ms_edge), 5) < 0)
			return (-1);
	}
	return (0);
}

// launches a prompt in case of an open reserved symbol.
int	ms_prompt_command(t_ms_symbol *symbol, t_token *token)
{
	char			*line;
	t_ms_context	*context;

	if (token->string)
		return (0);
	context = (t_ms_context *)symbol->context;
	line = readline("> ");
	if (line && ms_join_line(context, line, " ") < 0)
		return (-1);
	if (!line)
		return (add_eof_token(context));
	if (add_parsing_data(&context->parse_data, line) < 0)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (1);
}

// only works for reserved symbols
int	ms_match_equal(t_ms_symbol *symbol, t_token *token)
{
	size_t	len;

	if (!token->string)
		return (0);
	if (!(token->flags & IS_RESERVED))
		return (0);
	len = ft_strlen(symbol->name);
	if (ft_strlen(token->string) != len)
		return (0);
	if (ft_strncmp(symbol->name, token->string, len) == 0)
		return (1);
	return (0);
}

int	ms_match_string(t_ms_symbol *symbol, t_token *token)
{
	int	i;

	(void)symbol;
	if (!token->string)
		return (0);
	i = 0;
	if (token->flags & IS_RESERVED)
		return (0);
	while (token->string[i])
	{
		if (!ft_isascii(token->string[i]))
			return (0);
		i++;
	}
	return (1);
}
