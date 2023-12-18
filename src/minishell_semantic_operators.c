/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_semantic_operators.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:24 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 14:26:24 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*creates an operator and checks if there is a redirection on top of the stack*/
int	create_operator(t_parse_tree *node, t_stack *stack, int type, char *name)
{
	t_command	command;

	if (flatten_tree(ft_darray_get(node->children, 2), stack) < 0)
		return (-1);
	init_command(&command);
	command.command_name = name;
	if (!command.command_name)
		return (-1);
	command.command_flags = MS_OPERATOR | type;
	return (ft_stack_push(stack, &command));
}

static int	build_child(t_command *child, t_stack *commands)
{
	if (!child)
		return (0);
	if (child->command_flags & MS_OPERATOR)
		return (build_operator(child, commands));
	return (1);
}

//builds the operator and distributes redirections accordingly
int	build_operator(t_command *command, t_stack *commands)
{
	t_command	*right;
	t_command	*left;

	right = (t_command *)ft_stack_pop(commands);
	if (!right)
		return (0);
	build_child(right, commands);
	left = (t_command *)ft_stack_pop(commands);
	build_child(left, commands);
	command->left = left;
	command->right = right;
	return (1);
}
