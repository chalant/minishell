/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_semantics.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:41 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/28 16:06:32 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_operator(t_parse_tree *node, t_stack *stack, int type, const char *name)
{
	t_command	command;

	//todo:handle errors
	flatten_tree((t_parse_tree *)ft_darray_get(node->children, 2), stack);
	init_command(&command);
	command.command_name = ft_strdup(name);
	command.command_flags = MS_OPERATOR | type;
	return (ft_stack_push(stack, &command));
}

int	handle_parenthesis(t_parse_tree *node, t_command *command)
{
	if (!command || !command->command_name)
		return (0);
	if (node->children->size >= 4 && ((t_parse_tree *)ft_darray_get(node->children, 3))->rule_name)
	{
		if (!command->redirections)
		{
			command->redirections = malloc(sizeof(t_darray));
			if (!command->redirections)
				return (-1);
			if (ft_darray_init(command->redirections, sizeof(t_redirection), 10) < 0)
				return (-1);
			set_redirections(command, ft_darray_get(node->children, 3));
			create_files(command, command->redirections);
		}
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
		if (strcmp(node->rule_name, "parenthesis") == 0)
			handle_parenthesis(node, ft_stack_peek(commands));
		if (flatten_tree(child, commands) < 0)
			return (-1);
	}
	return (1);
}

int	flatten_tree(t_parse_tree *node, t_stack *commands)
{
	if (!node || !node->rule_name || node->terminal || !node->children)
		return (1);
	if (strcmp(node->rule_name, "simple_command") == 0)
		return (create_simple_command(node, commands));
	else if (strcmp(node->rule_name, "redirection_command") == 0)
		return (create_redirection_command(node, commands));
	return (handle_semantic_rule(node, commands));
}

t_command	*build_command(t_darray	*command_array, t_parse_tree *node)
{
	t_stack		commands;
	t_command	*command;

	ft_stack_init(&commands, command_array);
	flatten_tree(node, &commands);
	command = (t_command *)ft_stack_pop(&commands);
	if (!command->command_name)
		return (0);
	if (command->command_flags & MS_OPERATOR)
		build_operator(command, &commands);
	return (command);
}
