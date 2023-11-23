#ifndef MINISHELL_SEMANTICS_H
# define MINISHELL_SEMANTICS_H

# include "minishell.h"

int			flatten_tree(t_parse_tree *root, t_stack *stack);
int			build_operator(t_command *command, t_stack *commands);
int			set_redirections(t_command *command, t_parse_tree *tree);
int			create_files(t_command *command, t_darray *redirections);
int			minishell_execute(t_command *command);

char		**get_word(t_parse_tree *node);

t_command	*build_command(t_darray *command_array,	t_parse_tree *tree);

#endif