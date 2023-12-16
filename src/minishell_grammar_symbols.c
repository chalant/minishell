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

static int	ms_strclen(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

static char	*ms_strcdup(const char *src, char c)
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
	else
		handle_special(symbol, input);
	free(input);
	return (1);
}
