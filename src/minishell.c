/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:53 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/27 20:39:35 by bvercaem         ###   ########.fr       */
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

int	init_parsing_data(t_parsing_data *data)
{
	data->tokens = malloc(sizeof(t_darray));
	if (!data->tokens)
		return (-1);
	data->chart = malloc(sizeof(t_graph));
	if (!data->chart)
	{
		free(data->tokens);
		return (-1);
	}
	data->grammar = malloc(sizeof(t_ms_grammar));
	if (!data->grammar)
	{
		free(data->tokens);
		free(data->chart);
		return (-1);
	}
	return (1);
}

int	parse_input(t_parsing_data *data, t_parse_tree *tree)
{
	tree->start = 0;
	tree->rule_name = (char *)data->grammar->start_rule;
	tree->end = data->input_length - 1;
	tree->terminal = 0;
	tree->children = NULL;
	ms_start_rule(tree, data);
	if (build_parse_tree(tree, data))
		return (-1);
	//todo: remove this since it is for debugging.
	print_parse_tree(tree, 0);
	return (0);
}

int	tokenize_input(t_parsing_data *data, char *input)
{
	t_token_info		info;

	if (ft_darray_init(data->tokens, sizeof(t_token), 20) == -1)
		return (-1);
	info.reserved_double = RESERVED_DOUBLE;
	info.reserved_single = RESERVED_SINGLE;
	info.reserved_skip = RESERVED_SKIP;
	if (ms_tokeniser(&input, data->tokens, &info) > 1)
	{
		//todo: free tokens
		return (-1);
	}
	return (0);
}

int	recognize_input(t_parsing_data *data)
{
	int				i;
	t_darray		*sets;

	sets = malloc(sizeof(t_darray));
	if (!sets)
		return (-1);
	if (ft_darray_init(sets, sizeof(t_darray), data->tokens->size) < 0)
		return (-1);
	i = -1;
	while (++i < data->tokens->size)
		add_earley_set(sets, data->tokens->size);
	if (build_earley_items(sets, data->grammar, data->tokens) < 0)
		return (-1);
	//todo: remove the reverse_earley since it is for debugging
	reverse_earley(sets, data->grammar);
	//todo: interrupt the program if we haven't reached the last
	//state and print where the error occured
	if (build_chart(sets, data->chart, data->tokens->size) < 0)
		return (-1);
	data->input_length = data->tokens->size;
	data->chart_size = data->tokens->size;
	//todo: free sets.
	return (0);
}

int	execute(t_parse_tree *tree)
{
	t_darray	command_array;
	t_command	*command;

	if (ft_darray_init(&command_array, sizeof(t_command), 10) < 0)
		return (1);
	command = build_command(&command_array, tree);
	if (!command)
	{
		//todo: free command_array.
		return (1);
	}
	return (minishell_execute(command));
}

// int	free_on_error(t_parsing_data *data, t_parse_tree *tree)
// {
// 	return (1);
// }

// int	free_on_success(t_parsing_data *data, t_parse_tree *tree, int status)
// {
	
// 	return (1);
// }

int	main(int ac, char **av, char **env)
{
	int					status;
	t_parsing_data		data;
	t_parse_tree		tree;

	(void)ac;
	(void)av;
	(void)env;
	if (ac != 2)
		return (1);
	printf("INPUT: \"%s\"\n", av[1]);
	if (init_parsing_data(&data) < 0)
		return (1);
	//set the grammar only once.
	set_minishell_grammar(data.grammar);
	// this should be ran at each loop.
	tokenize_input(&data, av[1]);
	//print_grammar(&grammar);
	recognize_input(&data);
	parse_input(&data, &tree);
	status = execute(&tree);
	//todo: free data and tree; -> note: no need to free the grammar at each loop.
	return (status);
}
