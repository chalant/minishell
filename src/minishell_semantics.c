/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_semantics.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:41 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 14:08:12 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_operator(t_parse_tree *node, t_stack *commands)
{
	int				i;
	t_parse_tree	*child;

	i = -1;
	while (++i < node->children->size)
	{
		child = (t_parse_tree *)ft_darray_get(node->children, i);
		if (!child->rule_name)
			return (1);
		if (ft_strcmp(child->rule_name, "|") == 0)
			return (create_operator(node, commands, MS_PIPE, "PIPE"));
		else if (ft_strcmp(child->rule_name, "&&") == 0)
			return (create_operator(node, commands, MS_AND, "AND"));
		else if (ft_strcmp(child->rule_name, "||") == 0)
			return (create_operator(node, commands, MS_OR, "OR"));
		if (flatten_tree(child, commands) < 0)
			return (-1);
	}
	return (1);
}

int	create_redirections(t_command *new, t_parse_tree *node)
{
	init_command(new);
	new->redirections = malloc(sizeof(t_darray));
	if (!new->redirections)
		return (-1);
	if (ft_darray_init(new->redirections, sizeof(t_redirection), 3) < 0)
	{
		free(new->redirections);
		new->redirections = NULL;
		return (-1);
	}
	return (set_redirections(new, node));
}

int	handle_redirection_list(t_parse_tree *node, t_stack *commands)
{
	t_command	*command;
	t_command	new;

	command = ft_stack_peek(commands);
	if (!command)
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
		if (ft_darray_init(command->redirections,
				sizeof(t_redirection), 10) < 0)
			return (-1);
	}
	return (set_redirections(command, node));
}

int	flatten_tree(t_parse_tree *node, t_stack *commands)
{
	if (node->terminal)
		return (1);
	else if (ft_strcmp(node->rule_name, "simple_command") == 0)
		return (create_command(node, commands));
	else if (ft_strcmp(node->rule_name, "redirection_list") == 0)
		return (handle_redirection_list(node, commands));
	return (handle_operator(node, commands));
}

//builds a command tree to be executed.
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
	command->command_flags |= MS_LAST;
	command->command_flags |= MS_FIRST;
	if (command->command_flags & MS_OPERATOR)
		build_operator(command, &commands);
	return (command);
}
