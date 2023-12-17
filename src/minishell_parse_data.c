#include "minishell.h"

int	ft_darray_full_delete(t_darray *darray, void (*del_content)(void *))
{
	ft_darray_delete(darray, del_content);
	free(darray);
	return (0);
}

int	init_parse_data(t_parsing_data *data)
{
	data->chart = NULL;
	data->tokens = NULL;
	data->chart = NULL;
	data->grammar = NULL;
	data->earley_sets = NULL;
	return (0);
}

int	alloc_parse_data(t_parsing_data *data, int size)
{
	data->tokens = malloc(sizeof(t_darray));
	if (!data->tokens)
		return (-1);
	data->chart = malloc(sizeof(t_graph));
	if (!data->chart)
		return (-1);
	data->grammar = malloc(sizeof(t_ms_grammar));
	if (!data->grammar)
		return (-1);
	data->earley_sets = malloc(sizeof(t_darray));
	if (!data->earley_sets)
		return (-1);
	if (init_graph(data->chart, size) < 0)
		return (-1);
	if (ft_darray_init(data->earley_sets, sizeof(t_darray), size) < 0)
		return (-1);
	if (ft_darray_init(data->tokens, sizeof(t_token), size) < 0)
		return (-1);
	return (set_minishell_grammar(data->grammar));
}

int	reset_parse_data(t_parsing_data *data, t_parse_tree *tree)
{
	ft_darray_reset(data->tokens, ms_clear_token);
	clear_parse_tree(tree, ft_darray_reset, 0);
	ft_darray_reset(data->earley_sets, reset_earley_set);
	clear_graph(data->chart, ft_darray_reset);
	return (0);
}

int	free_parse_data(t_parsing_data *data, t_parse_tree *tree)
{
	delete_grammar(data->grammar);
	if (tree)
		clear_parse_tree(tree, ft_darray_full_delete, 1);
	ft_darray_delete(data->earley_sets, delete_earley_set);
	clear_graph(data->chart, ft_darray_delete);
	ft_darray_delete(data->tokens, ms_clear_token);
	free(data->chart->adjacency_list);
	free(data->tokens);
	free(data->grammar);
	free(data->chart);
	free(data->earley_sets);
	return (0);
}
