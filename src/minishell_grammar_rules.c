#include "minishell.h"

//todo: check for leaks
int	init_rule(char ***result, t_ms_grammar *grammar, char **rules, int i)
{
	char		**rule;
	char		**symbols;

	rule = ft_split(rules[i], ':');
	if (!rule)
		return (-1);
	result[0] = rule;
	grammar->rules[i] = malloc(sizeof(t_ms_rule));
	if (!grammar->rules[i])
		return (-1);
	grammar->rules[i]->name = rule[0];
	if (!grammar->rules[i]->name)
		return (-1);
	symbols = ft_split(rule[1], ' ');
	free(rule[1]);
	if (!symbols)
		return (-1);
	result[1] = symbols;
	grammar->rules[i]->symbols = malloc((ft_count_strings(symbols) + 1) * sizeof(t_ms_symbol *));
	if (!grammar->rules[i]->symbols)
		return (-1);
	return (1);
}

int	free_all(char ***to_free, int size)
{
	int	i;

	i = -1;
	while (++i < size)
		free(to_free[i]);
	free(to_free);
	return (0);
}

int	tail_rule(t_ms_symbol **symbols, char ***to_free, int size)
{
	t_ms_symbol	*symbol;

	symbol = malloc(sizeof(t_ms_symbol));
	if (!symbol)
		return (-1);
	symbol->name = ft_strdup("nil");
	if (!symbol->name)
		return (-1);
	symbol->symbol_type = MS_NULL_SYMBOL;
	symbols[size] = symbol;
	free_all(to_free, 2);
	return (1);
}

int	init_symbol(t_ms_symbol *symbol)
{
	symbol->earley_sets = NULL;
	symbol->match = NULL;
	symbol->name = NULL;
	symbol->rule = 0;
	symbol->symbol_type = 0;
	symbol->tokens = NULL;
	return (1);
}

int	add_symbol(t_ms_symbol **dest, char *definition, int j)
{
	t_ms_symbol	*symbol;

	symbol = malloc(sizeof(t_ms_symbol));
	if (!symbol)
		return (-1);
	init_symbol(symbol);
	dest[j] = symbol;
	if (ft_strchr(definition, '\''))
		return (handle_terminal_symbol(symbol, definition));
	symbol->name = definition;
	symbol->symbol_type = MS_NON_TERMINAL_SYMBOL;
	return (1);
}

int	add_rule(t_ms_grammar *grammar, char **rules, int i)
{
	int			j;
	char		**symbols;
	char		***to_free;

	to_free = ft_calloc(2, sizeof(char **));
	if (!to_free)
		return (-1);
	if (init_rule(to_free, grammar, rules, i) < 0)
		return (free_all(to_free, 2) - 1);
	j = 0;
	symbols = to_free[1];
	while (symbols[j])
	{
		if (add_symbol(grammar->rules[i]->symbols, symbols[j], j) < 0) 
			return (free_all(to_free, 2) - 1);
		j++;
		grammar->rules[i]->length = j;
	}
	if (tail_rule(grammar->rules[i]->symbols, to_free, j) < 0)
		return (free_all(to_free, 2) - 1);
	grammar->rules[i]->length = j + 1;
	return (1);
}
