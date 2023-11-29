#include "minishell.h"

void reset_tree_node(void *node)
{
	t_parse_tree	*tree;

	tree = (t_parse_tree *)node;
	tree->rule_name = NULL;
	tree->terminal = 0;
	tree->start = 0;
	tree->end = 0;
}

int	clear_parse_tree(t_parse_tree *tree, int(*del_method)(t_darray *, void(*)(void *)))
{
	int				i;
	t_parse_tree	*child;

	if (!tree->children)
		return (0);
	i = -1;
	while (++i < tree->children->size)
	{
		child = ft_darray_get(tree->children, i);
		clear_parse_tree(child, del_method);
	}
	del_method(tree->children, reset_tree_node);
	return (0);
}

