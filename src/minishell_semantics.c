#include "minishell.h"

int	init_command(t_command *command)
{
	command->command_flags = 0;
	command->left = NULL;
	command->right = NULL;
	command->command_name = NULL;
	command->arguments = NULL;
	return (1);
}

char	**get_word(t_parse_tree *node)
{
	t_parse_tree	*tree;

	tree = (t_parse_tree *)ft_darray_get(node->children, 0);
	while (tree->children)
		tree = (t_parse_tree *)ft_darray_get(tree->children, 0);
	//todo: set the rule name to NULL and free the string from the command?
	return (&tree->rule_name);
}

int	set_arguments(t_command *command, t_parse_tree *tree)
{
	//char			**args;
	//int				i;
	t_parse_tree	*node;
	//t_darray		*list;
	char			**lst;

	command->arguments = malloc(sizeof(t_darray));
	//list = command->arguments;
	if (ft_darray_init(command->arguments, sizeof(char *), 3) < 0)
		return (-1);
	if (ft_darray_append(command->arguments, get_word(tree)) < 0)
			return (-1);
	node = (t_parse_tree *)ft_darray_get(tree->children, 1);
	while (node->children)
	{
		if (ft_darray_append(command->arguments, get_word(node)) < 0)
			return (-1);
		node = (t_parse_tree *)ft_darray_get(node->children, 1);
	}
	lst = ft_darray_get(command->arguments, 1);
	//printf("HELLO %s\n", *lst);
	// args = malloc(sizeof(char *) * list.size + 2);
	// //todo: free all
	// args[0] = command->command_name;
	// i = 0;
	// while (++i < list.size + 1)
	// 	args[i] = *(char **)ft_darray_get(&list, i - 1);
	// args[i] = NULL;
	// command->arguments = args;
	// printf("Arguments: %d ", list.size);
	// i = 0;
	// while (command->arguments[i])
	// {
	// 	printf("%s ", command->arguments[i]);
	// 	i++;
	// }
	// printf("\n");
	//printf("%s\n", command->arguments[i + 1]);
	//we probably don't need to free this ?
	//ft_darray_delete(&list, NULL);
	return (1);
}

int	create_simple_command(t_parse_tree *node, t_stack *stack)
{
	t_command	command;

	init_command(&command);
	if (strcmp(node->rule_name, "builtin") == 0)
		command.command_flags |= MS_BUILTIN;
	command.command_name = ft_strdup(*get_word(node));
	//todo: set arguments, redirections...
	// if (ft_darray_exists(node->children, 1))
	// {
	node = ft_darray_get(node->children, 1);
	if (node->children)
	{
		set_arguments(&command, (t_parse_tree *)ft_darray_get(node->children, 0));
		// set_redirections(&command.redirections, node);
	}
	// }
	//arguments = (char *)ft_darray_get(command.arguments, 4);
	command.command_flags |= MS_OPERAND;
	return (ft_stack_push(stack, &command));
}

//todo: this also builds a simple command.
int	create_redirection_command(t_parse_tree *node, t_stack *stack)
{
	(void)node;
	(void)stack;
	return (1);
}

int	build_operator(t_command *command, t_stack *commands)
{
	t_command	*right;
	t_command	*left;

	right = (t_command *)ft_stack_pop(commands);
	if (right->command_flags & MS_OPERATOR)
		build_operator(right, commands);
	left = (t_command *)ft_stack_pop(commands);
	if (left->command_flags & MS_OPERATOR)
		build_operator(left, commands);
	command->left = left;
	command->right = right;
	return (1);
}

int	create_operator(t_parse_tree *node, t_stack *stack, int type, const char *name)
{
	t_command	command;

	collapse_tree((t_parse_tree *)ft_darray_get(node->children, 2), stack);
	init_command(&command);
	command.command_name = ft_strdup(name);
	command.command_flags |= MS_OPERATOR;
	command.command_flags |= type;
	return (ft_stack_push(stack, &command));
}

int	collapse_tree(t_parse_tree *node, t_stack *commands)
{
	int				i;
	t_parse_tree	*child;

	if (!node || !node->rule_name || node->terminal || !node->children)
		return (1);
	if (strcmp(node->rule_name, "simple_command") == 0)
		return (create_simple_command(node, commands));
	else if (strcmp(node->rule_name, "redirection_command") == 0)
		return (create_redirection_command(node, commands));
	i = -1;
	while (++i < node->children->size)
	{
		child = (t_parse_tree *)ft_darray_get(node->children, i);
		if (!child->rule_name)
			return (0);
		if (strcmp(child->rule_name, "|") == 0)
			return (create_operator(node, commands, MS_PIPE, "PIPE"));
		else if (strcmp(child->rule_name, "&&") == 0)
			return (create_operator(node, commands, MS_AND, "AND"));
		else if (strcmp(child->rule_name, "||") == 0)
			return (create_operator(node, commands, MS_OR, "OR"));
		//todo: handle errors
		if (collapse_tree(child, commands) < 0)
			return (-1);
	}
	return (1);
}

t_command	*build_command(t_darray	*command_array, t_parse_tree *node)
{
	t_stack		commands;
	t_command	*command;

	ft_darray_init(command_array, sizeof(t_command), 10);
	ft_stack_init(&commands, command_array);
	collapse_tree(node, &commands);

	command = (t_command *)ft_stack_pop(&commands);
	if (!command->command_name)
		return (0);
	if (command->command_flags & MS_OPERATOR)
		build_operator(command, &commands);
	return (command);
}
