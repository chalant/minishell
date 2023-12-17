/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_semantic_operators.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yves <yves@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:24 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/17 12:01:03 by yves             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  build_child(t_command *child, t_stack *commands)
{
	if (!child)
		return (0);
	if (child->command_flags & MS_OPERATOR)
		return (build_operator(child	, commands));
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
