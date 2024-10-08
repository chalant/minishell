/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_semantics.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 16:41:45 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/20 19:03:11 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_SEMANTICS_H
# define MINISHELL_SEMANTICS_H

# include "minishell.h"

int				flatten_tree(t_parse_tree *root, t_stack *stack);
int				build_operator(t_command *command, t_stack *commands);
int				set_redirection(t_redirection *redirection, t_parse_tree *tree);
int				set_redirections(t_command *command, t_parse_tree *tree);
int				create_operator(t_parse_tree *node, t_stack *stack,
					int type, char *name);

char			**get_word(t_parse_tree *node);

void			clear_redirection(void *address);

t_parse_tree	*get_leaf(t_parse_tree *node);

t_command		*build_command(t_darray *command_array,	t_parse_tree *tree);
int				handle_operator(t_parse_tree *node, t_stack *commands);

#endif