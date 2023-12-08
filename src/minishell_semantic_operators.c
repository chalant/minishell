/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_semantic_operators.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:24 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/06 17:47:47 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  build_right_operator(t_command *right, t_command *parent, t_stack *commands)
{
	if (right->command_flags & MS_OPERATOR)
	{
		if (!(parent->command_flags & MS_PIPE))
		{
			if (!right->input)
				right->input = parent->input;
			if (!right->output)
				right->output = parent->output;
		}
		return (build_operator(right, commands));
	}
	return (1);
}

static int  build_left_operator(t_command *left, t_command *parent, t_stack *commands)
{
	if (!left)
		return (0);
	if (left->command_flags & MS_OPERATOR)
	{
		if (!left->input)
			left->input = parent->input;
		if (!left->output)
			left->output = parent->output;
		return (build_operator(left, commands));
	}
	return (1);
}

//builds the operator and distributes redirections if any.
int	build_operator(t_command *command, t_stack *commands)
{
	t_command	*right;
	t_command	*left;

	right = (t_command *)ft_stack_pop(commands);
	build_right_operator(right, command, commands);
	left = (t_command *)ft_stack_pop(commands);
	build_left_operator(left, command, commands);
	if (left && !left->output && !(command->command_flags & MS_PIPE))
		left->output = command->output;
	if (!right->output)
		right->output = command->output;
	if (left && !left->input)
		left->input = command->input;
	if (!right->input && !(command->command_flags & MS_PIPE))
		right->input = command->input;
	command->left = left;
	command->right = right;
	return (1);
}
