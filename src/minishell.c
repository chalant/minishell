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
	t_ms_edge		*longest;

	end = 0;
	edge = ((t_ms_edge *)data->chart->adjacency_list[tree->start]->contents + end);
	longest = edge;
	while (++end < data->chart->adjacency_list[tree->start]->size)
	{
		edge = ((t_ms_edge *)data->chart->adjacency_list[tree->start]->contents + end);
		if (edge->finish > longest->finish)
			longest = edge;
		//printf("Starting rule: %d %d\n", longest->finish, end);
	}
	tree->rule_name = (char *)data->grammar->rules[longest->rule]->name;
	tree->end = longest->finish;
	//printf("Starting rule: %s %d\n", tree->rule_name, longest->finish);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	int		i = -1;
	int		size;
	char	**grammar_definition;

	t_darray	tokens;

	if (ac != 2)
		return (1);
	printf("INPUT: \"%s\"\n", av[1]);
	if (ft_darray_init(&tokens, sizeof(t_token), 20) == -1)
		return (1);
	ms_tokeniser(av[1], &tokens);
	ms_print_tokens(&tokens);
	//ft_darray_delete(&tokens, ms_clear_token);
	
	size = tokens.size;
	t_earley_set	**sets = malloc(sizeof(t_earley_set *) * (tokens.size));
	t_earley_set	**reversed = malloc(sizeof(t_earley_set *) * (tokens.size));
	t_ms_grammar	grammar;
	t_graph			graph;
	t_parse_tree	tree;
	t_parsing_data	data;
	
	grammar_definition = get_minishell_definition();
	set_grammar(&grammar, grammar_definition);
	print_grammar(&grammar);
	printf("\n");
	while (++i < tokens.size)
	{
		sets[i] = malloc(sizeof(t_earley_set));
		reversed[i] = malloc(sizeof(t_earley_set));
		sets[i]->items = malloc(sizeof(t_darray));
		reversed[i]->items = malloc(sizeof(t_darray));
		ft_darray_init(sets[i]->items, sizeof(t_earley_item), size);
		ft_darray_init(reversed[i]->items, sizeof(t_earley_item), size);
	}
	build_earley_items(sets, &grammar, size, &tokens);
	//todo: reverse earley items.
	print_earley(sets, &grammar, size);
	build_chart(sets, &graph, size);
	reverse_earley(sets, reversed, size);
	print_earley(reversed, &grammar, size);
	data.chart = &graph;
	data.grammar = &grammar;
	data.tokens = &tokens;
	data.input_length = tokens.size;
	data.chart_size = tokens.size;
	tree.start = 0;
	tree.rule_name = (char *)data.grammar->start_rule;
	// //tree.end = data.chart_size - 1;
	tree.end = data.input_length - 1;
	ms_start_rule(&tree, &data);
	tree.terminal = 0;
	ms_build_parse_tree(&tree, &data);
	print_parse_tree(&tree, 0);
	printf("\n");

	return (0);
}
