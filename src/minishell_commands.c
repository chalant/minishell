/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:30 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/28 17:44:03 by ychalant         ###   ########.fr       */
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

static int	create_command(t_parse_tree *node, t_stack *stack,
	t_command *command)
{
	if (!node->rule_name)
		return (0);
	//printf("RULE NAME! %s\n", node->rule_name);
	if (strcmp(node->rule_name, "builtin") == 0)
		command->command_flags |= MS_BUILTIN;
	//todo: need an error when the command is not found.
	command->command_name = get_command(*get_word(node));
	node = ft_darray_get(node->children, 1);

	//todo: only create arguments if there is any.
	command->arguments = malloc(sizeof(t_darray));
	// command->redirections = malloc(sizeof(t_darray));
	if (!command->arguments)
		return (-1);
	if (ft_darray_init(command->arguments, sizeof(char *), 3) < 0)
		return (-1);
	if (node->children)
	{
		// errors ?
		set_command_elements(command, node);
	}
	if (command->redirections)
		create_files(command, command->redirections);
	command->command_flags |= MS_OPERAND;
	return (ft_stack_push(stack, command));
}

int	create_simple_command(t_parse_tree *node, t_stack *stack)
{
	t_command		command;

	init_command(&command);
	//todo: no need to create redirections if there aren't any!
	command.redirections = malloc(sizeof(t_darray));
	if (!command.redirections)
		return (-1);
	if (ft_darray_init(command.redirections, sizeof(t_redirection), 5) < 0)
		return (-1);
	return (create_command(node, stack, &command));
}

int	create_redirection_command(t_parse_tree *node, t_stack *stack)
{
	t_command		command;
	t_parse_tree	*nod;

	init_command(&command);
	command.redirections = malloc(sizeof(t_darray));
	if (!command.redirections)
		return (-1);
	if (ft_darray_init(command.redirections, sizeof(t_redirection), 5) < 0)
		return (-1);
	set_redirections(&command, ft_darray_get(node->children, 0));
	//todo: fix redirection commands
	//command must take the redirection.
	//must be able to creat redirections without commands
	//is it 3 or 2?
	if (node->children->size >= 2)
	{
		nod = ft_darray_get(node->children, 1);
		if (nod->rule_name)
			return (create_command(nod, stack, &command));
	}
	create_files(&command, command.redirections);
	return (ft_stack_push(stack, &command));
}
