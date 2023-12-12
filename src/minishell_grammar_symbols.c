#include "minishell.h"

static int	set_symbol(t_ms_symbol *symbol, const char *name, int (*match)(t_ms_symbol *, t_token *))
{
	free(symbol->name);
	symbol->name = ft_strdup(name);
	if (!symbol->name)
		return (-1);
	symbol->match = match;
	return (1);
}

int	ms_strclen(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

char *ms_strcdup(const char *src, char c)
{
	int		i;
	char	*dest;

	i = 0;
	dest = malloc(sizeof(char) * (ms_strclen(src, c) + 1));
	if (!dest)
		return (NULL);
	while (src[i] && src[i] != c)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

// launches a prompt in case if an incomplete command.
int	ms_prompt_command(t_ms_symbol *symbol, t_token *token)
{
	int				i;
	int				init_size;
	char			*line;
	t_token_info	info;
	t_token			tok;

	if (token->string)
		return (0);
	ms_token_info(&info,
		RESERVED_SINGLE, RESERVED_DOUBLE, RESERVED_SKIP);
	line = readline("> ");
	if (!line)
	{
		symbol->tokens->size -= 1;
		ms_init_token(&tok);
		tok.string = ft_strdup("end of file");
		tok.flags |= IS_RESERVED;
		tok.flags |= IS_EOF;
		//todo: errors
		ft_darray_append(symbol->tokens, &tok);
		add_earley_set(symbol->earley_sets, 1);
		return (0);
	}
	symbol->tokens->size -= 1;
	init_size = symbol->tokens->size;
	ms_tokeniser(&line, symbol->tokens, &info);
	i = -1;
	//todo: errors
	while (++i < symbol->tokens->size - init_size - 1)
	{
		add_earley_set(symbol->earley_sets, 5);
		add_adjacency_list(symbol->chart, sizeof(t_ms_edge), 5);
	}
	free(line);
	return (1);
}

int	handle_special(t_ms_symbol *symbol, char *input)
{
	char	*ptr;

	ptr = ft_strchr(input, '$');
	ptr++;
	ptr++;
	if (*ptr == MS_PROMPTING)
		set_symbol(symbol, "<PROMPT>", ms_prompt_command);
	else if (*ptr == MS_STRING)
		set_symbol(symbol, "<STRING>", ms_match_string);
	return (1);
}

int	handle_terminal_symbol(t_ms_symbol *symbol, char *input)
{
	char	*name;

	name = ft_strchr(input, '\'');
	name++;
	symbol->name = ms_strcdup(name, '\'');
	if (!symbol->name)
	{
		free(input);
		return (-1);
	}
	symbol->symbol_type = MS_TERMINAL_SYMBOL;
	if (ft_strchr(input, '='))
		symbol->match = ms_match_equal;
	else if (ft_strchr(input, '<'))
		symbol->match = ms_match_subset;
	else
		handle_special(symbol, input);
	free(input);
	return (1);
}
