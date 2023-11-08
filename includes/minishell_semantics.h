#ifndef MINISHELL_SEMANTICS_H
# define MINISHELL_SEMANTICS_H

#include "minishell.h"

int	create_commands(t_parse_tree *root, t_darray *stack);
int	execute_and(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status);
int	execute_or(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status);
int	execute_command(t_command *command, t_stack *commands, int in_pipe[2], int out_pipe[2], int status);
int	execute_operand(t_command *command, t_stack *commands, int in_pipe[2], int out_pipe[2], int status);
int	execute_operator(t_command *command, t_stack *commands, int in_pipe[2], int out_pipe[2], int status);
int	minishell_execute(t_stack *commands);

#endif