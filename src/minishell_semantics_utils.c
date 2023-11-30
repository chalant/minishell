/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_semantics_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:09 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/23 17:22:10 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_word(t_parse_tree *node)
{
	t_parse_tree	*tree;

	tree = ft_darray_get(node->children, 0);
	while (!tree->terminal)
		tree = ft_darray_get(tree->children, 0);
	//todo: set the rule name to NULL and free the string from the command?
	// or return a copy?
	return (&tree->rule_name);
}
