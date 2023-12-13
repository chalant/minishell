/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_semantics.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:41 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/12 17:21:35 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_operator(t_parse_tree *node, t_stack *stack, int type, char *name)
{
	t_command	command;

	//todo:handle errors
	if (flatten_tree((t_parse_tree *)ft_darray_get(node->children, 2), stack) < 0)
		return (-1);
	init_command(&command);
	command.command_name = ft_strdup(name);
	if (!command.command_name)
		return (-1);
	command.command_flags = MS_OPERATOR | type;
	return (ft_stack_push(stack, &command));
}

//todo: this only retrieves stuff as arguments for the subshell
int	handle_parenthesis(t_parse_tree *node, t_command *command)
{
	if (!command || !command->command_name)
		return (0);
	//todo: checking the children isn't safe
	if (node->children->size >= 4 && ((t_parse_tree *)ft_darray_get(node->children, 3))->rule_name)
	{
		if (!command->redirections)
		{
			command->redirections = malloc(sizeof(t_darray));
			if (!command->redirections)
				return (-1);
			if (ft_darray_init(command->redirections, sizeof(t_redirection), 10) < 0)
				return (-1);
		}
		set_redirections(command, ft_darray_get(node->children, 3));
		create_files(command, command->redirections);
	}
	return (1);
}

int	handle_semantic_rule(t_parse_tree *node, t_stack *commands)
{
	int				i;
	t_parse_tree	*child;

	i = -1;
	while (++i < node->children->size)
	{
		child = (t_parse_tree *)ft_darray_get(node->children, i);
		if (!child->rule_name)
			return (1);
		if (strcmp(child->rule_name, "|") == 0)
			return (create_operator(node, commands, MS_PIPE, "PIPE"));
		else if (strcmp(child->rule_name, "&&") == 0)
			return (create_operator(node, commands, MS_AND, "AND"));
		else if (strcmp(child->rule_name, "||") == 0)
			return (create_operator(node, commands, MS_OR, "OR"));
		if (flatten_tree(child, commands) < 0)
			return (-1);
	}
	return (1);
}

//todo: free all on failure
int	create_redirections(t_command *new, t_parse_tree *node)
{
	init_command(new);
	new->redirections = malloc(sizeof(t_darray));
	//todo: need to free the redirections in case of failure
	if (!new->redirections)
		return (-1);
	if (ft_darray_init(new->redirections, sizeof(t_redirection), 3) < 0)
		return (-1);
	return (set_redirections(new, node));
}

int	handle_redirection_list(t_parse_tree *node, t_stack *commands)
{
	t_command	*command;
	t_command	new;

	command = ft_stack_peek(commands);
	if (!command || !command->command_name)
	{
		init_command(&new);
		if (create_redirections(&new, node) < 0)
			return (-1);
		new.command_flags |= MS_REDIR;
		ft_stack_push(commands, &new);
		return (1);
	}
	if (!command->redirections)
	{
		command->redirections = malloc(sizeof(t_darray));
		if (!command->redirections)
			return (-1);
		if (ft_darray_init(command->redirections, sizeof(t_redirection), 10) < 0)
			return (-1);
	}
	return (set_redirections(command, node));
}

int	flatten_tree(t_parse_tree *node, t_stack *commands)
{
	if (node->terminal)
		return (1);
	else if (strcmp(node->rule_name, "simple_command") == 0)
		return (create_command(node, commands, set_command_fields));
	else if (strcmp(node->rule_name, "redirection_list") == 0)
		return (handle_redirection_list(node, commands));
	return (handle_semantic_rule(node, commands));
}

//builds a command tree to be executed.
//todo: need a status?
t_command	*build_command(t_darray	*command_array, t_parse_tree *tree)
{
	t_stack		commands;
	t_command	*command;

	ft_stack_init(&commands, command_array);
	if (flatten_tree(tree, &commands) < 0)
		return (NULL);
	command = (t_command *)ft_stack_pop(&commands);
	if (!command)
		return (NULL);
	if (!command->command_name)
	{
		//todo: file creation might fail, so we would need a status...
		if (command->command_flags & MS_REDIR)
		{
			printf("creating files\n");
			create_files(command, command->redirections);
		}
		return (NULL);
	}
	//todo: could push back the command on top...
	command->command_flags |= MS_LAST;
	command->command_flags |= MS_FIRST;
	if (command->command_flags & MS_OPERATOR)
		build_operator(command, &commands);
	return (command);
}
