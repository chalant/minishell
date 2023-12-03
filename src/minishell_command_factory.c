/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_command_factory.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:30 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/01 19:31:14 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_command(t_command *command)
{
	command->command_flags = 0;
	command->input = 0;
	command->output = 0;
	command->left = NULL;
	command->right = NULL;
	command->command_name = NULL;
	command->arguments = NULL;
	command->redirections = NULL;
	return (1);
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
	while (!node->terminal)
	{
		if (ft_darray_append(command->arguments, get_word(node)) < 0)
			return (-1);
		node = ft_darray_get(node->children, 1);
	}
	return (1);
}

int	set_command_elements(t_command *command, t_parse_tree *tree)
{
	t_redirection	redirection;
	t_parse_tree	*node;

	while (!tree->terminal)
	{
		//todo: handle errors
		node = ft_darray_get(tree->children, 0);
		if (strcmp(node->rule_name, "redirection") == 0)
		{
			set_redirection(&redirection, node);
			ft_darray_append(command->redirections, &redirection);
		}
		else if (strcmp(node->rule_name, "word") == 0)
			ft_darray_append(command->arguments, get_word(node));
		tree = ft_darray_get(tree->children, 1);
	}
	return (1);
}

int	set_command_fields(t_parse_tree *node, t_command *command)
{
	if (!node->rule_name)
		return (0);
	//todo: need an error when the command is not found.
	command->command_name = get_command(*get_word(node));
	if (!command->command_name)
		return (-1);
	node = ft_darray_get(node->children, 1);
	if (node->children)
		//todo: errors
		set_command_elements(command, node);
	if (command->redirections)
		//todo: errors
		create_files(command, command->redirections);
	command->command_flags |= MS_OPERAND;
	return (0);
}

int	init_command_fields(t_command *command)
{
	init_command(command);
	command->redirections = malloc(sizeof(t_darray));
	if (!command->redirections)
		return (-1);
	if (ft_darray_init(command->redirections, sizeof(t_redirection), 3) < 0)
		return (-1);
	command->arguments = malloc(sizeof(t_darray));
	if (!command->arguments)
		return (-1);
	if (ft_darray_init(command->arguments, sizeof(char *), 3) < 0)
		return (-1);
	return (0);
}

int	redirection_command(t_parse_tree *node, t_command *command)
{
	t_parse_tree	*tmp;

	//todo: errors
	if (set_redirections(command, ft_darray_get(node->children, 0)) < 0)
		return (-1);
	//is it 3 or 2?
	if (node->children->size >= 2)
	{
		tmp = ft_darray_get(node->children, 1);
		if (tmp->rule_name)
			return (set_command_fields(tmp, command));
	}
	return (create_files(command, command->redirections));
}

int	create_command(t_parse_tree *node, t_stack *stack, int (*factory)(t_parse_tree *, t_command *))
{
	t_command	*command;
	t_command	new;

	command = ft_darray_get(stack->elements, stack->elements->size);
	if (!command || !command->redirections || !command->arguments)
	{
		init_command_fields(&new);
		//todo: errors
		if (factory(node, &new) < 0)
			return (-1);
		return (ft_stack_push(stack, &new));
	}
	//todo: errors
	if (factory(node, command) < 0)
		return (-1);
	return (ft_stack_push(stack, command));
}
