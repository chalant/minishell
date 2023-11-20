#ifndef MINISHELL_SEMANTICS_H
# define MINISHELL_SEMANTICS_H

#include "minishell.h"

int			flatten_tree(t_parse_tree *root, t_stack *stack);
int			build_operand(t_command *command, t_stack *commands);
int			build_operator(t_command *command, t_stack *commands);
int			minishell_execute(t_command *command);
t_command	*build_command(t_darray *command_array,	t_parse_tree *tree);

#endif