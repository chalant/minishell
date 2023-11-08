#include "minishell.h"

//todo: this is temporary
char	*get_word(t_parse_tree *node)
{
	t_parse_tree	*tree;

	tree = (t_parse_tree *)ft_darray_get(node->children, 0);
	tree = (t_parse_tree *)ft_darray_get(tree->children, 0);
	return (tree->rule_name);
}

int	build_simple_command(t_parse_tree *node, t_darray *stack)
{
	t_command	command;

	//todo: also check if it is a builtin.
	command.command_flags = 0;
	command.command_name = get_word(node);
	//todo: set values of the union.
	// set_arguments(&command.arguments, node);
	// set_redirections(&command.redirections, node);
	command.command_flags |= MS_OPERAND;
	return (ft_darray_append(stack, &command));
}

//todo: this also builds a simple command.
int	build_redirection_command(t_parse_tree *node, t_darray *stack)
{
	(void)node;
	(void)stack;
	return (1);
}

int	create_commands(t_parse_tree *node, t_darray *result)
{
	int				i;
	t_parse_tree	*child;
	t_command		command;

	if (!node)
		return (1);
	if (!node->rule_name)
		return (1);
	if (node->terminal)
		return (1);
	if (!node->children)
		return (1);
	if (strcmp(node->rule_name, "simple_command") == 0)
		return (build_simple_command(node, result));
	else if (strcmp(node->rule_name, "redirection_command") == 0)
		return (build_redirection_command(node, result));
	command.command_flags = 0;
	i = -1;
	while (++i < node->children->size)
	{
		child = (t_parse_tree *)ft_darray_get(node->children, i);
		if (!child->rule_name)
			return (0);
		if (strcmp(child->rule_name, "|") == 0)
		{
			create_commands((t_parse_tree *)ft_darray_get(node->children, 2), result);
			command.command_name = ft_strdup("PIPE");
			command.command_flags |= MS_OPERATOR;
			command.command_flags |= MS_PIPE;
			return (ft_darray_append(result, &command));
		}
		else if (strcmp(child->rule_name, "&&") == 0)
		{
			create_commands((t_parse_tree *)ft_darray_get(node->children, 2), result);
			command.command_name = ft_strdup("AND");
			command.command_flags |= MS_OPERATOR;
			command.command_flags |= MS_AND;
			return (ft_darray_append(result, &command));
		}
		else if (strcmp(child->rule_name, "||") == 0)
		{
			create_commands((t_parse_tree *)ft_darray_get(node->children, 2), result);
			command.command_name = ft_strdup("OR");
			command.command_flags |= MS_OPERATOR;
			command.command_flags |= MS_OR;
			return (ft_darray_append(result, &command));
		}
		//todo: handle errors
		if (create_commands(child, result) < 0)
			return (-1);
	}
	return (1);
}
