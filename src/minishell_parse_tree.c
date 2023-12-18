/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parse_tree.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:05:11 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 16:29:48 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_tree(t_parse_tree *tree)
{
	tree->rule_name = NULL;
	tree->children = NULL;
	tree->token = NULL;
	tree->terminal = 1;
	tree->start = 0;
	tree->end = 0;
	tree->start_rule = 0;
	tree->rule = 0;
	return (1);
}

int	init_children(t_parse_tree *parse_tree)
{
	int				i;
	t_parse_tree	subtree;

	parse_tree->children = malloc(sizeof(t_darray));
	if (!parse_tree->children)
		return (-1);
	if (ft_darray_init(parse_tree->children,
			sizeof(t_parse_tree), 3) < 0)
		return (-1);
	i = -1;
	while (++i < 3)
	{
		init_tree(&subtree);
		if (ft_darray_append(parse_tree->children, &subtree) < 0)
			return (-1);
	}
	return (1);
}

void	reset_tree_node(void *node)
{
	t_parse_tree	*tree;

	tree = (t_parse_tree *)node;
	tree->rule_name = NULL;
	tree->terminal = 1;
	tree->start = 0;
	tree->end = 0;
}

int	clear_parse_tree(t_parse_tree *tree,
	int (*del_method)(t_darray *, void(*)(void *)), int del)
{
	int				i;
	int				size;
	t_parse_tree	*child;

	if (!tree->children)
		return (0);
	i = -1;
	while (++i < tree->children->size)
	{
		child = ft_darray_get(tree->children, i);
		clear_parse_tree(child, del_method, del);
	}
	if (tree->children)
	{
		size = tree->children->size;
		del_method(tree->children, reset_tree_node);
		if (!del)
			tree->children->size = size;
	}
	return (0);
}
