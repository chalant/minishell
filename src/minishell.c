/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:53 by ychalant          #+#    #+#             */
/*   Updated: 2023/10/23 13:50:06 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		reversed[i]->size = j;
	}
	return (0);
}

int	build_chart(t_earley_set **sets, t_graph *graph, int size)
{
	int				i;
	int				j;
	int				k;
	t_ms_edge		edge;
	t_earley_item	*item;

	if (init_graph(graph, size, sizeof(t_ms_edge)) < 0)
		return (-1);
	i = -1;
	k = 0;
	while (++i < size)
	{
		j = -1;
		while (++j < sets[i]->items->size)
		{
			item = (t_earley_item *)sets[i]->items->contents + j;
			if (item->completed)
			{
				edge.finish = i;
				edge.start = item->start;
				edge.rule = item->rule;
				add_edge(graph, edge.start, &edge);
				k++;
			}
		}
	}
	return (0);
}

void print_parse_tree(t_parse_tree *node, int depth)
{
	int	i;
	t_parse_tree *child;

	if (!node)
		return ;
	if (!node->rule_name)
		return ;
	i = -1;
	for (int i = 0; i < depth; i++)
		printf("   |");
	printf("%s\n", node->rule_name);
	if (node->terminal)
		return ;
	if (!node->children)
		return ;
	while (++i < node->children->size)
	{
		child = (t_parse_tree *)node->children->contents + i;
		print_parse_tree(child, depth + 1);
	}
}

int	count_split(char **input)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	return (i);
}

int	ms_start_rule(t_parse_tree *tree, t_parsing_data *data)
{
	int				end;
	t_ms_edge		*edge;
	//int				i;

	end = 0;
	edge = ((t_ms_edge *)data->chart->adjacency_list[tree->start]->contents + end);
	while (++end < data->chart->adjacency_list[tree->start]->size + 1)
	{
		//printf("YO! %d %d\n", edge->finish, tree->end);
		if (edge->finish == tree->end)
		{
			printf("Found! %s %d\n", data->grammar->rules[edge->rule]->name, tree->end);
			// i = -1;
			// printf("Decompose ");
			// while (++i < data->grammar->rules[edge->rule]->length)
			// 	printf("%s ", data->grammar->rules[edge->rule]->symbols[i]->name);
			// printf("\n");
			tree->rule_name = (char *)data->grammar->rules[edge->rule]->name;
			return (1);
		}
		edge = ((t_ms_edge *)data->chart->adjacency_list[tree->start]->contents + end);
	}
	tree->end = edge->finish;
	return (0);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	int	i = -1;
	int	size;
	char *chars = av[1];
	char **input = ft_split(chars, ' ');
	size = count_split(input) + 1;
	t_earley_set	**sets = malloc(sizeof(t_earley_set *) * (size + 1));
	t_ms_grammar	grammar;
	t_earley_set	**reversed = malloc(sizeof(t_earley_set *) * (size + 1));
	t_graph			graph;
	t_parse_tree	tree;
	t_parsing_data	data;
	set_test_grammar(&grammar);
	while (++i < size)
	{
		sets[i] = malloc(sizeof(t_earley_set));
		reversed[i] = malloc(sizeof(t_earley_set));
		sets[i]->items = malloc(sizeof(t_darray));
		reversed[i]->items = malloc(sizeof(t_darray));
		ft_darray_init(sets[i]->items, sizeof(t_earley_item), 20);
		ft_darray_init(reversed[i]->items, sizeof(t_earley_item), 20);
	}
	build_earley_items(sets, &grammar, size, input);
	//todo: reverse earley items.
	print_earley(sets, &grammar, size);
	build_chart(sets, &graph, size);
	reverse_earley(sets, reversed, size);
	print_earley(reversed, &grammar, size);
	data.chart = &graph;
	data.grammar = &grammar;
	data.input = input;
	data.input_length = size;
	data.chart_size = size;
	tree.start = 0;
	tree.rule_name = (char *)data.grammar->start_rule;
	//tree.end = data.chart_size - 1;
	ms_start_rule(&tree, &data);
	tree.end = data.input_length - 1;
	tree.terminal = 0;
	ms_build_parse_tree(&tree, &data);
	print_parse_tree(&tree, 0);
	printf("\n");

	// t_darray	tokens;

	// if (ac != 2)
	// 	return (1);
	// printf("INPUT: \"%s\"\n", av[1]);
	// if (ft_darray_init(&tokens, sizeof(t_token), 20) == -1)
	// 	return (1);
	// ms_tokeniser(av[1], &tokens);
	// ms_print_tokens(&tokens);
	// ft_darray_delete(&tokens, ms_clear_token);
	// return (0);

	return (0);
}
