/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_semantic_operators.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:24 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/14 16:30:39 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//todo:don't need parent
static int  build_right_operator(t_command *right, t_command *parent, t_stack *commands)
{
	(void)parent;

	if (!right)
		return (0);
	if (right->command_flags & MS_OPERATOR)
		return (build_operator(right, commands));
	return (1);
}

//todo:don't need parent
//todo: set the left most command to be first.
static int  build_left_operator(t_command *left, t_command *parent, t_stack *commands)
{
	if (!left)
		return (0);
	if (parent->command_flags & MS_FIRST)
	{
		parent->command_flags &= ~(MS_FIRST);
		left->command_flags |= MS_FIRST;
	}
	if (left->command_flags & MS_OPERATOR)
		return (build_operator(left, commands));
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
	build_right_operator(right, command, commands);
	left = (t_command *)ft_stack_pop(commands);
	build_left_operator(left, command, commands);
	command->left = left;
	command->right = right;
	return (1);
}
