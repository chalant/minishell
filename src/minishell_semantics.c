#include "minishell.h"

int	init_command(t_command *command)
{
	command->command_flags = 0;
	command->left = NULL;
	command->right = NULL;
	command->command_name = NULL;
	command->arguments = NULL;
	command->redirections = NULL;
	command->forked = 0;
	return (1);
}

char	**get_word(t_parse_tree *node)
{
	t_parse_tree	*tree;

	tree = ft_darray_get(node->children, 0);
	while (tree->children)
		tree = ft_darray_get(tree->children, 0);
	//todo: set the rule name to NULL and free the string from the command?
	return (&tree->rule_name);
}

int	set_arguments(t_command *command, t_parse_tree *tree)
{
	t_parse_tree	*node;

	command->arguments = malloc(sizeof(t_darray));
	if (!command->arguments)
		return (-1);
	if (ft_darray_init(command->arguments, sizeof(char *), 3) < 0)
		return (-1);
	if (ft_darray_append(command->arguments, get_word(tree)) < 0)
		return (-1);
	node = ft_darray_get(tree->children, 1);
	while (node->children)
	{
		if (ft_darray_append(command->arguments, get_word(node)) < 0)
			return (-1);
		node = ft_darray_get(node->children, 1);
	}
	return (1);
}

int	set_redirection(t_redirection *redirection, t_parse_tree *tree)
{
	t_parse_tree	*redir;

	redirection->redirection_flags = 0;
	redirection->file_path = NULL;
	redir = ft_darray_get(tree->children, 0);
	redirection->redirection_flags |= MS_TRUNC;
	if (strcmp(redir->rule_name, ">") == 0)
		redirection->redirection_flags |= MS_WRITE;
	else if (strcmp(redir->rule_name, "<") == 0)
		redirection->redirection_flags |= MS_READ;
	else if (strcmp(redir->rule_name, "<<") == 0)
		redirection->redirection_flags |= MS_HEREDOC;
	else if (strcmp(redir->rule_name, ">>") == 0)
	{
		redirection->redirection_flags |= MS_APPEND;
		redirection->redirection_flags &= ~MS_TRUNC;
	}
	else
		return (-1);
	redirection->file_path = *(char **)get_word(ft_darray_get(tree->children, 1));
	return (0);
}

int	set_redirections(t_command *command, t_parse_tree *tree)
{
	t_parse_tree	*node;
	t_redirection	redirection;

	command->redirections = malloc(sizeof(t_darray));
	if (!command->redirections)
		return (-1);
	if (ft_darray_init(command->redirections, sizeof(t_redirection), 3) < 0)
		return (-1);
	set_redirection(&redirection, ft_darray_get(tree->children, 0));
	if (ft_darray_append(command->redirections, &redirection) < 0)
		return (-1);
	node = ft_darray_get(tree->children, 1);
	while (node->children)
	{
		set_redirection(&redirection, ft_darray_get(node->children, 0));
		if (ft_darray_append(command->redirections, &redirection) < 0)
			return (-1);
		node = ft_darray_get(node->children, 1);
	}
	return (0);
}

int	create_simple_command(t_parse_tree *node, t_stack *stack)
{
	t_command	command;

	init_command(&command);
	if (strcmp(node->rule_name, "builtin") == 0)
		command.command_flags |= MS_BUILTIN;
	command.command_name = ft_strdup(*get_word(node));
	node = ft_darray_get(node->children, 1);
	if (node->children)
	{
		set_arguments(&command, ft_darray_get(node->children, 0));
		if (node->children->size > 1)
			set_redirections(&command, ft_darray_get(node->children, 1));
	}
	command.command_flags |= MS_OPERAND;
	return (ft_stack_push(stack, &command));
}

//todo: this also builds a simple command.
int	create_redirection_command(t_parse_tree *node, t_stack *stack)
{
	//todo: this also builds a simple command.
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
