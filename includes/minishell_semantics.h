#ifndef MINISHELL_SEMANTICS_H
# define MINISHELL_SEMANTICS_H

#include "minishell.h"

int			collapse_tree(t_parse_tree *root, t_stack *stack);
int			execute_and(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status);
int			execute_or(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status);
int			build_operand(t_command *command, t_stack *commands);
int			build_operator(t_command *command, t_stack *commands);
int			minishell_execute(t_command *command);
t_command	*build_command(t_darray *command_array,	t_parse_tree *tree);

#endif