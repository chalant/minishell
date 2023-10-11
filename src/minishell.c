# include "minishell.h"

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

int	set_test_grammar(t_ms_grammar *grammar)
{
	int	i;

	i = -1;
	grammar->length = 8;
	grammar->rules = malloc(sizeof(t_ms_rule *) * 8);
	while (++i < 8)
		grammar->rules[i] = malloc(sizeof(t_ms_rule));
	grammar->start_rule = "sum";

	grammar->rules[0]->name = "sum";
	grammar->rules[0]->symbols = malloc(sizeof(t_ms_symbol *) * 4);
	grammar->rules[0]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[0]->symbols[0]->name = "sum";
	grammar->rules[0]->symbols[0]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[0]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[0]->symbols[1]->name = "+-";
	grammar->rules[0]->symbols[1]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[0]->symbols[1]->match = ms_match_subset;
	grammar->rules[0]->symbols[2] = malloc(sizeof(t_ms_symbol));
	grammar->rules[0]->symbols[2]->name = "product";
	grammar->rules[0]->symbols[2]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[0]->symbols[3] = malloc(sizeof(t_ms_symbol));
	grammar->rules[0]->symbols[3]->symbol_type = MS_NULL_SYMBOL;
	grammar->rules[0]->symbols[3]->name = "nil";
	grammar->rules[0]->length = 4;

	grammar->rules[1]->name = "sum";
	grammar->rules[1]->symbols = malloc(sizeof(t_ms_symbol *) * 2);
	grammar->rules[1]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[1]->symbols[0]->name = "product";
	grammar->rules[1]->symbols[0]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[1]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[1]->symbols[1]->symbol_type = MS_NULL_SYMBOL;
	grammar->rules[1]->symbols[1]->name = "nil";
	grammar->rules[1]->length = 2;

	grammar->rules[2]->name = "product";
	grammar->rules[2]->symbols = malloc(sizeof(t_ms_symbol *) * 4);
	grammar->rules[2]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[2]->symbols[0]->name = "product";
	grammar->rules[2]->symbols[0]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[2]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[2]->symbols[1]->name = "*/";
	grammar->rules[2]->symbols[1]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[2]->symbols[1]->match = ms_match_subset;
	grammar->rules[2]->symbols[2] = malloc(sizeof(t_ms_symbol));
	grammar->rules[2]->symbols[2]->name = "factor";
	grammar->rules[2]->symbols[2]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[2]->symbols[3] = malloc(sizeof(t_ms_symbol));
	grammar->rules[2]->symbols[3]->symbol_type = MS_NULL_SYMBOL;
	grammar->rules[2]->symbols[3]->name = "nil";
	grammar->rules[2]->length = 4;

	grammar->rules[3]->name = "product";
	grammar->rules[3]->symbols = malloc(sizeof(t_ms_symbol *) * 2);
	grammar->rules[3]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[3]->symbols[0]->name = "factor";
	grammar->rules[3]->symbols[0]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[3]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[3]->symbols[1]->symbol_type = MS_NULL_SYMBOL;
	grammar->rules[3]->symbols[1]->name = "nil";
	grammar->rules[3]->length = 2;

	grammar->rules[4]->name = "factor";
	grammar->rules[4]->symbols = malloc(sizeof(t_ms_symbol *) * 4);
	grammar->rules[4]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[4]->symbols[0]->name = "(";
	grammar->rules[4]->symbols[0]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[4]->symbols[0]->match = ms_match_subset;
	grammar->rules[4]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[4]->symbols[1]->name = "sum";
	grammar->rules[4]->symbols[1]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[4]->symbols[2] = malloc(sizeof(t_ms_symbol));
	grammar->rules[4]->symbols[2]->name = ")";
	grammar->rules[4]->symbols[2]->match = ms_match_subset;
	grammar->rules[4]->symbols[2]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[4]->symbols[3] = malloc(sizeof(t_ms_symbol));
	grammar->rules[4]->symbols[3]->symbol_type = MS_NULL_SYMBOL;
	grammar->rules[4]->symbols[3]->name = "nil";
	grammar->rules[4]->length = 4;

	grammar->rules[5]->name = "factor";
	grammar->rules[5]->symbols = malloc(sizeof(t_ms_symbol *) * 2);
	grammar->rules[5]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[5]->symbols[0]->name = "number";
	grammar->rules[5]->symbols[0]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[5]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[5]->symbols[1]->symbol_type = MS_NULL_SYMBOL;
	grammar->rules[5]->symbols[1]->name = "nil";
	grammar->rules[5]->length = 2;

	grammar->rules[6]->name = "number";
	grammar->rules[6]->symbols = malloc(sizeof(t_ms_symbol *) * 3);
	grammar->rules[6]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[6]->symbols[0]->name = "[0123456789]";
	grammar->rules[6]->symbols[0]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[6]->symbols[0]->match = ms_match_subset;
	grammar->rules[6]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[6]->symbols[1]->name = "number";
	grammar->rules[6]->symbols[1]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[6]->symbols[2] = malloc(sizeof(t_ms_symbol));
	grammar->rules[6]->symbols[2]->name = "nil";
	grammar->rules[6]->symbols[2]->symbol_type = MS_NULL_SYMBOL;
	grammar->rules[6]->length = 3;

	grammar->rules[7]->name = "number";
	grammar->rules[7]->symbols = malloc(sizeof(t_ms_symbol *) * 2);
	grammar->rules[7]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[7]->symbols[0]->name = "[0123456789]";
	grammar->rules[7]->symbols[0]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[7]->symbols[0]->match = ms_match_subset;
	grammar->rules[7]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[7]->symbols[1]->name =  "nil";
	grammar->rules[7]->symbols[1]->symbol_type =  MS_NULL_SYMBOL;
	grammar->rules[7]->length = 2;

	return (0);
}

