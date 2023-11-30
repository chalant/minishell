#include "minishell.h"

void reset_tree_node(void *node)
{
	t_parse_tree	*tree;

	tree = (t_parse_tree *)node;
	tree->rule_name = NULL;
	tree->terminal = 1;
	tree->start = 0;
	tree->end = 0;
}

int	clear_parse_tree(t_parse_tree *tree, int(*del_method)(t_darray *, void(*)(void *)), int del)
{
	int				i;
	int				size;
	t_parse_tree	*child;

	if (!tree->children)
		return (0);
	i = -1;
	while (++i < tree->children->size)
	{
		child = ft_darray_get(tree->children, i);
		clear_parse_tree(child, del_method, del);
	}
	if (tree->children)
	{
		size = tree->children->size;
		del_method(tree->children, reset_tree_node);
		if (!del)
			tree->children->size = size;
	}
	return (0);
}

