/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:53 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/24 17:34:35 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	print_earley(t_earley_set **sets, t_ms_grammar *grammar, int size)
{
	int				i;
	int				j;
	int				k;
	t_earley_item	*item;
	t_earley_set	*set;

	i = -1;
	while (++i < size)
	{
		j = -1;
		set = sets[i];
		printf("=================(%d)==================\n\n", i);
		while (++j < set->items->size)
		{
			item = (t_earley_item *)set->items->contents + j;
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

int	reverse_earley(t_darray *sets, t_ms_grammar *grammar)
{
	int				i;
	int				j;
	int				k;
	t_earley_item	*item;
    t_earley_set    *set;
	t_earley_set	**reversed;

	i = -1;
	reversed = malloc(sizeof(t_earley_set *) * (sets->size));
	ft_bzero(reversed, sets->size);
	while (++i < sets->size)
	{
		reversed[i] = malloc(sizeof(t_earley_set));
		reversed[i]->items = malloc(sizeof(t_darray));
		ft_darray_init(reversed[i]->items, sizeof(t_earley_item), sets->size);
	}
    item = malloc(sizeof(t_earley_item));
	i = -1;
	while (++i < sets->size)
	{
		j = -1;
        set = ft_darray_get(sets, i);
		while (++j < set->items->size)
		{
            ft_memcpy(item, ft_darray_get(set->items, j), sizeof(t_earley_item));
			if (item->completed)
			{
				k = item->start;
				item->start = i;
				ft_darray_append(reversed[k]->items, item);
			}
		}
		reversed[i]->size = j;
	}
	print_earley(reversed, grammar, sets->size);
	return (0);
}

int	build_chart(t_darray *sets, t_graph *graph, int size)
{
	int				i;
	int				j;
	t_ms_edge		edge;
	t_earley_item	*item;
	t_earley_set	*set;

	if (init_graph(graph, size, sizeof(t_ms_edge)) < 0)
		return (-1);
	i = -1;
	while (++i < size)
	{
		j = -1;
		set = ft_darray_get(sets, i);
		while (++j < set->items->size)
		{
			item = ft_darray_get(set->items, j);
			if (item->completed)
			{
				edge.finish = i;
				edge.start = item->start;
				edge.rule = item->rule;
				add_edge(graph, edge.start, &edge);
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

int	ms_start_rule(t_parse_tree *tree, t_parsing_data *data)
{
	int				end;
	t_ms_edge		*edge;
	t_ms_edge		*longest;
	t_darray		*edges;

	end = 0;
	edges = get_edges(data->chart, tree->start);
	edge = (t_ms_edge *)edges->contents + end;
	longest = edge;
	while (++end < edges->size)
	{
		edge = ((t_ms_edge *)edges->contents + end);
		if (edge->finish >= longest->finish)
			longest = edge;
	}
	tree->rule_name = (char *)data->grammar->rules[longest->rule]->name;
	tree->end = longest->finish;
	return (0);
}

int	print_execution_stack(t_darray *stack)
{
	int	i;

	printf("[ ");
	i = -1;
	while (++i < stack->size - 1)
		printf("%s, ", ((t_command *)ft_darray_get(stack, i))->command_name);
	printf("%s ", ((t_command *)ft_darray_get(stack, i))->command_name);
	printf("]\n");
	return (1);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	int		i = -1;
	int					size;
	char				**grammar_definition;
	t_token_info		info;
	t_darray			tokens;
	t_darray			command_array;

	if (ac != 2)
		return (1);
	printf("INPUT: \"%s\"\n", av[1]);
	if (ft_darray_init(&tokens, sizeof(t_token), 20) == -1)
		return (1);
	info.reserved_double = RESERVED_DOUBLE;
	info.reserved_single = RESERVED_SINGLE;
	info.reserved_skip = RESERVED_SKIP;
	ms_tokeniser(av[1], &tokens, &info);

	size = tokens.size;
	//todo: store sets into dynamic array.
	t_darray		*sets = malloc(sizeof(t_darray));

	t_ms_grammar	grammar;
	t_graph			graph;
	t_parse_tree	tree;
	t_command		*command;
	//todo: this should store everything.
	t_parsing_data	data;

	grammar_definition = get_minishell_definition();
	set_grammar(&grammar, grammar_definition);
	//print_grammar(&grammar);
	//printf("\n");
	ft_darray_init(sets, sizeof(t_darray), size);
	while (++i < tokens.size)
		add_earley_set(sets, size);
	build_earley_items(sets, &grammar, &tokens);
	//print_earley(sets, &grammar, size);
	build_chart(sets, &graph, tokens.size);
	//todo: interrupt the program if we haven't reached the last
	//state.
	reverse_earley(sets, &grammar);
	//print_earley(reversed, &grammar, size);
	data.chart = &graph;
	data.grammar = &grammar;
	data.tokens = &tokens;
	data.input_length = tokens.size;
	data.chart_size = tokens.size;
	tree.start = 0;
	tree.rule_name = (char *)data.grammar->start_rule;
	tree.end = data.input_length - 1;
	tree.terminal = 0;
	tree.children = NULL;
	ms_start_rule(&tree, &data);
	build_parse_tree(&tree, &data);
	print_parse_tree(&tree, 0);
	command = build_command(&command_array, &tree);
	minishell_execute(command);
	return (0);
}