int	print_earley(t_earley_set **sets, t_ms_grammar *grammar, int size)
{
	int				i;
	int				j;
	int				k;
	t_earley_item	*item;

	i = -1;
	while (++i < size)
	{
		j = -1;
		printf("=================(%d)==================\n\n", i);
		while (++j < sets[i]->items->size)
		{
			item = (t_earley_item *)sets[i]->items->contents + j;
			printf("%s -> ", grammar->rules[item->rule]->name);
			k = -1;
			while (++k < grammar->rules[item->rule]->length)
			{
				if (k == item->next)
					printf("• ");
				if (k != grammar->rules[item->rule]->length - 1)
					printf("%s ", grammar->rules[item->rule]->symbols[k]->name);
			}
			printf("(%d)\n", item->start);
		}
		printf("\n");
	}
	return (0);
}

int	reverse_earley(t_earley_set **sets, t_earley_set **reversed, int size)
{
	int				i;
	int				j;
	int				k;
	t_earley_item	*item;

	i = -1;
	while (++i < size)
	{
		j = -1;
		while (++j < sets[i]->items->size)
		{
			item = (t_earley_item *)sets[i]->items->contents + j;
			if (item->completed)
			{
				k = item->start;
				item->start = i;
				ft_darray_append(reversed[k]->items, item);
			}
		}
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	int	i = -1;
	const char *chars = "1 + ( 2 * 3 - 4 )";
	char **input = ft_split(chars, ' ');
	t_earley_set **sets = malloc(sizeof(t_earley_set *) * 10);
	t_ms_grammar	grammar;
	t_earley_set **reversed = malloc(sizeof(t_earley_set *) * 10);
	set_test_grammar(&grammar);
	while (++i < 10)
	{
		sets[i] = malloc(sizeof(t_earley_set));
		reversed[i] = malloc(sizeof(t_earley_set));
		sets[i]->items = malloc(sizeof(t_darray));
		reversed[i]->items = malloc(sizeof(t_darray));
		ft_darray_init(sets[i]->items, sizeof(t_earley_item), 20);
		ft_darray_init(reversed[i]->items, sizeof(t_earley_item), 20);
	}
	build_earley_items(sets, &grammar, 10, input);
	//todo: reverse earley items.
	//print_earley(sets, &grammar, 10);
	reverse_earley(sets, reversed, 10);
	print_earley(reversed, &grammar, 10);
	return (0);
}
