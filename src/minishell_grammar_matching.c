#include "minishell.h"

int add_eof_token(t_ms_context *context)
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

// launches a prompt in case of an open reserved symbol.
int ms_prompt_command(t_ms_symbol *symbol, t_token *token)
{
	int				init_size;
	char			*line;
	t_token_info	info;
	t_ms_context	*context;

	if (token->string)
		return (0);
	context = (t_ms_context *)symbol->context;
	ms_token_info(&info, RESERVED_SINGLE, RESERVED_DOUBLE, RESERVED_SKIP);
	g_global_status = MS_SUBPROMPT_INT;
	line = readline("> ");
	//todo: add the command to history
	if (ms_join_line(context, line, " ") < 0)
		return (-1);
	if (!line)
		return (add_eof_token(context));
	init_size = context->parse_data.tokens->size - 1;
	ms_tokeniser(&line, context->parse_data.tokens, &info);
	while (++init_size < context->parse_data.tokens->size)
	{
		if (add_earley_set(context->parse_data.earley_sets, 5) < 0)
			return (-1);
		if (add_adjacency_list(context->parse_data.chart, sizeof(t_ms_edge), 5) < 0)
			return (-1);
	}
	free(line);
	return (1);
}

// only works for reserved symbols (IS_RESERVED flag is the expansion check)
int ms_match_equal(t_ms_symbol *symbol, t_token *token)
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
