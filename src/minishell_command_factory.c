/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_command_factory.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yves <yves@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:30 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/17 11:50:30 by yves             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_command_elements(t_command *command, t_parse_tree *tree)
{
	t_redirection	redirection;
	t_parse_tree	*node;

	while (!tree->terminal)
	{
		node = ft_darray_get(tree->children, 0);
		if (strcmp(node->rule_name, "redirection") == 0)
		{
			set_redirection(&redirection, node);
			if (ft_darray_append(command->redirections, &redirection) < 0)
				return (-1);
		}
		else if (strcmp(node->rule_name, "word") == 0
			&& ft_darray_append(command->arguments, get_leaf(node)->token) < 0)
			return (-1);
		tree = ft_darray_get(tree->children, 1);
	}
	return (1);
}

int	set_command_fields(t_parse_tree *node, t_command *command)
{
	if (!node->rule_name)
		return (0);
	//todo: need an error when the command is not found.
	command->command_name = *get_word(node);
	if (is_builtin(command->command_name))
		command->command_flags |= MS_BUILTIN;
	node = ft_darray_get(node->children, 1);
	if (node->children && set_command_elements(command, node) < 0)
		return (-1);
	if (!command->redirections)
		return (-1);
	//create_files(command, command->redirections);
	command->command_flags |= MS_OPERAND;
	return (0);
}

//todo: free stuff here in case of failed mallocs?
int	init_command_fields(t_command *command)
{
	if (!command->redirections)
	{
		command->redirections = malloc(sizeof(t_darray));
		if (!command->redirections)
			return (-1);
		if (ft_darray_init(command->redirections, sizeof(t_redirection), 3) < 0)
			return (-1);
	}
	if (!command->arguments)
		command->arguments = malloc(sizeof(t_darray));
	if (!command->arguments)
		return (-1);
	if (ft_darray_init(command->arguments, sizeof(t_token), 3) < 0)
		return (-1);
	return (0);
}

int	overwite_command(t_parse_tree *node, t_command *command)
{
	init_command_fields(command);
	if (set_command_fields(node, command) < 0)
		return (-1);
	//todo: check if it is not an operator
	if (command->command_flags & MS_OPERATOR)
	{
		if (command->input > 0)
			close(command->input);
		if (command->output > 0)
			close(command->output);
		command->input = 0;
		command->output = 0;
	}
	command->command_flags &= ~(MS_REDIR);
	return (1);
}

int	create_command(t_parse_tree *node, t_stack *stack)
{
	t_command	*command;
	t_command	new;

	command = ft_stack_peek(stack);
	if (command && command->command_flags & MS_REDIR)
		return (overwite_command(node, command));
	command = ft_darray_get(stack->elements, stack->elements->size);
	if (!command || !command->command_name)
	{
		init_command(&new);
		init_command_fields(&new);
		if (set_command_fields(node, &new) < 0)
		{
			clear_command(&new);
			return (-1);
		}
		if (ft_stack_push(stack, &new) < 0)
			return (-1); 
	}
	return (0);
}
