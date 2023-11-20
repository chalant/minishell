#include "minishell.h"

int	init_command(t_command *command)
{
	command->command_flags = 0;
	command->left = NULL;
	command->right = NULL;
	command->command_name = NULL;
	command->arguments = NULL;
	command->redirections = NULL;
	command->input = NULL;
	command->output = NULL;
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

int	create_files(t_command *command, t_darray *redirections)
{
	int				i;
	int				last_in;
	int				last_out;
	t_redirection	*redirection;
	int				fd;

	last_in = -1;
	last_out = -1;
	i = -1;
	//todo: set proper permissions for file.
	while (++i < redirections->size)
	{
		redirection = ft_darray_get(redirections, i);
		if (!(redirection->redirection_flags & MS_HEREDOC) && !(redirection->redirection_flags & MS_READ))
		{
			//todo: handle errors
			fd = open(redirection->file_path, redirection->file_flags, 0666);
			close(fd);
		}
		if (redirection->redirection_flags & MS_READ)
			last_in = i;
		if (redirection->redirection_flags & MS_WRITE)
			last_out = i;
	}
	//todo: store redirections as file descriptors instead.
	if (last_in != -1)
	{
		redirection = ft_darray_get(redirections, last_in);
		command->input = malloc(sizeof(t_redirection));
		ft_memcpy(command->input, redirection, sizeof(t_redirection));
	}
	if (last_out != -1)
	{
		redirection = ft_darray_get(redirections, last_out);
		command->output = malloc(sizeof(t_redirection));
		ft_memcpy(command->output, redirection, sizeof(t_redirection));
	}
	return (1);
}

int	set_redirection(t_redirection *redirection, t_parse_tree *tree)
{
	t_parse_tree	*redir;

	//todo: need to set the right file permissions.
	redirection->redirection_flags = 0;
	redirection->file_flags = 0;
	redirection->file_path = NULL;
	redir = ft_darray_get(tree->children, 0);
	if (strcmp(redir->rule_name, ">") == 0)
	{
		redirection->file_flags = O_TRUNC | O_CREAT | O_APPEND | O_RDWR;
		redirection->redirection_flags |= MS_WRITE;
	}
	else if (strcmp(redir->rule_name, "<") == 0)
	{
		redirection->file_flags |= O_RDONLY;
		redirection->redirection_flags |= MS_READ;
	}
	else if (strcmp(redir->rule_name, "<<") == 0)
		redirection->redirection_flags |= MS_HEREDOC;
	else if (strcmp(redir->rule_name, ">>") == 0)
	{
		redirection->file_flags = O_CREAT | O_APPEND | O_RDWR;
		redirection->redirection_flags |= MS_WRITE;
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

int	create_simple_command_core(t_parse_tree *node, t_stack *stack, t_command *command)
{
	t_parse_tree	*element_type;

	if (!node->rule_name)
		return (0);
	if (strcmp(node->rule_name, "builtin") == 0)
		command->command_flags |= MS_BUILTIN;
	command->command_name = ft_strdup(*get_word(node));
	node = ft_darray_get(node->children, 1);
	if (node->children)
	{
		int i = -1;
		while (++i < node->children->size)
		{
			element_type = ft_darray_get(node->children, i);
			if (strcmp(element_type->rule_name, "command_argument") == 0)
				set_arguments(command, element_type);
			if (strcmp(element_type->rule_name, "redirection_list") == 0)
				set_redirections(command, element_type);
		}
	}
	if (command->redirections)
		create_files(command, command->redirections);
	command->command_flags |= MS_OPERAND;
	return (ft_stack_push(stack, command));
}

//todo:add redirections for operands!-> it will pass redirections
//to each member, disabling the truncation mode after opening the file.
int	create_simple_command(t_parse_tree *node, t_stack *stack)
{
	t_command		command;
	t_parse_tree	*element_type;

	init_command(&command);
	//todo: no need to create redirections if there aren't any!
	command.redirections = malloc(sizeof(t_darray));
	if (!command.redirections)
		return (-1);
	//todo:always store the last redirections.
	if (ft_darray_init(command.redirections, sizeof(t_redirection), 10) < 0)
		return (-1);
	return (create_simple_command_core(node, stack, &command));
}

//todo: this also builds a simple command.
int	create_redirection_command(t_parse_tree *node, t_stack *stack)
{
	t_command		command;
	t_parse_tree	*element_type;

	init_command(&command);
	command.redirections = malloc(sizeof(t_darray));
	if (!command.redirections)
		return (-1);
	//todo:always store the last redirections.
	if (ft_darray_init(command.redirections, sizeof(t_redirection), 10) < 0)
		return (-1);
	set_redirections(&command, ft_darray_get(node->children, 0));
	printf("Simple command %d\n", node->children->size);
	if (node->children->size >= 2)
	{
		printf("Simple command !\n");
		return (create_simple_command_core(ft_darray_get(node->children, 1), stack, &command));
	}
	create_files(&command, command.redirections);
	return (ft_stack_push(stack, &command));
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
	if (command->redirections)
	{
		//todo:distribute inputs as-well
		if (!left->output && !(command->command_flags & MS_PIPE))
			left->output = command->output;
		if (!right->output)
			right->output = command->output;
	}
	command->left = left;
	command->right = right;
	return (1);
}

int	create_operator(t_parse_tree *node, t_stack *stack, int type, const char *name)
{
	t_command	command;

	flatten_tree((t_parse_tree *)ft_darray_get(node->children, 2), stack);
	init_command(&command);
	command.command_name = ft_strdup(name);
	command.command_flags |= MS_OPERATOR;
	command.command_flags |= type;
	return (ft_stack_push(stack, &command));
}

int	handle_parenthesis(t_parse_tree *node, t_command *command)
{
	if (node->children->size == 4)
	{
		set_redirections(command, ft_darray_get(node->children, 3));
		if (!(command->command_flags & MS_PIPE))
			command->output->file_flags &= ~O_TRUNC;
	}
	return (1);
}

int	flatten_tree(t_parse_tree *node, t_stack *commands)
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
		if (strcmp(node->rule_name, "parenthesis") == 0)
			handle_parenthesis(node, ft_stack_peek(commands));
		//todo:set redirections if any exists.
		//todo: handle errors
		if (flatten_tree(child, commands) < 0)
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
	flatten_tree(node, &commands);

	command = (t_command *)ft_stack_pop(&commands);
	if (!command->command_name)
		return (0);
	if (command->command_flags & MS_OPERATOR)
		build_operator(command, &commands);
	return (command);
}
