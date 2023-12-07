/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_command_factory.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:30 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/07 13:45:44 by ychalant         ###   ########.fr       */
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
	command->context = NULL;
	command->token = NULL;
	return (1);
}

int	set_arguments(t_command *command, t_parse_tree *tree)
{
	t_parse_tree	*node;

	command->arguments = malloc(sizeof(t_darray));
	if (!command->arguments)
		return (-1);
	if (ft_darray_init(command->arguments, sizeof(t_token), 3) < 0)
		return (-1);
	if (ft_darray_append(command->arguments, get_leaf(tree)->token) < 0)
		return (-1);
	node = ft_darray_get(tree->children, 1);
	while (!node->terminal)
	{
		if (ft_darray_append(command->arguments, get_leaf(node)->token) < 0)
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

int	is_builtin(char *command_name)
{
	//todo: add more builtins
	if (strcmp(command_name, "cd") == 0)
		return (1);
	else if (strcmp(command_name, "echo") == 0)
		return (1);
	else if (strcmp(command_name, "pwd") == 0)
		return (1);
	else if (strcmp(command_name, "export") == 0)
		return (1);
	else if (strcmp(command_name, "env") == 0)
		return (1);
	else if (strcmp(command_name, "unset") == 0)
		return (1);
	else if (strcmp(command_name, "exit") == 0)
		return (1);
	return (0);
}

int	set_command_fields(t_parse_tree *node, t_command *command)
{
	char	*command_name;

	if (!node->rule_name)
		return (0);
	//todo: need an error when the command is not found.
	command_name = *get_word(node);
	if (is_builtin(command_name))
	{
		command->command_flags |= MS_BUILTIN;
		command->command_name = ft_strdup(command_name);
	}
	else
		command->command_name = get_binary(command_name);
	if (!command->command_name)
		return (-1);
	node = ft_darray_get(node->children, 1);
	if (node->children && set_command_elements(command, node) < 0)
			return (-1);
	if (command->redirections && create_files(command, command->redirections) < 0)
			return (-1);
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

//todo: need to handle the parenthesis and the redirections
//after the parenthesis -> children >= 5
int	redirection_command(t_parse_tree *node, t_command *command)
{
	t_parse_tree	*tmp;

	if (set_redirections(command, ft_darray_get(node->children, 0)) < 0)
		return (-1);
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
		if (factory(node, &new) < 0)
			return (-1);
		return (ft_stack_push(stack, &new));
	}
	if (factory(node, command) < 0)
		return (-1);
	return (ft_stack_push(stack, command));
}
