/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_grammar.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:44 by ychalant          #+#    #+#             */
/*   Updated: 2023/10/23 13:50:05 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static int	count_split(char **input)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	return (i);
}

static const char	*g_minishell_grammar = "sum:sum <'+-' product\n"\
	"sum:product\n"\
	"product:product <'*/' factor\n"\
	"product:factor\n"\
	"factor:='(' sum =')'\n"\
	"factor:number\n"\
	"number:<'0123456789' number\n"\
	"number:<'0123456789'\n";

int ms_match_subset(t_ms_symbol *symbol, char *input)
{
	int	i;

	i = 0;
	if (!input)
		return (0);
	while (input[i])
	{
		if (!ft_strchr(symbol->name, input[i]))
			return (0);
		i++;
	}
	return (1);
}

int ms_match_equal(t_ms_symbol *symbol, char *input)
{
	size_t	len;

	if (!input)
		return (0);
	len = ft_strlen(symbol->name);
	if (ft_strlen(symbol->name) != len)
		return (0);
	if (ft_strnstr(symbol->name, input, len))
		return (1);
	return (0);
}

int	ms_match_integer(t_ms_symbol *symbol, char *input)
{
	(void)symbol;
	(void)input;
	return (0);
}

int	ms_match_string(t_ms_symbol *symbol, char *input)
{
	(void)symbol;
	(void)input;
	return (0);
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

int	handle_terminal_symbol(t_ms_symbol *symbol, char *input)
{
	char	*ptr;
	char	*name;

	name = ft_strchr(input, '\'');
	name++;
	symbol->name = ms_strcdup(name, '\'');
	symbol->symbol_type = MS_TERMINAL_SYMBOL;
	if (ft_strchr(input, '='))
		symbol->match = ms_match_equal;
	else if (ft_strchr(input, '<'))
		symbol->match = ms_match_subset;
	else
	{
		ptr = ft_strchr(input, '%');
		if (!ptr)
			return (0);
		ptr++;
		ptr++;
		if (*ptr == MS_NEW_LINE)
		{
			symbol->name = "\n";
			symbol->match = ms_match_equal;
		}
		else if (*ptr == MS_INTEGER)
		{
			symbol->name = "number";
			symbol->match = ms_match_integer;
		}
		else if (*ptr == MS_EQUAL)
		{
			symbol->name = "=";
			symbol->match = ms_match_equal;
		}
		else if (*ptr == MS_STRING)
		{
			symbol->name = "string";
			symbol->match = ms_match_string;
		}

	}
	return (1);
}

const char *get_test_definition(void)
{
	return (g_minishell_grammar);
}

int	add_rule(t_ms_grammar *grammar, char **rules, int i)
{
	int			j;
	char		**rule;
	char		**symbols;
	t_ms_symbol	*symbol;

	rule = ft_split(rules[i], ':');
	grammar->rules[i] = malloc(sizeof(t_ms_rule));
	grammar->rules[i]->name = rule[0];
	symbols = ft_split(rule[1], ' ');
	grammar->rules[i]->symbols = malloc((count_split(symbols) + 1) * sizeof(t_ms_symbol *));
	j = 0;
	// function loop for setting up the symbol.
	while (symbols[j])
	{
		symbol = malloc(sizeof(t_ms_symbol));
		symbol->name = NULL;
		if (!symbol)
			return (-1);
		grammar->rules[i]->symbols[j] = symbol;
		//todo: this is a terminal symbol
		if (ft_strchr(symbols[j], '\''))
			handle_terminal_symbol(symbol, symbols[j]);
		else
		{
			symbol->name = symbols[j];
			symbol->symbol_type = MS_NON_TERMINAL_SYMBOL;
		}
		j++;
	}
	symbol = malloc(sizeof(t_ms_symbol));
	symbol->name = "nil";
	symbol->symbol_type = MS_NULL_SYMBOL;
	grammar->rules[i]->symbols[j] = symbol;
	grammar->rules[i]->length = j + 1;
	return (1);
}

//todo use t_darray for splits, symbols and rules.
int	set_grammar(t_ms_grammar *grammar, const char *definition)
{
	int			i;
	char		**rules;
	// int			size;

	i = 0;
	rules = ft_split(definition, '\n');
	//size = count_split(rules);
	grammar->rules = malloc(count_split(rules) * sizeof(t_ms_rule *));
	//todo: set start rule of the grammar as-well
	while (rules[i])
	{
		add_rule(grammar, rules, i);
		i++;
	}
	grammar->start_rule = grammar->rules[0]->name;
	grammar->length = i;
	return (1);
}
