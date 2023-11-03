#include "minishell.h"

int	build_simple_command(t_parse_tree *node, t_parse_tree *result)
{

}

int	collapse_tree(t_parse_tree *node, t_parse_tree *parent)
{
	int	i;
	t_parse_tree *child;

	if (!node)
		return ;
	if (!node->rule_name)
		return ;
	i = -1;
	if (node->terminal)
	{
		//todo: check the parent
		return ;
	}
	if (!node->children)
		return ;
	while (++i < node->children->size)
	{
		child = (t_parse_tree *)node->children->contents + i;
		if (strcmp(child->rule_name, "simple_command"))
			build_simple_command(node, parent);
	}
}
